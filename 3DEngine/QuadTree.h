#pragma once
#include "Globals.h"

class GameObject;

class QuadTreeNodeObj
{
public:
	QuadTreeNodeObj(float3 min_point, float3 max_point){}
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
	int max_game_objects = 6;
	std::vector<GameObject*> game_objects;
};


class QuadTreeObj
{
public:
	QuadTreeObj();
	~QuadTreeObj();
	//void Draw();
	void Calcutale();
	QuadTreeNodeObj root;
private:
	std::vector<GameObject*> game_objects;
	
};

