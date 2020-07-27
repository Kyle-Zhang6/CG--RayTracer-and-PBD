#include "AABB.h"
#include <algorithm>

Vec3f invVec3f(const Vec3f& v)
{
	return Vec3f(1 / v[0], 1 / v[1], 1 / v[2]);
}

Vec3f min(const Vec3f& v1, const Vec3f& v2)
{
	return Vec3f(min(v1[0], v2[0]), min(v1[1], v2[1]), min(v1[2], v2[2]));
}

Vec3f max(const Vec3f& v1, const Vec3f& v2)
{
	return Vec3f(max(v1[0], v2[0]), max(v1[1], v2[1]), max(v1[2], v2[2]));
}

bool isEqual(const Vec3f& v1, const Vec3f& v2)
{
	return (norm(v1 - v2) < 1e-6);
}

AABB merge(const AABB& box1, const AABB& box2)
{
	AABB box;
	box.cmin = min(box1.cmin, box2.cmin);
	box.cmax = max(box1.cmax, box2.cmax);
	return box;
}


AABB::AABB()
{
	cmin = Vec3f(-INFINITY, -INFINITY, -INFINITY); 
	cmax = Vec3f(INFINITY, INFINITY, INFINITY);
}

AABB::AABB(Vec3f v)
{
	cmin = v;
	cmax = v;
}

void AABB::merge(const AABB& box)
{
	cmin = min(cmin, box.cmin);
	cmax = max(cmax, box.cmax);
	return;
}

void AABB::overlap(const AABB& box)
{
	cmin = max(cmin, box.cmin);
	cmax = min(cmax, box.cmax);
	return;
}

bool AABB::intersect(const Ray& ray)
{
	Vec3f t_cmin = (cmin - ray.orig).mul(invVec3f(ray.dir));
	Vec3f t_cmax = (cmax - ray.orig).mul(invVec3f(ray.dir));
	Vec3f p;
	for (int i = 0; i < 3; i++)
	{
		p = ray.at(t_cmin[i]);
		if (isEqual(cmin, min(cmin, p)) && isEqual(cmax, max(cmax, p))) return true;

		p = ray.at(t_cmax[i]);
		if (isEqual(cmin, min(cmin, p)) && isEqual(cmax, max(cmax, p))) return true;
	}
	return false;
}

Vec3f AABB::getCentroid()
{
	return (cmin + cmax) / 2;
}

int AABB::longEdge()
{
	Vec3f delta = cmax - cmin;
	if (delta[0] >= delta[1] && delta[0] >= delta[2])
	{
		return 0;
	}
	else if (delta[1] >= delta[2])
	{
		return 1;
	}
	else
		return 2;
}