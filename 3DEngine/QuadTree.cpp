#include "QuadTree.h"
#include <vector>

//Node functions--------------------------------
template<typename TYPE>
QuadTreeNodeObj<TYPE>::~QuadTreeNodeObj()
{
}

template<typename TYPE>
math::AABB QuadTreeNodeObj<TYPE>::Create(float3 center, float3 half_width)
{
	return math::AABB(center, half_width);
}

template<typename TYPE>
void QuadTreeNodeObj<TYPE>::Insert(GameObject* game_object)
{
	game_objects.push_back(game_object);
}

template<typename TYPE>
void QuadTreeNodeObj<TYPE>::Remove(GameObject* game_object)
{
	game_objects.pop_back(game_object);
}

template<typename TYPE>
void QuadTreeNodeObj<TYPE>::Clear()
{
	std::vector<QuadTreeNodeObj*>::iterator it = children.end();
	while (it != children.begin())
	{
		children.pop_back((*it));
		--it;
	}
	std::vector<GameObject*>::iterator it1 = game_objects.end();
	while (it1 != game_objects.begin())
	{
		game_objects.pop_back((*it));
		--it;
	}
}

template<typename TYPE>
bool QuadTreeNodeObj<TYPE>::Intersect(std::vector<GameObject*>& game_bjects, const TYPE & primitive)
{
	return false;
}


//QuadTree functions---------------------------------------
QuadTreeObj::~QuadTreeObj()
{}

void QuadTreeObj::Draw()
{
	int i = 0;
	root->Create(float3::zero, { 12,12,12 });
	std::vector<GameObject*>::iterator it = game_objects.begin();
	while (it != game_objects.end())
	{
		++i;
		++it;
	}
	if (i > root->GetMaxGameObjects())
	{
		root->Create({ 1.5,6,1.5 }, { 3,12,3 });
		root->Create({ -1.5,6,-1.5 }, { 3,12,3 });
		root->Create({ 1.5,6,-1.5 }, { 3,12,3 });
		root->Create({ -1.5,6,1.5 }, { 3,12,3 });
	}

	
}



