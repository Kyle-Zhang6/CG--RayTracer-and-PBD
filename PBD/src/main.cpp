#include <iostream>
#include "Mesh.h"
#include "Cloth.h"
#include "Scene.h"
#include "global.h"

using namespace std;
#define column	15
#define row		10

int main(int argc, char *argv[])
{	
	Scene scene(1.29);

	Cloth cloth(0.01);
	vec3 p = { 0 };
	vec3 ep = { 0 };
	for (int j = 0; j < row; j++)
	{
		for (int i = 0; i < column; i++)
		{
			p[0] = (float)i;
			p[2] = (float)j;
			Vertex* v = new Vertex();
			vec3_copy(v->position, p);
			cloth.vertices.push_back(v);
		}
	}

	for (int j = 0; j < row-1; j++)
	{
		for (int i = 0; i < column-1; i++)
		{
			ep[0] = float(i) + 0.5;
			ep[2] = float(j) + 0.5;
			Face* f = new Face();
			f->e = new Element();
			vec3_copy(f->e->position, ep);
			f->indices.push_back(i + column * j);
			f->indices.push_back(i + column * j + 1);
			f->indices.push_back(i + column * j + column + 1);
			f->indices.push_back(i + column * j + column);
			cloth.faces.push_back(f);
		}
	}

	scene.addMesh(&cloth);

	cloth.init();
	cloth.map[0]->cur_e->w = 0;
	cloth.map[0]->old_e->w = 0;
	cloth.map[column-2]->cur_e->w = 0;
	cloth.map[column-2]->old_e->w = 0;
	cloth.map[(column - 1)*(row - 2)]->cur_e->w = 0;
	cloth.map[(column - 1)*(row - 2)]->old_e->w = 0;
	cloth.map[(column - 1)*(row - 1)-1]->cur_e->w = 0;
	cloth.map[(column - 1)*(row - 1)-1]->old_e->w = 0;

	float dt = 0.1;
	int t = 10/dt;
	int a = 0;
	while (a<t)
	{

#ifdef SHOWPROCESS
		printf("\n\n-----------------------\n");
		for (int j = 0; j < row - 1; j++)
		{
			for (int i = 0; i < column - 1; i++)
			{
				printf("%.3f,", cloth.map[i + j * (column - 1)]->cur_e->position[1]);
				//cout << cloth.map[i + j * 4]->cur_e->position[1] << "";
			}
			printf("\n");
		}
#endif // SHOWPROCESS
		
		cloth.verletIntegrate(dt);
		
		cloth.iteration(60000,0.001*cloth.s_constraint.size());// limit it as 1cm error for each constraint 
		cloth.updateVelocity(dt);
		cloth.updateAcc(scene, dt*a);
		
		a++;
		printf("\rProcess: %.2f %%", ((float)a / t)*100);
	}

	printf("\n\n\n------------Final Frame------------\n");
	for (int j = 0; j < row - 1; j++)
	{
		for (int i = 0; i < column - 1; i++)
		{
			printf("%.3f,", cloth.map[i + j * (column - 1)]->cur_e->position[1]);
			//cout << cloth.map[i + j * 4]->cur_e->position[1] << "";
		}
		printf("\n");
	}
	
	return 0;
}