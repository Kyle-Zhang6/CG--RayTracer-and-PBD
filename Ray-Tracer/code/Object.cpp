#include "Object.h"

float sum(cv::Vec3f v)
{
	return v[0] + v[1] + v[2];
}

bool Sphere::intersect(const Ray& ray, float t_min, float t_max, Intersection& i, float& u, float& v)
{
	cv::Vec3f dist = ray.orig - center;
	float half_b = sum(ray.dir.mul(dist));
	float a = ray.dir[0] * ray.dir[0] + ray.dir[1] * ray.dir[1] + ray.dir[2] * ray.dir[2];
	float c = (dist[0] * dist[0] + dist[1] * dist[1] + dist[2] * dist[2]) - radius * radius;
	float delta = half_b * half_b - a*c;
	if (delta >= 0)
	{
		float t = (-half_b - sqrt(delta)) / a;
		if (t >= t_min && t <= t_max)
		{
			i.exist = true;
			i.obj = this;
			i.time = t;
			i.point = ray.at(i.time);
			i.normal = cv::normalize(i.point - center);
			i.color = this->material->Kd;
			return true;
		}
		t = (-half_b + sqrt(delta)) / a;
		if (t >= t_min && t <= t_max)
		{
			i.exist = true;
			i.obj = this;
			i.time = t;
			i.point = ray.at(i.time);
			i.normal = cv::normalize(i.point - center);
			i.color = this->material->Kd;
			return true;
		}
	}
	return false;
}

AABB Sphere::getAABB()
{
	AABB box;
	box.cmax = center + cv::Vec3f(radius, radius, radius);
	box.cmin = center - cv::Vec3f(radius, radius, radius);
	return box;
}

bool Triangle::intersect(const Ray& ray, float t_min, float t_max, Intersection& i, float& u, float& v)
{
	cv::Vec3f E1 = v1 - v0;
	cv::Vec3f E2 = v2 - v0;
	cv::Vec3f S = ray.orig - v0;
	cv::Vec3f S1 = ray.dir.cross(E2);
	cv::Vec3f S2 = S.cross(E1);
	cv::Vec3f result(S2.dot(E2), S1.dot(S), S2.dot(ray.dir));
	result = result / S1.dot(E1);
	if (result[0] > t_min && result[0] < t_max && result[1] > 0 && result[2] > 0 && (result[1] + result[2]) < 1)
	{
		u = result[1];
		v = result[2];
		i.exist = true;
		i.time = result[0];
		i.obj = this;
		i.point = (1 - u - v)*v0 + u * v1 + v * v2;
		i.normal = (1 - u - v)*n0 + u * n1 + v * n2;
		//i.normal = cv::normalize(E1.cross(E2));
		i.color = this->material->Kd;
		cv::Vec2f texture_coordinate = (1 - u - v)*t0 + u * t1 + v * t2;
		u = texture_coordinate[0];
		v = texture_coordinate[1];

		return true;
	}
	else
	{
		i.exist = false;
		return false;
	}
}

AABB Triangle::getAABB()
{
	AABB box;
	box.cmin = cv::Vec3f(min(v2, min(v0, v1)));
	box.cmax = cv::Vec3f(max(v2, max(v0, v1)));
	return box;
}

