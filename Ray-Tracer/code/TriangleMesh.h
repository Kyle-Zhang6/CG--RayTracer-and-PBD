#pragma once
#ifndef TriangleMesh_H
#define TriangleMesh_H

#include <string>
#include "OBJ_Loader.hpp"
#include "Object.h"
#include "BVH.h"

void rotate(cv::Vec3f& v, float angle)
{
	angle = angle * PI / 180;
	cv::Mat r = cv::Mat::zeros(cv::Size(3, 3), CV_32FC1);
	r.at<float>(0, 0) = cos(angle);
	r.at<float>(0, 2) = sin(angle);
	r.at<float>(2, 0) = -sin(angle);
	r.at<float>(2, 2) = cos(angle);
	r.at<float>(1, 1) = 1;
	//std::cout << r << std::endl;
	v = cv::Mat(r * v);
}

/****************************************/
/*To describe a Mesh object in the scene*/
/****************************************/

namespace obj
{
	class TriangleMesh : public Object
	{
	public:
		objl::Mesh* mesh;
		std::vector<std::shared_ptr<Triangle>> triangles;
		cv::Mat texture;
		BVHNode* root;

		TriangleMesh(objl::Mesh* m, float scale, cv::Vec3f trans, float angle)
		{
			mesh = m;
			for (int i = 0; i < m->Vertices.size(); i += 3)
			{

				std::shared_ptr<Triangle> tri = std::make_shared<Triangle>();
				tri->v0 = cv::Vec3f(m->Vertices[i].Position.X, m->Vertices[i].Position.Y, m->Vertices[i].Position.Z);
				tri->v1 = cv::Vec3f(m->Vertices[i + 1].Position.X, m->Vertices[i + 1].Position.Y, m->Vertices[i + 1].Position.Z);
				tri->v2 = cv::Vec3f(m->Vertices[i + 2].Position.X, m->Vertices[i + 2].Position.Y, m->Vertices[i + 2].Position.Z);
				rotate(tri->v0, angle);
				rotate(tri->v1, angle);
				rotate(tri->v2, angle);
				tri->v0 = tri->v0*scale + trans;
				tri->v1 = tri->v1*scale + trans;
				tri->v2 = tri->v2*scale + trans;

				tri->n0 = cv::normalize(cv::Vec3f(m->Vertices[i].Normal.X, m->Vertices[i].Normal.Y, m->Vertices[i].Normal.Z));
				tri->n1 = cv::normalize(cv::Vec3f(m->Vertices[i + 1].Normal.X, m->Vertices[i + 1].Normal.Y, m->Vertices[i + 1].Normal.Z));
				tri->n2 = cv::normalize(cv::Vec3f(m->Vertices[i + 2].Normal.X, m->Vertices[i + 2].Normal.Y, m->Vertices[i + 2].Normal.Z));
				rotate(tri->n0, angle);
				rotate(tri->n1, angle);
				rotate(tri->n2, angle);

				tri->t0 = cv::Vec2f(m->Vertices[i].TextureCoordinate.X, m->Vertices[i].TextureCoordinate.Y);
				tri->t1 = cv::Vec2f(m->Vertices[i + 1].TextureCoordinate.X, m->Vertices[i + 1].TextureCoordinate.Y);
				tri->t2 = cv::Vec2f(m->Vertices[i + 2].TextureCoordinate.X, m->Vertices[i + 2].TextureCoordinate.Y);

				triangles.push_back(tri);
			}

			root = kdTreeInit(triangles);
		};

		void textureInit(const std::string& name)
		{
			texture = cv::imread(name);
			cv::cvtColor(texture, texture, cv::COLOR_RGB2BGR);
		}

		bool intersect(const Ray& ray, float t_min, float t_max, Intersection& i, float& u, float& v)
		{
			/*for (auto& tri : triangles)
			{
				Intersection i_temp;
				if (tri->intersect(ray, t_min, t_max, i_temp, u, v))
				{
					if (i_temp.time < i.time)
					{
						i = i_temp;
					}
				}
			}*/

			intersectWithKDTree(root, ray, t_min, t_max, i, u, v);

			if (i.exist)
			{
				i.obj = this;
				if (texture.dims == 0) i.color = material->Kd;
				else i.color = getTextureBilinear(u, v);
				//i.color = getTexture(u, v);
				return true;
			}
			else
				return false;
		}

		AABB getAABB()
		{
			return root->box;
		}

		cv::Color getTexture(float u, float v)
		{
			if (u > 1) u = 1;
			else if (u < 0) u = 0;
			if (v > 1) v = 1;
			else if (v < 0) v = 0;
			auto color = texture.at<Vec3b>((int)round((1 - v)*(texture.rows - 1)), (int)round(u*(texture.cols - 1)));
			return cv::Color(color[0], color[1], color[2]) / 255;
		}

		cv::Color getTextureBilinear(float u, float v)
		{
			float u_img = u * texture.cols;
			float v_img = (1 - v) * texture.rows;
			if (u_img < 0) { u_img = 0; }
			else if (u_img >= texture.cols) { u_img = texture.cols; }
			if (v_img < 0) { v_img = 0; }
			else if (v_img >= texture.rows) { v_img = texture.rows; }

			int u11 = round(u_img);
			int v11 = round(v_img);
			cv::Vec3f color, color0, color1;
			if (u11 == 0 || u11 == texture.cols)
			{
				if (v11 == 0 || v11 == texture.rows)
				{
					color = texture.at<cv::Vec3b>(std::max(0, v11 - 1), std::max(0, u11 - 1));
				}
				else
				{
					color0 = texture.at<cv::Vec3b>(v11 - 1, std::max(0, u11 - 1));
					color1 = texture.at<cv::Vec3b>(v11, std::max(0, u11 - 1));
					color = lerp(v_img, color0, color1);
				}
			}
			else
			{
				if (v11 == 0 || v11 == texture.rows)
				{
					color0 = texture.at<cv::Vec3b>(std::max(0, v11 - 1), u11 - 1);
					color1 = texture.at<cv::Vec3b>(std::max(0, v11 - 1), u11);
					color = lerp(u_img, color0, color1);
				}
				else
				{
					cv::Vec3f color00, color01, color10, color11;
					color00 = texture.at<cv::Vec3b>(v11 - 1, u11 - 1);
					color01 = texture.at<cv::Vec3b>(v11, u11 - 1);
					color10 = texture.at<cv::Vec3b>(v11 - 1, u11);
					color11 = texture.at<cv::Vec3b>(v11, u11);
					color0 = lerp(v_img, color00, color01);
					color1 = lerp(v_img, color10, color11);
					color = lerp(u_img, color0, color1);
				}
			}
			return color / 255;
		}

		cv::Vec3f lerp(float a, cv::Vec3f color0, cv::Vec3f color1)
		{
			float b = a - round(a);
			return color0 + (color1 - color0) * (0.5 + b);
		}

		

	};
}





#endif // !TriangleMesh
