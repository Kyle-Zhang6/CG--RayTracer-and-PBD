#pragma once
#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "fast_math.h"
#include "Scene.h"
#include "Mesh.h"

class Actuator
{
public:
	////method to simulate an element
	//void actuate(Element & e_out, const Element & e_in, int t0,
	//	int t1, int framerate);
	////method to simulate a scene¡¯s meshes¡¯ elements
	void actuate(Scene & out, Scene & in, int t0, int t1, int framerate);
};


#endif