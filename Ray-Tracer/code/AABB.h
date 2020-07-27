#pragma once
#ifndef AABB_H
#define AABB_H

#include <opencv2/opencv.hpp>
#include "Ray.h"

using namespace cv;

class AABB
{
public:
	Vec3f cmin, cmax;

	AABB();
	AABB(Vec3f p);
	
	void merge(const AABB& box);
	void overlap(const AABB& box);
	bool intersect(const Ray& ray);
	Vec3f getCentroid();
	int longEdge();
};

AABB merge(const AABB& box1, const AABB& box2);
Vec3f invVec3f(const Vec3f& v);
Vec3f min(const Vec3f& v1, const Vec3f& v2);
Vec3f max(const Vec3f& v1, const Vec3f& v2);
bool isEqual(const Vec3f& v1, const Vec3f& v2);

#endif // !AABB_H
