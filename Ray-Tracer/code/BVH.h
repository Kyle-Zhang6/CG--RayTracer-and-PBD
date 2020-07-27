#pragma once
#ifndef BVH_H
#define BVH_H

#include <vector>
#include <memory>
#include "Object.h"
#include "AABB.h"

struct BVHNode
{
	AABB box;
	BVHNode* left;
	BVHNode* right;
	Object* obj;
};

BVHNode* kdTreeInit(std::vector<std::shared_ptr<Object>> obj_list);
BVHNode* kdTreeInit(std::vector<std::shared_ptr<Triangle>> triangles);
void intersectWithKDTree(BVHNode* node, const Ray& ray, float tmin, float tmax, Intersection& i_nearest, float& u, float& v);

#endif // !BVH_H
