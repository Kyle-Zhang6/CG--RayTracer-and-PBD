#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Mesh.h"

class Scene {
public:
	std::vector<Mesh*> meshes;
	float rho; // Air density

	Scene() {};
	Scene(float r) : rho(r) {};
	~Scene() {};

	void addMesh(Mesh* mesh) { meshes.push_back(mesh); }
	void getWindVector(vec3 result, float t, vec3 pos)
	{
		// Wind should be related to time and position in the scene
		// Currently use a constant vector (0,0,0) to represent no wind
		result[0] = 0;
		result[1] = 0;
		result[2] = 0;
	}
};

class FrameSeq {
public:
	int frame_rate;
	std::pair<float, float> interval; //render interval (t0, t1)
	std::vector<Scene*> frames;
};


#endif // !SCENE_H
