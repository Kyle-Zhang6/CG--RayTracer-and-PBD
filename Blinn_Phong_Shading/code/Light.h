#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "my_vector.h"

enum lightType
{
	POINT, DIRECTIONAL
};

class Light
{
public:
	lightType type;
	// If this is a point light, 'pos' stands for the position of it;
	// If this is a directional light, 'pos' stands for its direction.
	Vector3f pos;
	Vector3f intensity;
	Light(): type(POINT),pos(Vector3f(500,500,50)),intensity(Vector3f(1.0f,1.0f,1.0f)) {}
	Light(lightType t,Vector3f position, Vector3f intens): type(t),pos(position),intensity(intens) {}
};

#endif // !LIGHT_H
