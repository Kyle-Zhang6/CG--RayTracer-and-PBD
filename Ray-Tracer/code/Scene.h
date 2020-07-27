#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <algorithm>
#include <opencv2/opencv.hpp>
#include <vector>
#include "Ray.h"
#include "Object.h"
#include "global.h"
#include "Light.h"
#include "BVH.h"

using namespace cv;

class Scene
{
public:
	int max_depth;
	Color background;
	std::vector<std::shared_ptr<Object>> object_list;
	std::vector<std::shared_ptr<AreaLight>> light_list;
	BVHNode* root;

	Scene() { max_depth = 5; background = Color(0.75, 0.85, 1.0); }

	void addObject(std::shared_ptr<Object> obj) { object_list.push_back(obj); }
	void addLight(std::shared_ptr<AreaLight> light) { light_list.push_back(light); }
	Color getColor(const Ray& ray, const int depth);
	Color phongShading(Intersection& i, const Vec3f& eye);
	void intersect(const Ray& ray, Intersection& i_nearest);
};

#endif