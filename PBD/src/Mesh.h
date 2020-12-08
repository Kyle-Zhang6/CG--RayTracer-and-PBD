#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>
#include "fast_math.h"

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 uv;
};

struct Element {
	vec3 position;
	vec3 velocity;
	vec3 acceleration;
	//vec3 force;
	float w = 1000; // (1/mass), to save time when calculating

	//other parameters to be implemented by actuator needs
	/*float elasticity;
	float mass;*/
};


struct Face {
	std::vector<int> indices; //this points to the vertex list to retrieve vertices
	Element* e; //physic element contained in this face
};

class Mesh {
public:
	std::vector<Vertex*> vertices;
	std::vector<Face*> faces;

	virtual char getClassType() { return 'm'; }
};




#endif // !MESH_H
