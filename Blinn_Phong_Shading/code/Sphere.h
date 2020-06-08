#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "my_vector.h"

enum objMaterial
{
	ISOTROPIC,
	ANISOTROPIC
};

class Sphere
{
public:
	objMaterial type;
	Vector3f center;
	float radius;
	Vector3f color;
	Sphere(): center(Vector3f(0,0,0)),radius(10),color(Vector3f(0,0,0)), type(ISOTROPIC){}
	Sphere(Vector3f c,float r, Vector3f col, objMaterial t): center(c),radius(r), color(col), type(t) {}

};

#endif // !SPHERE_H
