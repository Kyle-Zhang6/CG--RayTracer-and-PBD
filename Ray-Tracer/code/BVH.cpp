#include "BVH.h"
#include <algorithm>

BVHNode* kdTreeInit(std::vector<std::shared_ptr<Object>> obj_list)
{
	BVHNode* node = new BVHNode();

	if (obj_list.size() == 1)
	{
		node->obj = obj_list[0].get();
		node->box = obj_list[0]->getAABB();
		node->left = nullptr;
		node->right = nullptr;
	}
	else if (obj_list.size() == 2)
	{
		BVHNode* left_node = new BVHNode();
		BVHNode* right_node = new BVHNode();
		left_node->obj = obj_list[0].get();
		left_node->box = obj_list[0]->getAABB();
		left_node->left = nullptr;
		left_node->right = nullptr;
		right_node->obj = obj_list[1].get();
		right_node->box = obj_list[1]->getAABB();
		right_node->left = nullptr;
		right_node->right = nullptr;

		node->left = left_node;
		node->right = right_node;
		node->box = merge(left_node->box, right_node->box);
		node->obj = nullptr;
	}
	else
	{
		node->box = obj_list[0]->getAABB();
		for (int i = 1; i < obj_list.size(); i++) node->box.merge(obj_list[i]->getAABB());
		int long_edge_index = node->box.longEdge();

		std::sort(obj_list.begin(), obj_list.end(), [&long_edge_index](auto obj1, auto obj2)
		{
			return obj1->getAABB().getCentroid()[long_edge_index] <
				obj2->getAABB().getCentroid()[long_edge_index];
		}
		);

		std::vector<std::shared_ptr<Object>> head(obj_list.begin(), obj_list.begin() + obj_list.size() / 2);
		std::vector<std::shared_ptr<Object>> tail(obj_list.begin() + obj_list.size() / 2, obj_list.end());

		node->left = kdTreeInit(head);
		node->right = kdTreeInit(tail);
		node->box = merge(node->left->box, node->right->box);
		node->obj = nullptr;
	}
	return node;
}

BVHNode* kdTreeInit(std::vector<std::shared_ptr<Triangle>> triangles)
{
	BVHNode* node = new BVHNode();

	if (triangles.size() == 1)
	{
		node->obj = triangles[0].get();
		node->box = triangles[0]->getAABB();
		node->left = nullptr;
		node->right = nullptr;
	}
	else if (triangles.size() == 2)
	{
		BVHNode* left_node = new BVHNode();
		BVHNode* right_node = new BVHNode();
		left_node->obj = triangles[0].get();
		left_node->box = triangles[0]->getAABB();
		left_node->left = nullptr;
		left_node->right = nullptr;
		right_node->obj = triangles[1].get();
		right_node->box = triangles[1]->getAABB();
		right_node->left = nullptr;
		right_node->right = nullptr;

		node->left = left_node;
		node->right = right_node;
		node->box = merge(left_node->box, right_node->box);
		node->obj = nullptr;
	}
	else
	{
		node->box = triangles[0]->getAABB();
		for (int i = 1; i < triangles.size(); i++) node->box.merge(triangles[i]->getAABB());
		int long_edge_index = node->box.longEdge();

		std::sort(triangles.begin(), triangles.end(), [long_edge_index](auto obj1, auto obj2)
		{
			return obj1->getAABB().getCentroid()[long_edge_index] <
				obj2->getAABB().getCentroid()[long_edge_index];
		}
		);

		std::vector<std::shared_ptr<Triangle>> head(triangles.begin(), triangles.begin() + triangles.size() / 2);
		std::vector<std::shared_ptr<Triangle>> tail(triangles.begin() + triangles.size() / 2, triangles.end());

		node->left = kdTreeInit(head);
		node->right = kdTreeInit(tail);
		node->box = merge(node->left->box, node->right->box);
		node->obj = nullptr;
	}
	return node;
}



void intersectWithKDTree(BVHNode* root, const Ray& ray, float tmin, float tmax, Intersection& i_nearest, float& u, float& v)
{
	if ((root->left == nullptr) && (root->right == nullptr))
	{
		root->obj->intersect(ray, tmin, tmax, i_nearest, u, v);
		return;
	}

	if (root->box.intersect(ray))
	{
		Intersection i1, i2;
		intersectWithKDTree(root->left, ray, tmin, tmax, i1, u, v);
		intersectWithKDTree(root->right, ray, tmin, tmax, i2, u, v);

		if (i1.exist && i2.exist)
		{
			i_nearest = (i1.time < i2.time) ? i1 : i2;
		}
		else if (i1.exist)
		{
			i_nearest = i1;
		}
		else
		{
			i_nearest = i2;
		}
		return;
	}
}