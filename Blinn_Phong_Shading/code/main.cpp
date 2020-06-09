//****************************************************
// Starter code for assignment #1.  It is provided to 
// help get you started, but you are not obligated to
// use this starter code.
//****************************************************

#include <vector>
#include <iostream>
#include <cmath>
#include <string>

//include header file for glfw library so that we can use OpenGL
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include "getopt.h"
#include "my_vector.h"
#include "Light.h"
#include "Sphere.h"

#define PI 3.14159265 // Should be used from mathlib

using namespace std;



//****************************************************
// Global Variables
// Generally speaking, global variables should be 
// avoided, but for this small assignment we'll make
// and exception.
//****************************************************

Vector3f Translation = {0.0f, 0.0f, 0.0f};
bool Auto_strech = false;
int  Width_global = 800;
int  Height_global = 600;

int  SizeX_saved_global;
int  SizeY_saved_global;

int  PosX_saved_global;
int  PosY_saved_global;

const GLFWvidmode * VideoMode_global = NULL;


Vector3f ka(0.05f);
Vector3f Ia(1.0f);
Vector3f kd(0.5f);
Vector3f ks(0.5f);
float puv = 20, pu = 10, pv = 500;

vector<Light> lights = {};
Sphere mySphere(
	Vector3f(Width_global / 2,Height_global / 2,0 ), 
	min(Width_global, Height_global) * 0.8 / 2.0, 
	Vector3f(0.5f,0.5f,0.5f),
	ANISOTROPIC);
Vector3f eyePos(Width_global / 2, Height_global / 2, mySphere.radius*5);

int plNum = 0, dlNum = 0;// Number of point lights and directional lights

/* Initialize GLFW */
void initializeRendering()
{
	if (!glfwInit())
	{
		printf("GLFW Initialization Failed!");
		glfwTerminate();
	}
}

/* Function that is called when window is resized */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//glfwGetFramebufferSize(window, &GLOBAL_WIDTH, &GLOBAL_HEIGHT);
	Width_global = width;
	Height_global = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, Width_global, 0, Height_global, 1, -1);
	mySphere.center = Vector3f(Width_global / 2, Height_global / 2, 0);
	mySphere.radius = min(Width_global, Height_global) * 0.8 / 2.0;
	eyePos = Vector3f(Width_global / 2, Height_global / 2, mySphere.radius * 5);
}

/* Keyboard inputs */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
                    
        // Hint on making up/down left/right work: the variable Translation [0] and [1].
                
        case GLFW_KEY_ESCAPE: 
        case GLFW_KEY_Q:      
            glfwSetWindowShouldClose(window, GLFW_TRUE); 
            break;
        case GLFW_KEY_LEFT :
			mySphere.center.x -= 2;
            break;
        case GLFW_KEY_RIGHT:
			mySphere.center.x += 2;
            break;
        case GLFW_KEY_UP   :
			mySphere.center.y += 2;
            break;
        case GLFW_KEY_DOWN :
			mySphere.center.y -= 2;
            break;
        case GLFW_KEY_F:
            if (action) {
                Auto_strech = !Auto_strech;                 
                if (Auto_strech){
                    glfwGetWindowPos(window, &PosX_saved_global, &PosY_saved_global);
                    glfwGetWindowSize(window, &SizeX_saved_global, &SizeY_saved_global);
                    glfwSetWindowSize(window, VideoMode_global->width, VideoMode_global->height);
                    glfwSetWindowPos(window, 0, 0);
                }else{
                    glfwSetWindowSize(window, SizeX_saved_global, SizeY_saved_global);
                    glfwSetWindowPos(window, PosX_saved_global, PosY_saved_global);
                }
            }
            break;
        case GLFW_KEY_SPACE: 
            break;  
        default: 
            break;
    }
    
}


/* A routine to set a pixel by drawing a GL point.  This is not a
   general purpose routine as it assumes a lot of stuff specific to
   this example. */
void setPixel(float x, float y, GLfloat r, GLfloat g, GLfloat b) {
	glColor3f(r, g, b);
	glVertex2f(x + 0.5, y + 0.5);
	// The 0.5 is to target pixel centers
	// Note that some OpenGL implementations have created gaps in the past.
}

/* This function calculate the color for one pixle According to the result 
   of its point position, normal vector 'n' as well as direction 'u' and 'v'
   ('u' and 'v' are applyed  only when the sphere is anisotropic), which is 
   based on Blinn-Phong Reflectance Model. */
Vector3f getColor(Vector3f p, Vector3f n, Vector3f u, Vector3f v)
{
	// This is for anisotropic sphere model
	Vector3f La = ka * Ia;				// Ambient Light
	Vector3f Ld(0);
	Vector3f Ls(0),Lsu(0),Lsv(0);
	for (auto& light : lights)
	{
		Vector3f l, lu, lv, e;
		// vector 'l' and 'e' represents the direction of light and eye respectively
		if (light.type == POINT) { l = normalize(light.pos - p); }
		else if (light.type == DIRECTIONAL) { l = -normalize(light.pos); }

		Ld += kd * (light.intensity)*max(0.0f, n.dot(l)); // Diffusion light

		e = normalize(eyePos - p);
		Vector3f h = normalize(e + l); // Half vector
		// hu is h's projection to u and n's coplane
		// hv is h's projection to v and n's coplane
		// They are responsible for calculating the specular reflectionrespectively on direction u and v
		Vector3f hu = normalize(projection(h, u) + projection(h, n));
		Vector3f hv = normalize(projection(h, v) + projection(h, n));
		Lsu = ks * (light.intensity)*pow(max(0.0f, n.dot(hu)), pu);
		Lsv = ks * (light.intensity)*pow(max(0.0f, n.dot(hv)), pv);
		Ls += clamp(Lsu*Lsv); // Multiply specular reflection on both direction to get the final specular term
		
	}
	/* At first I thought I need to add an extra prameter to stand for the color ov the sphere. However I notice
	   that 'kd', 'ka', 'ks' are exactly what represent the reflectance, so there's no need to use this. */
	// 'reflection' represents the color of the sphere, which can influence the result of reflection light
	//Vector3f reflection = mySphere.color.norm2() <= 1e-6 ? Vector3f(1.0f, 1.0f, 1.0f) : normalize(mySphere.color);
	//return reflection*(La + Ld) + Ls ;
	return La + Ld + Ls;
}

Vector3f getColor(Vector3f p, Vector3f n)
{
	// This is for shading a isotropic sphere
	Vector3f La = ka*Ia;
	Vector3f Ld(0);
	Vector3f Ls(0);
	for (auto& light : lights)
	{
		Vector3f l, e;
		if (light.type == POINT) { l = normalize(light.pos - p); }
		else if (light.type == DIRECTIONAL) { l = -normalize(light.pos); }
		
		Ld += kd * (light.intensity)*max(0.0f, n.dot(l));

		e = normalize(eyePos - p);
		Vector3f r = -l + 2*(l.dot(n))*n;
		Ls += ks * (light.intensity)*pow(max(0.0f, e.dot(r)),puv);
		
	}
	//Vector3f reflection = (mySphere.color.norm2() <= 1e-4) ? Vector3f(1.0f, 1.0f, 1.0f) : normalize(mySphere.color);

	return La + Ld + Ls;
	//return Ld;
}

/* Draw a filled circle. */
void drawCircle(float centerX, float centerY, float radius) {

    // Start drawing a list of points
    glBegin(GL_POINTS);

    // We could eliminate wasted work by only looping over the pixels
    // inside the sphere's radius.  But the example is more clear this
    // way.  In general drawing an object by loopig over the whole
    // screen is wasteful.

    int minI = max(0,(int)floor(centerX-radius));
    int maxI = min(Width_global-1,(int)ceil(centerX+radius));

    int minJ = max(0,(int)floor(centerY-radius));
    int maxJ = min(Height_global-1,(int)ceil(centerY+radius));

    for (int i = minI; i < maxI; i++) {
        for (int j = minJ; j < maxJ; j++) {

            // Location of the center of pixel relative to center of sphere
            float x = (i+0.5-centerX);
            float y = (j+0.5-centerY);

            float dist = sqrt(x*x + y*y);

            if (dist <= radius) {

                // This is the front-facing Z coordinate
                float z = sqrt(radius*radius-dist*dist);
				Vector3f p(i + 0.5, j + 0.5, z);
				Vector3f n(x, y, z);
				Vector3f pcolor;
				if (mySphere.type == ISOTROPIC)
				{
					n.normalize();
					pcolor = getColor(p, n);
				}
				else
				{
					Vector3f v = Vector3f(0, radius*radius / y, 0) - n;
					Vector3f u = v.cross(n);
					u.normalize();
					v.normalize();
					n.normalize();
					pcolor = getColor(p, n, u, v);
				}				
				pcolor = clamp(pcolor);
				setPixel(i, j, pcolor.x, pcolor.y, pcolor.z);

                // This is amusing, but it assumes negative color values are treated reasonably.
                // setPixel(i,j, x/radius, y/radius, z/radius );
                
            }
        }
    }

    glEnd();
}

/* Function that does the actual drawing of stuff */
void display()
{
    glClearColor( 0.05f,0.05f,0.1f, 0.0f );      //clear background screen to black
    
    glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)
    
    glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
    glLoadIdentity();                            // make sure transformation is "zero'd"
    
    //----------------------- code to draw objects --------------------------
    glPushMatrix();
    glTranslatef (Translation.x, Translation.y, Translation.z);
	drawCircle(mySphere.center.x, mySphere.center.y, mySphere.radius);  // What do you think this is doing?
    glPopMatrix();
    
    //glfwSwapBuffers(window);
    
}



//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) 
{
	int opt;
	int digit_optind = 0;
	int option_index = 0;
	char *string = (char*)"m:";
	static struct option long_options[] =
	{
		{(char*)"ka", required_argument,NULL, 'a'},
		{(char*)"kd", required_argument,NULL, 'd'},
		{(char*)"ks", required_argument,NULL, 's'},
		{(char*)"sp", required_argument,NULL, 'p'},
		{(char*)"spu", required_argument,NULL, 'u'},
		{(char*)"spv", required_argument,NULL, 'v'},
		{(char*)"pl", required_argument,NULL, 'x'},
		{(char*)"dl", required_argument,NULL, 'y'},
		{(char*)"ssm", required_argument,NULL, 'm'},
		{(char*)"ssc", required_argument,NULL, 'c'},
		{NULL,     0,                      NULL, 0}
	};
	while ((opt = getopt_long_only(argc, argv, string, long_options, &option_index)) != -1)
	{
		//if (opt >= '10') continue;
		switch (opt)
		{
		case 'a':
			ka = Vector3f(atof(optarg),atof(argv[optind]) ,atof(argv[optind + 1]));
			printf("ka = (%.3f,%.3f,%.3f)\n\n", ka.x, ka.y, ka.z);
			break;

		case 'd':
			kd = Vector3f(atof(optarg),atof(argv[optind]) ,atof(argv[optind + 1]));
			printf("kd = (%.3f,%.3f,%.3f)\n\n", kd.x, kd.y, kd.z);
			break;

		case 's':
			ks = Vector3f(atof(optarg),atof(argv[optind]) ,atof(argv[optind + 1]));
			printf("ks = (%.3f,%.3f,%.3f)\n\n", ks.x, ks.y, ks.z);
			break;

		case 'p':
			puv = atof(optarg);
			printf("Set specular exponent: p = %.3f\n\n", puv);
			break;

		case 'u':
			pu = atof(optarg);
			printf("Set specular exponent in u direction: pu = %.3f\n\n", pu);
			break;

		case 'v':
			pv = atof(optarg);
			printf("Set specular exponent in v direction: pv = %.3f\n\n", pv);
			break;

		case 'x':
			if (plNum >= 5)
			{
				printf("*ERROR: Fail to add a point light --- Current point lights number: 5\n");
			}
			else
			{
				plNum++;
				lights.push_back(Light(POINT, Vector3f(atof(optarg),atof(argv[optind]) ,atof(argv[optind + 1])),
					Vector3f(atof(argv[optind + 2]),atof(argv[optind + 3]) ,atof(argv[optind + 4]))));
				printf("Added point light%d successfully:\n", plNum);
				printf("\tPosition: \t(%s,%s,%s)\n", optarg, argv[optind], argv[optind + 1]);
				printf("\tIntensity: \t(%s,%s,%s)\n\n", argv[optind + 2], argv[optind + 3], argv[optind + 4]);
			}
			break;

		case 'y':
			if (dlNum >= 5)
			{
				printf("*ERROR: Fail to add a directional light --- Current directional lights number: 5\n");
			}
			else
			{
				dlNum++;
				lights.push_back(Light(DIRECTIONAL, Vector3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1])),
					Vector3f(atof(argv[optind + 2]), atof(argv[optind + 3]), atof(argv[optind + 4]))));
				printf("Added directional light%d successfully:\n", plNum);
				printf("\tDirection: \t(%s,%s,%s)\n", optarg, argv[optind], argv[optind + 1]);
				printf("\tColor: \t\t(%s,%s,%s)\n\n", argv[optind + 2], argv[optind + 3], argv[optind + 4]);
			}
			break;

		case 'm':
			if (strcmp(optarg, "isotropic") == 0) 
			{ 
				mySphere.type = ISOTROPIC;
				printf("Set sphere material as ISOTROPIC.\n\n");
			}
			else if (strcmp(optarg, "anisotropic") == 0) 
			{ 
				mySphere.type = ANISOTROPIC; 
				printf("Set sphere material as ANISOTROPIC.\n\n");
			}
			else { printf("*ERROR: Fault material for sphere --- Choice:{isotropic,anisotropic}\n\n"); }
			break;

		case 'c':
			mySphere.color = Vector3f(atof(optarg), atof(argv[optind]), atof(argv[optind + 1]));
			printf("Set sphere color as (%.3f,%.3f,%.3f)\n\n", mySphere.color.x, mySphere.color.y, mySphere.color.z);
			break;

		default: printf("*ERROR: Unrecognized option.\n");
			break;
		}
	}
	
    //This initializes glfw
    initializeRendering();
    
    GLFWwindow* window = glfwCreateWindow( Width_global, Height_global, "Computer Graphics", NULL, NULL );
    if ( !window )
    {
        cerr << "Error on window creating" << endl;
        glfwTerminate();
        return -1;
    }
    
    VideoMode_global = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if ( !VideoMode_global )
    {
        cerr << "Error on getting monitor" << endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent( window );

    framebuffer_size_callback(window, Width_global, Height_global);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
                
	// These were used to test code when debugging
	//lights.push_back(Light(POINT, { (float)Width_global / 2,1000,0 }, { 1,1.49,2.37 }));
	//lights.push_back(Light(DIRECTIONAL, { 1,0,-1 }, { 1,1,1 }));
	//lights.push_back(Light(DIRECTIONAL, { -1,0,-1 }, { 1,1,1 }));
	//lights.push_back(Light(DIRECTIONAL, { -100,100,-100 }, { 1,1,1 }));

    while( !glfwWindowShouldClose( window ) ) // main loop to draw object again and again
    {   
		display();
		glfwSwapBuffers(window);
        glfwPollEvents();        
    }

    return 0;
}





