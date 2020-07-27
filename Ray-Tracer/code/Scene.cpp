#include "Scene.h"

#define EPSILON 1e-2



Vec3f reflect(const Vec3f& d, const Vec3f& n)
{
	return d - 2 * d.dot(n)*n;
}

bool refract(const Vec3f& d, const Vec3f& n, const float nf, const float nt, Vec3f& t)
{
	// nf: n_from; nt: n_to
	float c = d.dot(n);
	float sqrcos = 1 - (1 - c * c)*(nf * nf) / (nt*nt);
	if (sqrcos < 0) return false;
	else
	{
		t = (nf / nt)*(d - c * n) - sqrt(sqrcos)*n;
		return true;
	}
}

Color Scene::phongShading(Intersection& i, const Vec3f& eye)
{
	Ray r = Ray();
	r.orig = i.point;
	// This is for shading a isotropic sphere
	Color La = i.obj->material->Ka.mul(background);
	Color Ld{ 0,0,0 };
	Color Ls{ 0,0,0 };
	for (auto& light : light_list)
	{
		Vec3f l, n;
		Vec3f current_light_pos;
		Color Ld_temp{ 0,0,0 };
		Color Ls_temp{ 0,0,0 };
		for (int k = 0; k < light->u_num; k++)
		{
			for (int j = 0; j < light->v_num; j++)
			{
				current_light_pos = light->base_position + light->u * k + light->v * j;
				r.dir = normalize(current_light_pos - r.orig);
				l = r.dir;
				n = i.normal;
				Intersection obj_i;
				this->intersect(r, obj_i);
				if (obj_i.exist)
				{
					float light_reach_time = norm(light->base_position - r.orig);

					if (light_reach_time > obj_i.time) continue;
				}
				Ld_temp += (i.color).mul((light->intensity)*max(0.0f, n.dot(l)));

				Vec3f r = -l + 2 * (l.dot(n))*n;
				Ls_temp += (i.obj->material->Ks).mul((light->intensity)*pow(max(0.0f, eye.dot(r)), i.obj->material->p));
			}
		}
		
		Ld += Ld_temp / (light->u_num*light->v_num);
		Ls += Ls_temp / (light->u_num*light->v_num);

	}

	return La + Ld + Ls;
	//return Ls;
}

// Get the nearest intersection with a given ray 
// and all of the objects in the scene
void Scene::intersect(const Ray& ray,Intersection& i_nearest)
{
	/*Intersection i;
	for (auto& object : object_list)
	{
		float u, v;
		object->intersect(ray, EPSILON, INFINITY, i, u, v);
		if (i.exist && i.time < i_nearest.time)
		{
			i_nearest = i;
		}
	}
	return;*/
	float u, v;
	intersectWithKDTree(root, ray, EPSILON, INFINITY, i_nearest, u, v);
}

// Get the color for one pixel with the ray omit from it
Color Scene::getColor(const Ray& ray, const int depth)
{
	if (depth > max_depth) return Color{ 0,0,0 };

	Intersection i;
	intersect(ray,i);
	if (i.exist)
	{
		Color phong_color = phongShading(i, -ray.dir) * 255;
		Color r_color;
		Ray ray_reflect{};
		ray_reflect.orig = i.point;
		ray_reflect.dir = reflect(ray.dir, i.normal);

		Vec3f k;
		if (i.obj->material->has_refraction)
		{
			Vec3f refract_dir;
			bool total_reflection = 0;
			float c;
			if (ray.dir.dot(i.normal) < 0)
			{
				
				refract(ray.dir, i.normal, 1, i.obj->material->ior, refract_dir);
				c = -ray.dir.dot(i.normal);
				k = Vec3f(1, 1, 1);
			}
			else
			{
				k[0] = exp(-i.obj->material->attenuation[0] * i.time);
				k[1] = exp(-i.obj->material->attenuation[1] * i.time);
				k[2] = exp(-i.obj->material->attenuation[2] * i.time);
				if (refract(ray.dir, -i.normal, i.obj->material->ior, 1, refract_dir))
				{
					c = refract_dir.dot(i.normal);
				}
				else
				{
					// Total reflection
					r_color = k.mul(getColor(ray_reflect, depth + 1));
					total_reflection = 1;
				}
			}
			if (!total_reflection)
			{
				Ray ray_refract{};
				ray_refract.orig = i.point;
				ray_refract.dir = refract_dir;
				float R0 = (i.obj->material->ior - 1) / (i.obj->material->ior + 1);
				R0 = R0 * R0;
				float R = R0 + (1 - R0)*pow((1 - c), 5);
				r_color = k.mul(R*getColor(ray_reflect, depth + 1) + (1 - R)*getColor(ray_refract, depth + 1));
			}
			
		}
		else
		{
			r_color = getColor(ray_reflect, depth + 1);
		}
		
		return phong_color + i.obj->material->Km.mul(r_color);
	}
		

	//float t = 0.5*(ray.dir[1] + 1);
	//return ((1 - t) * Color(1, 1, 1) + t * background)*255;
	return background * 255;
}
