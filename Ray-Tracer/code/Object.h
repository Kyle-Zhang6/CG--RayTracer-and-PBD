#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <opencv2/opencv.hpp>
#include <memory>
#include "Ray.h"
#include "Material.h"
#include "AABB.h"

class Object;

/*****************************************************/
/*To describe a intersection between ray and a object*/
/*****************************************************/
struct Intersection
{
	bool exist;
	float time;
	cv::Vec3f point;
	cv::Vec3f normal;
	cv::Color color;
	Object* obj;

	Intersection()
	{
		exist = false;
		time = INFINITY;
		obj = nullptr;
	}
};


/********************************************************/
/*Father class of any other specific classes like Sphere*/
/********************************************************/
class Object
{
public:
	std::shared_ptr<Material> material;	

	Object() { material = std::make_shared<Material>(); }
	~Object() {};

	// To get the intersection between the object and the ray
	virtual bool intersect(const Ray& ray, float t_min, float t_max, Intersection& i, float& u, float& v) = 0;
	virtual AABB getAABB() = 0;
};


/******************************************/
/*To describe a Sphere object in the scene*/
/******************************************/
class Sphere : public Object
{
public:
	cv::Vec3f center;
	float radius;

	Sphere(cv::Vec3f c, float r) : center(c), radius(r) {};
	bool intersect(const Ray& ray, float t_min, float t_max, Intersection& i, float& u, float& v);
	AABB getAABB();

};


/********************************************/
/*To describe a Triangle object in the scene*/
/********************************************/
class Triangle : public Object
{
public:
	cv::Vec3f v0, v1, v2; // vertex
	cv::Vec2f t0, t1, t2; // texture
	cv::Vec3f n0, n1, n2; // normal

	Triangle() {};
	Triangle(cv::Vec3f vv0, cv::Vec3f vv1, cv::Vec3f vv2) : v0(vv0), v1(vv1), v2(vv2) {};
	bool intersect(const Ray& ray, float t_min, float t_max, Intersection& i, float& u, float& v);
	AABB getAABB();
};




#endif