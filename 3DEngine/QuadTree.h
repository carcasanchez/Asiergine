#pragma once
#include "Globals.h"

class GameObject;
template <typename TYPE>
class QuadTreeNodeObj
{
public:
	QuadTreeNodeObj(float3 size, std::vector<QuadTreeNodeObj*>* children, int max_game_objects) : size(size),children(children),max_game_objects(max_game_objects) {}
	~QuadTreeNodeObj();
	math::AABB Create(float3 center, float3 half_width);

	void Insert(GameObject* game_object);
	void Remove(GameObject* game_object);
	int GetMaxGameObjects() { return max_game_objects; }
	void Clear();

	bool Intersect(std::vector<GameObject*> &game_bjects, const TYPE &primitive);
	
private:	
	math::float3 size = float3::zero;
	std::vector<QuadTreeNodeObj*> children = nullptr;
	int max_game_objects = 6;
	std::vector<GameObject*> game_objects;
};


class QuadTreeObj
{
public:
	QuadTreeObj() {}
	~QuadTreeObj();
	void Draw();
private:
	QuadTreeNodeObj<math::AABB>* root;
	std::vector<GameObject*> game_objects;
	
};

