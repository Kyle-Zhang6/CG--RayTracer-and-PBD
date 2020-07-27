#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <opencv2/opencv.hpp>
#include "global.h"

//enum MaterialType { DIFFUSE, REFLECTION_AND_REFRACTION, REFLECTION };

class Material
{
public:
	//MaterialType type;
	cv::Color Ka;
	cv::Color Kd;				// Diffusion term color
	cv::Color Ks;				// Specular term color
	cv::Color Km;				// Mirror specular term
	float p;					// Specular exponent
	bool has_refraction;		// If this material has refraction
	float ior;					// index of refraction
	cv::Vec3f attenuation;		// The higher, the more attenuation

	Material()
	{
		Ka = cv::Color(0.05, 0.05, 0.05);
		Kd = cv::Color(0.5, 0.5, 0.5);
		Ks = cv::Color(0.5, 0.5, 0.5);
		Km = cv::Color(0, 0, 0);
		p = 100;
		has_refraction = false;
		ior = 1.5;
		attenuation = cv::Vec3f(0, 0, 0);
	}
};

#endif // !MATERIAL_H
