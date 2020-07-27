## Ray Tracer

- #### Overview:

  This project achieves a simple ray tracer with material that has reflection, refraction and texture mapping. Also, to make the result much better, this project achieves soft shadowing as well as antialiasing.

  Kai Zhang and Yihao Wang finished this ray tracer together. 

  Yihao Wang is responsible for the class __'Sphere'__, including the initiation of a sphere and getting the intersection between it and a ray; He also finished class __'Light'__, which help perform soft shadowing. 

  Kai Zhang wrote 'Triangle' as well as 'TriangleMesh' which could save information of mesh objects. And he performed texture mapping. To accelerate the whole process, he used AABB  and KD Tree as the data structure to save the information of objects.

  The whole project is based on __OPENCV__, including image reading and writing, and some vector/matrix operation.

  

- #### Results:

  __Some results with their according Command Line Options are given below...__

  1. ./Assignment2 -sph 3 1 -8 2 -kd 0.9 0.6 0 -ks 0.9 0.6 0 -km 0.45 0.3 0 -add -sph -3.2 1 -8 2 -kd 0.5 0.4 0.9 -ks 0.25 0.2 0.45 -km 0.1 0.08 0.18 -add -sph 1.5 -0.5 -0.5 0.5 -km 1 1 1 -refract -att 1 0.1 0.5 -add -sph -1 -0.7 -0.5 0.3 -km 1 1 1 -refract -att 0 2.4 2.8 -add -sph -0.5 2 -2 1 -ks 0.1 0.1 0.1 -kd 0 0 0 -km 1 1 1 -refract -add -sph 0 -10000 -1 9999 -km 0.1 0.1 0.1 -add -l 10 10 -2 1 0 0 0 0 1 1 1 -lc 1 1 1

     <img src="Results\spheres.jpg" alt="" style="zoom:100%;" />

     

  2. ./Assignment2 -sph 3 1 -8 2 -kd 0.9 0.6 0 -ks 0.9 0.6 0 -km 0.45 0.3 0 -add -sph -3.2 1 -8 2 -kd 0.5 0.4 0.9 -ks 0.25 0.2 0.45 -km 0.1 0.08 0.18 -add -sph 1.5 -0.5 -0.5 0.5 -km 1 1 1 -refract -att 1 0.1 0.5 -add -sph -1 -0.7 -0.5 0.3 -km 1 1 1 -refract -att 0 2.4 2.8 -add -sph -0.5 2 -2 1 -ks 0.1 0.1 0.1 -kd 0 0 0 -km 1 1 1 -refract -add -sph 0 -10000 -1 9999 -km 0.1 0.1 0.1 -add -l 10 10 -2 1 0 0 0 0 1 4 8 -lc 1 1 1

     __This image apples area lights to achieve soft shadowing.____

     <img src="Results\spheres_softshadow.jpg" alt="soft" style="zoom:100%;" />

     
     
  3. ./Assignment2 -sph 3 1 -8 2 -kd 0.9 0.6 0 -ks 0.9 0.6 0 -km 0.45 0.3 0 -add -sph -3.2 1 -8 2 -kd 0.5 0.4 0.9 -ks 0.25 0.2 0.45 -km 0.1 0.08 0.18 -add -sph 1.5 -0.5 -0.5 0.5 -km 1 1 1 -refract -att 1 0.1 0.5 -add -sph -1 -0.7 -0.5 0.3 -km 1 1 1 -refract -att 0 2.4 2.8 -add -sph -0.5 2 -2 1 -ks 0.1 0.1 0.1 -kd 0 0 0 -km 1 1 1 -refract -add -sph 0 -10000 -1 9999 -km 0.1 0.1 0.1 -add -l 10 10 -2 1 0 0 0 0 1 4 8 -lc 1 1 1 -anti
     
     __Apply antialiasing based on the second result shown above.__
     
     <img src="Results\spheres_softshadow_antialiasing.jpg" alt="anti" style="zoom:100%;" />
     
  4. ./Assignment2 -mdl models/panda/panda.obj -rev 10 -1 -0.5 -1.2 -20 -tex models/panda/panda.png -add -mdl models/grassland/grassland.obj -rev 30 -2 -1.5 -3 0 -tex models/grassland/grassland.png -add -mdl models/grassland/grassland.obj -rev 30 2 -1.5 -3 0 -tex models/grassland/grassland.png -add -mdl models/bee/bee.obj -rev 1 2.25 -0.9 0 -170 -tex models/bee/bee.png -add -mdl models/flower/Purple_flower.obj -rev 2 1.75 -1.25 1.5 0 -tex models/flower/texture.png -add -mdl models/topiary/topiary.obj -rev 5 4 -1.2 -1 0 -tex models/topiary/3.png -add -mdl models/topiary/topiary.obj -rev 5 4 -1.2 0 0 -tex models/topiary/3.png -add -mdl models/topiary/topiary.obj -rev 5 4 -1.2 1 0 -tex models/topiary/3.png -add -mdl models/topiary/topiary.obj -rev 5 4 -1.2 -2 0 -tex models/topiary/3.png -add -mdl models/topiary/topiary.obj -rev 5 -4 -1.2 -1 0 -tex models/topiary/3.png -add -mdl models/topiary/topiary.obj -rev 5 -4 -1.2 0 0 -tex models/topiary/3.png -add -mdl models/topiary/topiary.obj -rev 5 -4 -1.2 1 0 -tex models/topiary/3.png -add -mdl models/topiary/topiary.obj -rev 5 -4 -1.2 -2 0 -tex models/topiary/3.png -add -l -2 10 0 1 0 0 0 0 2 2 2 -lc 0.7 0.7 0.7 -l -0.5 5 10 1 0 0 0 1 0 2 4 -lc 0.9 0.9 0.9 -l -0.5 5 -10 1 0 0 0 1 0 2 2 -lc 0.6 0.6 0.5 -l 10 1 0 0 1 0 0 0 1 2 2 -lc 0.5 0.5 0.3 -l -2 10 0 1 0 0 0 0 1 2 2 -lc 0.5 0.5 0.3

     __Import some '.obj' files with their textures to form this scene.____

     <img src="Results\panda.jpg" alt="panda" style="zoom:100%;" />

     __It can also apply antialiasing...__

     <img src="Results\panda_antialiasing.jpg" alt="panda_anti" style="zoom:100%;" />

     

- #### Codes:

  All of the codes are saved in ./code.

  __'main.cpp'__: Initialize the scene;

  __'Scene.h'__: Define a class 'Scene' which includes object list, light list and some other essential members;

  __'Object.h'__: Including class 'Object', and 'Sphere' & 'Triangle' which are inherited from 'Object';

  __'TriangleMesh.h'__: Define class 'TriangleMesh' which is used to store information of mesh objects;

  __'Light.h'__: Define a class 'Light' to manage point lights in the main scene; It also includes class 'AreaLight' which could help achieve soft shadowing;

  __'Ray.h'__: Define a class 'Ray' to manage the rays;

  __'Material.h'__: Define class 'Material' which includes some members like 'Kd', 'Ks', 'Ka', etc. To describe the material of an object;

  __'AABB.h' & 'BVH.h'__: To achieve the acceleration for this ray tracer;

  __'getopt.c/getopt.h'__: Downloaded from the internet;	

  ​									 Contains function 'getopt_long_only()' to analyse command lines.

  

- #### Command lines:

  __'-bc r g b'__ 

  Background color.  The parameters r g b are numbers between 0 and 1 inclusive. (All of the color mentioned below are the same.)

  

  __'-depth n'__ 

  Since reflection and refraction could be unlimited, we define a depth to stop the recursive process in getting color based on the reflection and refraction rays.

  

  __'-mdl <url>'__

  Import a model.

  

  __'-sph x y z r'__

  Initiate a sphere, with 'x','y' and 'z' being its position coordinate respectively, and 'r' as its radius.

  

  __'-rev s x y z a'__

  To perform scale, translation and rotation.

  's': scale coefficient;

  'x'&'y'&'z': translation coefficient;

  'a': rotation angle, in degree.

  

  __'-tex <url>'__

  Specify the texture for a mesh object.

  

  __'-kd r g b'__

  This is the diffusion color coefficients of the material. The parameters r g b are numbers between 0 and 1 inclusive. 

  

  __'-ks r g b'__

  This is the specular color coefficients of the material. The parameters r g b are numbers between 0 and 1 inclusive. 

  

  __'-km r g b'__ 

  This is the mirror color coefficients of the material. The parameters r g b are numbers between 0 and 1 inclusive. 

  

  __'-refract'__ 

  No argument required. To specify that the object has refraction.

  

  __'-att r g b'__ 

  This is the attenuation color coefficients of the material, which only work when the object has refraction. The parameters r g b are numbers between 0 and 1 inclusive.

  

  __'-add'__

  No argument required. To add the initiated object into the scene's object list.

  

  __'-l x y z  d1.x  d1.y  d1.z  d2.x  d2.y  d2.z  d1_num  d2_num'__ 

  This adds an area light, which consists several point lights, to the scene. 'x'&'y'&'z' define the first point light of the area light; 'd1'&'d2' are the extending direction; 'd1_num'&'d2_num' define the number of point light on each direction.  

  

  __'-lc r g b'__ 

  Change the color of the most recent added light.

  

  __'-anti'__

  No argument required. When specifying this, antialiasing will be applied.
