#pragma once
#ifndef CLOTH_H
#define CLOTH_H

#include "Scene.h"
#include "Mesh.h"
#include "fast_math.h"

//struct Spring
//{
//	int p1;				// Partical NO.1
//	int p2;				// Partical NO.2
//	float rest_length;	// 
//	float ks;			// stiffness fector
//	float kd;			// damping factor
//};

struct ElementMap
{
	Element* old_e;
	Element* cur_e;
	int index;

	ElementMap()
	{
		old_e = new Element();
		cur_e = new Element();
	}
};

struct StretchContraint
{
	int index1;
	int index2;
	float rest_length;
};

class Cloth : public Mesh
{
public:
	float cw;						// Wind reaction Coefficient
	std::vector<ElementMap*> map;	// Ordered map towards initial faces
	std::vector<StretchContraint*> s_constraint;	// List of Stretch Constraint
	//std::vector<Spring*> springs;	// Springs that describe the connecting ralationships between faces

	Cloth(float c):cw(c) {};
	~Cloth() {};
	virtual char getClassType() { return 'c'; }
	void init();
	void verletIntegrate(float dt);
	void iteration(const int num_iteration, const float err_converge);
	void updateAcc(Scene scene, float t);
	void updateVelocity(float dt);
};

void vec3_div(vec3 v, float n);
void vec3_scale(vec3 v, float n);

#endif // !CLOTH_H
