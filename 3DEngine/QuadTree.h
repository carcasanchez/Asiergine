#pragma once
#include "Globals.h"

class GameObject;

class QuadTreeNodeObj
{
public:
	QuadTreeNodeObj(float3 min_point, float3 max_point) { box = math::AABB(min_point, max_point); }
	QuadTreeNodeObj() {}
	~QuadTreeNodeObj();

	void Insert(GameObject* game_object);
	void Remove();
	int GetMaxGameObjects() { return max_game_objects; }
	void Clear();
	void SetAABBToDraw();
	void Partition();
	void Fill();
	bool IsFull() { return game_objects.size() >= max_game_objects; }

	math::AABB box;
	
private:	
	std::vector<QuadTreeNodeObj*> children;
	int max_game_objects = 2;
	std::vector<GameObject*> game_objects;
	float3 minaabb = { 0, MIN_SCENE_POINT_Y, 0 };
	float3 maxaabb = { 1,MAX_SCENE_POINT_X,1 };
	math::AABB minimum_aabb = math::AABB(minaabb, maxaabb);
};


class QuadTreeObj
{
public:
	QuadTreeObj();
	~QuadTreeObj();
	//void Draw();
	void Calcutale();
	void DeleteAll();
	QuadTreeNodeObj root;
private:
	std::vector<GameObject*> game_objects;
	
};

