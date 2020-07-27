#pragma once
#ifndef RAY_H
#define RAY_H

#include <opencv2/opencv.hpp>

class Ray
{
public:
	cv::Vec3f orig;	// Origin of the ray
	cv::Vec3f dir;	// Direction of the ray --- ought to be a unit vector

	Ray() {};
	Ray(const cv::Vec3f& o, const cv::Vec3f& d) : orig(o), dir(cv::normalize(d)) {};

	cv::Vec3f at(float t) const { return orig + t * dir; } // Calculate the position of the Ray at the moment of t.
};


#endif