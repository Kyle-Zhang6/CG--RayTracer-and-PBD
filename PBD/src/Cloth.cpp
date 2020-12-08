#include "Cloth.h"
#include "global.h"
#include <iostream>

void elementCopy(Element* dest, Element* orig)
{
	vec3_copy(dest->acceleration, orig->acceleration);
	vec3_copy(dest->position, orig->position);
	vec3_copy(dest->velocity, orig->velocity);
	dest->w = orig->w;
	return;
}

void vec3_div(vec3 v, float n)
{
	v[0] = v[0] / n;
	v[1] = v[1] / n;
	v[2] = v[2] / n;
}

void vec3_scale(vec3 v, float n)
{
	v[0] = v[0] * n;
	v[1] = v[1] * n;
	v[2] = v[2] * n;
}

// Get the farrest face centriod from point p, returning its index
int getFarPoint(vec3 p, std::vector<Face*> faces)
{
	vec3 temp;
	float max_dist2 = 0, dist2 = 0;
	int max_index = 0;
	for (int i = 0; i < faces.size(); i++)
	{
		vec3_sub(temp, p, faces[i]->e->position);
		dist2 = (temp[0] * temp[0] + temp[1] * temp[1] + temp[2] * temp[2]);
		if (dist2 > max_dist2)
		{
			max_dist2 = dist2;
			max_index = i;
		}
	}
	return max_index;
}

void Cloth::init()
{
	//---------------------------Map Init------------------------------
	map.resize(faces.size());
	
	vec3 p,corner1;
	vec3_copy(p, (faces[0]->e)->position);
	int corner1_index = getFarPoint(p, faces);
	int corner2_index = getFarPoint(faces[corner1_index]->e->position, faces);
	vec3_copy(corner1, faces[corner1_index]->e->position);

	// Assume that all of the grids are of same length and width
	// get the step between two adjacent points as length vector and width vector
	vec3 l, w;
	vec3_sub(l, vertices[faces[0]->indices[0]]->position, vertices[faces[0]->indices[1]]->position);
	vec3_sub(w, vertices[faces[0]->indices[0]]->position, vertices[faces[0]->indices[3]]->position);
	// Calculate p as the diagnal vector
	vec3_sub(p, faces[corner2_index]->e->position, corner1);
	if (vec3_mul_inner(p, l) < 0)
	{
		l[0] = -l[0];
		l[1] = -l[1];
		l[2] = -l[2];
	}
	if (vec3_mul_inner(p, w) < 0)
	{
		w[0] = -w[0];
		w[1] = -w[1];
		w[2] = -w[2];
	}

	// Calculate numbers of columns and rows
	int column_num, row_num;
	column_num = round(vec3_mul_inner(p, l)) / (l[0] * l[0] + l[1] * l[1] + l[2] * l[2]);
	row_num = round(vec3_mul_inner(p, w)) / (w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);

	if ((column_num + 1)*(row_num + 1) != faces.size())
	{
		std::cerr << "Failure when initiating face map for cloth..." << std::endl;
		exit(-1);
	}

	int l_index, w_index;
	for (int i=0;i<faces.size();i++)
	{
		ElementMap* em = new ElementMap();
		vec3_sub(p, faces[i]->e->position, corner1);
		l_index = round(vec3_mul_inner(p, l)) / (l[0] * l[0] + l[1] * l[1] + l[2] * l[2]);
		w_index = round(vec3_mul_inner(p, w)) / (w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
		int index = l_index + w_index * (column_num + 1);
		em->index = i;
		elementCopy(em->old_e, faces[i]->e);
		em->cur_e = faces[i]->e;
		//elementCopy(em->cur_e, faces[i]->e);
		map[index] = em;
	}

	float l_len = vec3_len(l);
	float w_len = vec3_len(w);
	//-------------------------Contraint Init---------------------------
	for (int j = 0; j < row_num + 1; j++)
	{
		for (int i = 0; i < column_num + 1; i++)
		{
			if (i < column_num)
			{
				StretchContraint* sc = new StretchContraint();
				sc->index1 = i + j * (column_num + 1);
				sc->index2 = sc->index1 + 1;
				sc->rest_length = l_len;
				s_constraint.push_back(sc);
			}
			if (j < row_num)
			{
				StretchContraint* sc = new StretchContraint();
				sc->index1 = i + j * (column_num + 1);
				sc->index2 = sc->index1 + (column_num + 1);
				sc->rest_length = w_len;
				s_constraint.push_back(sc);
			}
		}
	}
	return;
}

// Perform Verlet Integration with an interval 'dt'
void Cloth::verletIntegrate(float dt)
{
	for (int i = 0; i < map.size(); i++)
	{
		if (map[i]->cur_e->w == 0) continue; // If mass is infinity, regard it as a fixed point

		vec3 old_position, cur_position, next_position;
		vec3_copy(old_position, map[i]->old_e->position); // t-dt
		vec3_copy(cur_position, map[i]->cur_e->position); // t
		
		vec3 a;
		vec3_scale(a, map[i]->cur_e->acceleration, dt*dt);

		// next_position = cur_position*2 - old_position + a * dt * dt
		vec3_scale(next_position,cur_position, 2);
		vec3_sub(cur_position, next_position, old_position);
		vec3_add(next_position, cur_position, a);

		vec3_copy(map[i]->old_e->position, map[i]->cur_e->position);
		vec3_copy(map[i]->cur_e->position, next_position);

		//// Update velocity
		//vec3 v;
		//vec3_sub(v, next_position, old_position);
		//vec3_div(v, 2 * dt);
		//vec3_copy(map[i]->old_e->velocity, v);

		//// Predict current velocity roughly with Euler explicit integration
		//vec3 dv;
		//vec3_scale(dv, map[i]->old_e->acceleration, dt);
		//vec3_add(map[i]->cur_e->velocity, v, dv);
	}
	return;
}

void Cloth::updateVelocity(float dt)
{
	for (auto em : map)
	{
		if (em->cur_e->w == 0) continue; // If mass is infinity, regard it as a fixed point

		vec3_sub(em->cur_e->velocity, em->cur_e->position, em->old_e->position);
		vec3_scale(em->cur_e->velocity, 1 / dt);
	}
}

void Cloth::updateAcc(Scene scene, float t)
{
	for (auto em : map)
	{
		if (em->cur_e->w == 0) continue; // If mass is infinity, regard it as a fixed point

		vec3 gravity = { 0 };
		gravity[1] = -9.8;	// Gravity

		// Wind
		vec3 wind;
		scene.getWindVector(wind, t, em->cur_e->position);
		vec3 v_relative;
		vec3_sub(v_relative, em->cur_e->velocity, wind);

		vec3 wind_force = { 0 };
		vec3 force1, force2, force3, force4, face_dir;
		vec3 l0, l1, l2, l3;
		float scale,co;

		vec3_sub(l0, vertices[faces[em->index]->indices[0]]->position, em->cur_e->position);
		vec3_sub(l1, vertices[faces[em->index]->indices[1]]->position, em->cur_e->position);
		vec3_sub(l2, vertices[faces[em->index]->indices[2]]->position, em->cur_e->position);
		vec3_sub(l3, vertices[faces[em->index]->indices[3]]->position, em->cur_e->position);
		co = -0.25*scene.rho*cw*vec3_len(v_relative);

		vec3_mul_cross(face_dir, l0, l1);
		scale = co * vec3_mul_inner(face_dir, v_relative)/vec3_len(face_dir);
		vec3_scale(force1, face_dir, scale);
		
		vec3_mul_cross(face_dir, l1, l2);
		scale = co * vec3_mul_inner(face_dir, v_relative) / vec3_len(face_dir);
		vec3_scale(force2, face_dir, scale);
		vec3_add(force3, force1, force2);

		vec3_mul_cross(face_dir, l2, l3);
		scale = co * vec3_mul_inner(face_dir, v_relative) / vec3_len(face_dir);
		vec3_scale(force1, face_dir, scale);

		vec3_mul_cross(face_dir, l3, l0);
		scale = co * vec3_mul_inner(face_dir, v_relative) / vec3_len(face_dir);
		vec3_scale(force2, face_dir, scale);
		vec3_add(force4, force1, force2);

		vec3_add(wind_force, force3, force4);
		vec3_scale(wind_force, em->cur_e->w);


		// Addition of all of the force
		vec3 acc;
		vec3_add(acc, gravity, wind_force);
		vec3_copy(em->cur_e->acceleration, acc);
	}
	return;
}


void Cloth::iteration(const int num_iteration, const float err_converge)
{
	for (int i = 1; i <= num_iteration; i++)
	{
		float err = 0;
		for (auto sc : s_constraint)
		{
			vec3 old_position1,old_position2;
			vec3_copy(old_position1, map[sc->index1]->cur_e->position);
			vec3_copy(old_position2, map[sc->index2]->cur_e->position);

			vec3 dist,delta1,delta2;
			float w1 = map[sc->index1]->cur_e->w;
			float w2 = map[sc->index2]->cur_e->w;
			vec3_sub(dist, old_position1, old_position2);
			float dist_len = vec3_len(dist);
			float scale = (dist_len - sc->rest_length) / ((w1 + w2)*dist_len);

			vec3_scale(delta1, dist, -w1*scale);
			vec3_scale(delta2, dist, w2*scale);
			vec3_add(map[sc->index1]->cur_e->position, old_position1, delta1);
			vec3_add(map[sc->index2]->cur_e->position, old_position2, delta2);

			err += vec3_len(delta1)+vec3_len(delta2);
		}


		if (err < err_converge)
		{
#ifdef SHOWITERATIONTIME
			printf("Converge after %d iterations.\n", i);
#endif // SHOWITERATIONTIME
			return;
		}
		
		// To be implemented...
		//---------------------------------------
		// Currently only stretch constraint is taken into consideration.
		// Maybe add bending constraint & collide constraint later
		// ...
		// ...
		//---------------------------------------
	}


#ifdef SHOWITERATIONTIME
	printf("Converge after %d iterations.\n", num_iteration);
#endif // SHOWITERATIONTIME

	
	return;
}