## Data Structure for Actuator

#### Using Position-Based Dynamics

2020-07-26

### 1 Overall

​	Since the face list could be disordered after being subdivided while the construction of constraints for Position Based Dynamic requires ordered face list, especially stretch constraint, the first part of Actuator is sort the face list. Then to achieve the animation, the second step is performing the Verlet Integration. Lastly, iterate for a couple of times according to the constraints constructed before till the result converges.

​	All of the codes for this part conform this logic. The data structure might be a little different from the other parts and something new is also added as our group worked separately at first. But this does not matter because we will combine our codes together later. So currently, I will merely introduce things about this part.

### 2 Results

​	Currently I use some other methods rather than rendering to test the code. The strategy is initiating data for cloth with some of its points fixed, and let other points free fall. For example, in this case I fix the four corners of a 15x10 cloth and print the y coordinate on the screen to see if the result goes well. Here is the result:

![](Result\result.png)




### 3 class 'Cloth'

```c++
class Cloth : public Mesh
{
public:
	float cw;						// Wind reaction Coefficient
	std::vector<ElementMap*> map;	// Ordered map towards initial faces
    // List of Stretch Constraint
	std::vector<StretchContraint*> s_constraint;	

	Cloth(float c):cw(c) {};
	~Cloth() {};
	void init();
	void verletIntegrate(float dt);
	void iteration(const int num_iteration, const float err_converge);
	void updateAcc(Scene scene, float t);
	void updateVelocity(float dt);
};
```

​	Basically, cloth is still mesh. Thus this class is inherited from class 'Mesh', with some extinct members added.	

1. __cw: coefficient of wind responding __

2. __map: to save the data after sorting the result got from the Subdivider__

   ```c++
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
   ```

   And also, since the Verlet Integration required not only the position and other states of a particle for the current moment, but also the past one, it saves both current state like position, velocity, etc.

3. __s_constraint: stretch constraint__

   ```c++
   struct StretchContraint
   {
   	int index1;
   	int index2;
   	float rest_length;
   };
   ```

   'index1' and 'index2' are used to describe the two particles that shares stretch constraint. And 'rest_length' is for their initial gap.

   

### 3 Functions for 'Cloth'

```c++
	Cloth(float c):cw(c) {};
	~Cloth() {};
	void init();
	void verletIntegrate(float dt);
	void iteration(const int num_iteration, const float err_converge);
	void updateAcc(Scene scene, float t);
	void updateVelocity(float dt);
```

1. __init(): to initiate the cloth.__

2. __verletIntegration(float dt)：performing Verlet Integration__

   'dt', in second, is the time interval between two frames.

3. __iteration(...)__

   num_iteration: the limitation of the greatest time for iteration；

   err_converge: after one time of iteration, the position of the particles would have changed. When the difference is not really huge, we can regard it as a convergence, which means the process of iteration can stop. And 'err_converge' determines when to regard it as convergence.

4. __updateAcc(...)__

5. __updateVelocity(float dt)__




### 4 steps for animation

```c++
cloth.verletIntegrate(dt);	
cloth.iteration(10000,0.001*cloth.s_constraint.size());
// gotta update velocity first 必须先更新速度
cloth.updateVelocity(dt);
cloth.updateAcc(scene, dt*a);
```



### 5 How to describe wind

```c++
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
```

