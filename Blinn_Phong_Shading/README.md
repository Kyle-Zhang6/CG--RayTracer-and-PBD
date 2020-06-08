## Assignment-01

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

  Except for all the options '-ka', '-kd', '-ks', '-sp', '-spu', '-spv', '-pl' and '-dl', I added two more to help specify parameters of the sphere we are rendering:

  __'-ssm mt'__: Set Sphere Material. Options for 'mt' are 'isotropic' and 'anisotropic'. With  				   'anisotropic' as default;

  __'-ssc r g b'__: Set Sphere Color. 'r', 'g' and 'b' are between 0 and 1 inclusive.

  

- #### Recommended parameters :

  __'-pl'__: 

  Position and intensity of the point light are required. 

  Since the sphere in the center of the scene is already big enough, with radius being several hundreds, the position (x,y,z) should be far enough away from the center of the sphere. e.g. (400,1000,0);

  Because this is a 'Blinn-Phong Refelection Model', distance between sphere and a point light would be taken into consideration. The farther, the weaker for the light arriving the surface. So each channel for intensity are recommended to be about 1000. e.g. (1000,1490,2370)



- #### Results:

  1. ./Assignment1 -ssm isotropic -ssc 0.1 0.1 0.1 -ka 0.2 0.2 0.2 -kd 0.7 0.7 0.7 -ks 0.7 0.7 0.7 -sp 20 -pl 400 1000 0 1000 1490 2370 -dl -1 0 -1 1 0.870 0.678
  
     ![iso](Results\isotropic.png)
  
     
  
  2. ./Assignment1 -ssm anisotropic -ssc 0.1 0.1 0.1 -ka 0.2 0.2 0.2 -kd 0.7 0.7 0.7 -ks 0.7 0.7 0.7 -sp 20 -spu 10 -spv 250 -pl 400 1000 0 1000 1490 2370 -dl 1 0 -1 1 1 1
  
     ![aniso](Results\anisotropic.png)
  
     We can also press direction keys to move the sphere, which could be rendered instantaneously.
  
     ![aniso_moved](Results\isotropic_moved.png)