## Blinn-Phong Shading

- #### Overview:

  In this project, I apply OpenGL to initialize a rendering window to show a rendered sphere with Blinn-Phong Reflection Model. 

  There's one special effect for anisotropic material, which can have different  specular-term coefficients on direction __u__ and __v__ respectively. The results would be shown below.

  

- #### Results:

  __Some results with their according Command Line Options are given below...__

  1. ./Assignment1 -ssm isotropic -ssc 0.1 0.1 0.1 -ka 0.1 0.1 0.1 -kd 0.5 0.5 0.5 -ks 0.5 0.5 0.5 -sp 10 -pl 400 1000 0 0.5 0.745 1.185 -dl -1 0 -1 1 0.870 0.678

     <img src="Results\isotropic.png" alt="iso" style="zoom:100%;" />

     

  2. ./Assignment1 -ssm anisotropic -ssc 0.1 0.1 0.1 -ka 0.1 0.1 0.1 -kd 0.6 0.6 0.6 -ks 0.7 0.7 0.7 -sp 20 -spu 10 -spv 300 -pl 400 1000 0 0.5 0.745 1.185 -dl 1 0 -1 0.8 0.8 0.8

     <img src="Results\anisotropic.png" alt="iso" style="zoom:100%;" />

     We can also press direction keys to move the sphere, which could be re-rendered instantaneously.

     <img src="Results\anisotropic_moved.png" alt="iso" style="zoom:100%;" />

     

  3. ./Assignment1 -ssm isotropic -ssc 0.1 0.1 0.1 -ka 0.086 0.01 0.03 -kd 0.57 0.08 0.2 -ks 0.5 0.5 0.5 -sp 10 -pl 400 1000 0 1 1 1 -dl -1 0 -1 0.8 0.8 0.8

     <img src="Results\plastic_effect.png" alt="plastic" style="zoom:100%;" />

  4. ./Assignment1 -ssm anisotropic -ssc 0.1 0.1 0.1 -ka 0.086 0.01 0.03 -kd 0.57 0.08 0.2 -ks 0.7 0.08 0.25 -spu 10 -spv 200 -pl 400 1000 0 1 1 1 -dl -1 0 -1 0.8 0.8 0.8

     <img src="Results\metal_effect.png" alt="metal" style="zoom:100%;" />

- #### Codes:

  All of the codes are saved in ./code.

  __'main.cpp'__: Initialize GLFW window and perform Blinn-Phong Reflection Model on a sphere;

  __'my_vector.h'__: Define a class 'Vector3f' to manage 3-dimension vectors and their 						   operations;

  __'Light.h'__: Define a class 'Light' to manage lights in the main scene;

  ​				 Each 'Light' object is either a 'POINT' or a 'DIRECTIONAL' one, with its position and 				 intensity (or direction and color if it is a directional light);

  __'Sphere.h'__: Define a class 'Sphere' to manage the sphere object;

  ​				     Material (isotropic or anisotropic), center ([x,y,z]) and radius are required to 					 specify one sphere;

  __'getopt.c/getopt.h'__: Downloaded from the internet;	

  ​									 Contains function 'getopt_long_only()' to analyse command lines.

  

- #### Command lines:

  __'-ka r g b'__ 

  This is the ambient color coefficients of the sphere material. The parameters r g b are numbers between 0 and 1 inclusive. 

  

  __'-kd r g b'__ 

  This is the diffuse color coefficients of the sphere material. The parameters r g b are numbers between 0 and 1 inclusive. 

  

  __'-ks r g b'__

  This is the specular color coefficients of the sphere material. The parameters r g b are numbers between 0 and 1 inclusive. 

  

  __'-spu pu'__ 

  This is the power coefficient on the specular term in the u direction for an anisotropic material.  It is a number between 0 and max_float.

  

  __'-spv pv'__ 

  This is the power coefficient on the specular term in the v direction for an anisotropic material. 

  It is a number between 0 and max_float.

  

  __' -sp p'__ 

  This is the power coefficient on the specular term for an isotropic material. It is a number between 0 and max_float. (*i.e.* the same as setting *pu* and *pv* the the same value.)

  

  __'-pl x y z r g b'__

  This adds a point light to the scene. The x y z values are the location of the light. The r g b values are it's color. Note that the x y z values are relative to the sphere. That is, the center of the sphere is at the origin and the radius of the sphere defines one unit of length. The Y direction is UP, the X direction is to the right on the screen, and the Z direction is "in your face." The r g b value are between 0 and max_float, NOT between 0 and 1 (that is, the r g b values encode the brightness of the light). 

  

  __'-dl x y z r g b'__ 

  This adds a directional light to the scene. The x y z values are the direction that the light points in. The r g b values are it's color. See -pl for coordinate system notes. 

  

  __Except for all the options '-ka', '-kd', '-ks', '-sp', '-spu', '-spv', '-pl' and '-dl', I added two more to help specify parameters of the sphere we are rendering:__

  __'-ssm mt'__: Set Sphere Material. Options for 'mt' are 'isotropic' and 'anisotropic'. With  				   'anisotropic' as default;

  __'-ssc r g b'__: Set Sphere Color. 'r', 'g' and 'b' are between 0 and 1 inclusive.

  

- #### Recommended parameters :

  __'-pl'__: 

  Position and intensity of the point light are required. 

  Since the sphere in the center of the scene is already big enough, with radius being several hundreds, the position (x,y,z) should be far enough away from the center of the sphere. e.g. (400,1000,0);

  Because this is a 'Blinn-Phong Refelection Model', distance between sphere and a point light would be taken into consideration. The farther, the weaker for the light arriving the surface. So each channel for intensity are recommended to be about 1000. e.g. (1000,1490,2370)



