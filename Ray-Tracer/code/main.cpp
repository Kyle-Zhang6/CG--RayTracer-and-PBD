#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>
#include <memory>
#include <ctime>

#include <opencv2/opencv.hpp>
#include "Ray.h"
#include "Scene.h"
#include "Object.h"
#include "Light.h"
#include "global.h"
#include "TriangleMesh.h"
#include "BVH.h"
#include "getopt.h"

using namespace std;
using namespace cv;


bool antialiasing = 0;

/* get the index for the vector storing color info. */
int getIndex(int x, int y)
{
	return (HEIGHT - y - 1)*WIDTH + x;
}

int main(int argc, char *argv[])
{
	float aspect_ratio = (float)WIDTH / HEIGHT;
	float fov = 90;
	Vec3f eye{ 0,0,5 }; // Position of the EYE
	float viewport_height = 2 * eye[2] * tan(fov*PI / 360);
	float viewport_width = viewport_height * aspect_ratio;

	printf("Initiating Scene Information...\n");
	/*Initiate the scene with its objects*/
	Scene scene;
	objl::Loader Loader;
	vector<shared_ptr<obj::TriangleMesh>> obj;
	shared_ptr<Sphere> sphere;

	int opt;
	int digit_optind = 0;
	int option_index = 0;
	char *string = (char*)"m:";
	static struct option long_options[] =
	{
		{(char*)"mdl", required_argument,NULL, 'x'},
		{(char*)"rev", required_argument,NULL, 'r'},// scale, translation, rotation
		{(char*)"tex", required_argument,NULL, 't'},
		{(char*)"l", required_argument,NULL, 'l'},
		{(char*)"kd", required_argument,NULL, 'd'},
		{(char*)"ks", required_argument,NULL, 's'},
		{(char*)"km", required_argument,NULL, 'm'},
		{(char*)"att", required_argument,NULL, 'a'},
		{(char*)"sph", required_argument,NULL, 'p'},
		{(char*)"refract", no_argument,NULL, 'f'},
		{(char*)"anti", no_argument,NULL, 'y'},
		{(char*)"lc", required_argument,NULL, 'c'},
		{(char*)"bc", required_argument,NULL, 'b'},
		{(char*)"depth", required_argument,NULL, 'h'},
		{(char*)"add", no_argument,NULL, 'z'},
		{NULL,     0,                      NULL, 0}
	};
	while ((opt = getopt_long_only(argc, argv, string, long_options, &option_index)) != -1)
	{
		//if (opt >= '10') continue;
		switch (opt)
		{
		case 'b':
			scene.background = Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1]));
			break;

		case 'z':
			for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
			{
				scene.addObject(obj[i]);
			}
			if (sphere != nullptr)
			{
				scene.addObject(sphere);
			}
			sphere = nullptr;
			break;

		case 'x':
			Loader.LoadFile(optarg);
			
			break;

		case 'r':
			for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
			{
				if (i >= obj.size())
				{
					obj.push_back(make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[i], atof(optarg),
						Vec3f(atof(argv[optind]), atof(argv[optind + 1]), atof(argv[optind + 2])), atof(argv[optind + 3])));
				}
				else
				{
					obj[i] = make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[i], atof(optarg),
						Vec3f(atof(argv[optind]), atof(argv[optind + 1]), atof(argv[optind + 2])), atof(argv[optind + 3]));
				}
				
			}
			break;

		case 't':
			for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
			{
				if (strcmp(optarg, "null") != 0)
				{
					obj[i]->textureInit(optarg);
				}
				
			}
			
			break;

		case 'l':
			scene.addLight(make_shared<AreaLight>(Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1])),
				Vec3f(0, 0, 0),
				Vec3f(atof(argv[optind] + 2), atof(argv[optind] + 3), atof(argv[optind + 4])),
				Vec3f(atof(argv[optind] + 5), atof(argv[optind] + 6), atof(argv[optind + 7])),
				atof(argv[optind + 8]), atof(argv[optind + 9])));
			break;

		case 'c':
			scene.light_list[scene.light_list.size() - 1]->intensity =
				Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1]));
			break;

		case 'd':
			for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
			{
				obj[i]->material->Kd = Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1]));
			}
			if (sphere)
			{
				sphere->material->Kd = Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1]));
			}
			break;

		case 's':
			for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
			{
				obj[i]->material->Ks = Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1]));
			}
			if (sphere)
			{
				sphere->material->Ks = Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1]));
			}
			break;

		case 'm':
			for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
			{
				obj[i]->material->Km = Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1]));
			}
			if (sphere)
			{
				sphere->material->Km = Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1]));
			}
			break;

		case 'a':
			for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
			{
				obj[i]->material->attenuation = Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1]));
			}
			if (sphere)
			{
				sphere->material->attenuation = Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1]));
			}
			break;

		case 'f':
			for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
			{
				obj[i]->material->has_refraction = 1;
			}
			if (sphere)
			{
				sphere->material->has_refraction = 1;
			}
			break;

		case 'y':
			antialiasing = 1;
			break;

		case 'p':
			sphere = make_shared<Sphere>(Vec3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1])), 
				atof(argv[optind + 2]));
			break;

		case 'h':
			scene.max_depth = atof(optarg);
			break;

		default: printf("*ERROR: Unrecognized option.\n");
			break;
		}
	}

	/*Loader.LoadFile("../../models/panda/panda.obj");
	shared_ptr<obj::TriangleMesh> obj1 = make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[0], 10,Vec3f(1,-0.5,-2),-20);
	obj1->textureInit("../../models/spot/spot_texture.png");
	obj1->textureInit("../../models/panda/panda.png");
	scene.addObject(obj1);*/

	//Loader.LoadFile("../../models/grassland/grassland.obj");
	//shared_ptr<obj::TriangleMesh> ground1 = make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[0], 30, Vec3f(-2, -1.5, -3), 0);
	//shared_ptr<obj::TriangleMesh> ground_left1 = make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[1], 30, Vec3f(-2, -1.5, -3), 0);
	//ground1->textureInit("../../models/grassland/grassland.png");
	//ground_left1->textureInit("../../models/grassland/6.png");
	//scene.addObject(ground1);
	//scene.addObject(ground_left1);

	//shared_ptr<obj::TriangleMesh> ground2 = make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[0], 30, Vec3f(2, -1.5, -3), 0);
	//shared_ptr<obj::TriangleMesh> ground_left2 = make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[1], 30, Vec3f(2, -1.5, -3), 0);
	//ground2->textureInit("../../models/grassland/grassland.png");
	//ground_left2->textureInit("../../models/grassland/6.png");
	//scene.addObject(ground2);
	//scene.addObject(ground_left2);

	//Loader.LoadFile("../../models/flower/Purple_flower.obj");
	//shared_ptr<obj::TriangleMesh> flower = make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[0], 2, Vec3f(2, -1.1, 1), 0);
	//flower->textureInit("../../models/flower/texture.png");
	//scene.addObject(flower);

	//Loader.LoadFile("../../models/bee/bee.obj");
	//shared_ptr<obj::TriangleMesh> bee = make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[0], 1, Vec3f(3, 0, -1), -170);
	//bee->textureInit("../../models/bee/bee.png");
	//scene.addObject(bee);

	//Loader.LoadFile("../../models/Rock_fountain/Rock_fountain.obj");
	//shared_ptr<obj::TriangleMesh> rf1 = make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[0], 25, Vec3f(-3, -0.41, -5), 10);
	//rf1->textureInit("../../models/Rock_fountain/texture1.png");
	//shared_ptr<obj::TriangleMesh> rf2 = make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[1], 25, Vec3f(-3, -0.41, -5), 10);
	//rf2->textureInit("../../models/Rock_fountain/texture2.png");
	//scene.addObject(rf1);
	//scene.addObject(rf2);

	/*Loader.LoadFile("../../models/home/home.obj");
	shared_ptr<obj::TriangleMesh> home = make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[0], 30, Vec3f(-2, 1, -5.5), 10);
	home->textureInit("../../models/home/3.png");
	scene.addObject(home);*/


	/*Loader.LoadFile("../../models/car/car.obj");
	for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
	{
		scene.addObject(make_shared<obj::TriangleMesh>(&Loader.LoadedMeshes[i], 5, Vec3f(0, -3, -3), 180));
		
	}	
	
	scene.object_list[1]->material->has_refraction = 1;
	scene.object_list[1]->material->attenuation = Color(2,1.5,0);
	scene.object_list[1]->material->Km = Color(1.0, 1, 1);
	scene.object_list[1]->material->Ks = Color(0.5, 0.3, 0);*/

	//scene.object_list[1]->material->has_refraction = 1;
	//scene.object_list[1]->material->attenuation = Color(2, 1.5, 0);
	/*scene.object_list[2]->material->Kd = Color(1.0, 0.6, 0);
	scene.object_list[2]->material->Ks = Color(0.5, 0.3, 0);*/

	/*obj1->material->has_refraction = 1;
	obj1->material->Km = Color(1, 1, 1);
	obj1->material->Kd = Color(0, 0, 0);
	obj1->material->attenuation = Vec3f(0, 1.4, 1);
	obj1->material->Ks = Color(0.5, 0.5, 0.5);
	obj1->material->p = 200;
	obj1->material->Km = Color(0, 0, 0);
	
	obj1->material->has_refraction = 1;
	obj1->material->Km = Color(1, 1, 1);
	obj1->material->Kd = Color(0, 0, 0);
	obj1->material->attenuation = Vec3f(0, 1.4, 1);
	ground1->material->Ks = Color(0.5, 0.6, 0.9);
	ground1->material->p = 200;
	ground1->material->Km = Color(0,0,0);*/

	
	
	//scene.addLight(make_shared<AreaLight>(Vec3f(0, 10, 10), Vec3f(0.5, 0.5, 0.5), Vec3f(0.25, 0, 0), Vec3f(0, 0, 0.25), 1, 1));
	//scene.addLight(make_shared<AreaLight>(Vec3f(-2, 5, 4), Vec3f(0.5, 0.5, 0.5), Vec3f(0.25, 0, 0), Vec3f(0, 0, 0.25), 1, 1));
	//scene.addLight(make_shared<AreaLight>(Vec3f(-5, 5, 2), Vec3f(0.5, 0.5, 0.5), Vec3f(0.25, 0, 0), Vec3f(0, 0, 0.25), 1, 1));
	
	//scene.addLight(make_shared<AreaLight>(Vec3f(-0.5, 2, 10), Vec3f(1, 1, 1), Vec3f(1, 0, 0), Vec3f(0, 1, 0), 2, 4));
	//scene.addLight(make_shared<AreaLight>(Vec3f(-10, 1, 0), Vec3f(0.5, 0.5, 0.5), Vec3f(0, 0.25, 0), Vec3f(0, 0, 0.5), 2, 4));
	//scene.addLight(make_shared<AreaLight>(Vec3f(10, 1, 0), Vec3f(0.5, 0.5, 0.5), Vec3f(0, 0.25, 0), Vec3f(0, 0, 0.5), 2, 4));
	//scene.addLight(make_shared<AreaLight>(Vec3f(-2, 10, 0), Vec3f(1, 1, 0.7), Vec3f(1, 0, 0), Vec3f(0, 0, 2), 4, 4));
	//scene.addLight(make_shared<AreaLight>(Vec3f(-0.5, 10, -10), Vec3f(1, 1, 1), Vec3f(1, 0, 0), Vec3f(0, 1, 0), 2, 4));
	if (scene.object_list.size() != 0)
	{
		scene.root = kdTreeInit(scene.object_list);
	}

	/*Initiate ray and its origin. The direction changes over diffrent pixels*/
	Ray ray;
	ray.orig = eye;

	/*Initiate the image*/
	vector<Color> img(WIDTH * HEIGHT);
	float step = viewport_width / WIDTH;


	float base_x = -viewport_width / 2 + step / 2;
	float base_y = -viewport_height / 2 + step / 2;
	cout << "Initiation Finished!\nStart Rendering...\n" << endl;

	clock_t start,end,start_global;
	start_global = clock();
	start = clock();

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			/*if (!(i > 150 && i < 300 && j > 300 && j < 500))
			{
				img[getIndex(j, i)] = Color(255,255,255);
				continue;
			}*/
			if (antialiasing)
			{
				Color c{0,0,0};
				float step_devide_4 = step / 4;
				ray.dir = normalize(Vec3f(base_x + step * j - step_devide_4, base_y + step * i - step_devide_4, 0) - ray.orig);
				c += scene.getColor(ray, 1);
				ray.dir = normalize(Vec3f(base_x + step * j + step_devide_4, base_y + step * i - step_devide_4, 0) - ray.orig);
				c += scene.getColor(ray, 1);
				ray.dir = normalize(Vec3f(base_x + step * j - step_devide_4, base_y + step * i + step_devide_4, 0) - ray.orig);
				c += scene.getColor(ray, 1);
				ray.dir = normalize(Vec3f(base_x + step * j + step_devide_4, base_y + step * i + step_devide_4, 0) - ray.orig);
				c += scene.getColor(ray, 1);
				img[getIndex(j, i)] = c / 4;
			}
			else
			{
				/*if (i == 180 && j == 360)
				{
					cout << " ";
				}*/
				ray.dir = normalize(Vec3f(base_x + step * j, base_y + step * i, 0) - ray.orig);
				img[getIndex(j, i)] = scene.getColor(ray, 1);
				/*Color cc = img[getIndex(j, i)] / 255;
				if (norm(cc) < 0.1)
				{
					cout << img[getIndex(j, i)];
				}*/
			}
		}
		//printProcess((float)i / HEIGHT * 100);
		printf("\rProcess: %.2f %%", (float)i / (HEIGHT - 1) * 100);
		end = clock();
		int delta = difftime(end, start)*(HEIGHT - 1 - i)/1000;
		cout << "      [Time remaining: " << delta / 60 << "min " << delta % 60 << "s]     ";
		start = clock();
	}

	int delta = difftime(clock(), start_global) / 1000;
	printf("\n\nRender Done!\n");
	printf("Time taken: %dmin %ds", delta / 60, delta % 60);

	/*Show the generated image and write it into a file*/
	Mat result(HEIGHT, WIDTH, CV_32FC3,img.data()); // Transform data according to each rows
	result.convertTo(result, CV_8UC3, 1.0f);
	cv::cvtColor(result, result, COLOR_RGB2BGR);

	cv::namedWindow("Ray Tracer",WINDOW_AUTOSIZE);
	char key = 0;
	cv::imshow("Ray Tracer", result);
	while ((char)waitKey() != 27) // Press 'Esc' to quit
	{
		
	}
	
	imwrite("./Results/result.jpg", result);
	//getchar();

	return 0;
}
