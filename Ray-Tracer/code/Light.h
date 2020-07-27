#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <opencv2/opencv.hpp>
#include <memory>
#include "Object.h"
#include "global.h"

class Light
{
public:
	cv::Vec3f intensity;
	cv::Vec3f base_position;

	Light(cv::Vec3f pos, cv::Vec3f intens) : base_position(pos), intensity(intens) {};
	
};


class AreaLight: public Light
{
public:
	cv::Vec3f u;
	cv::Vec3f v;
	int u_num;
	int v_num;

	AreaLight(cv::Vec3f pos, cv::Vec3f intens, cv::Vec3f uu, cv::Vec3f vv, float num_u, float num_v) :
		Light(pos,intens), u(uu), v(vv), u_num(num_u), v_num(num_v) {};
};

#endif // !LIGHT_H
