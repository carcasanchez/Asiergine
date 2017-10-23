#pragma once
#include "Globals.h"

class GameObject;

class QuadTreeNodeObj
{
public:
	QuadTreeNodeObj(float3 min_point, float3 max_point, std::vector<QuadTreeNodeObj*> children, int max_game_objects){}
	QuadTreeNodeObj() {}
	~QuadTreeNodeObj();

	void Insert(GameObject* game_object);
	void Remove();
	int GetMaxGameObjects() { return max_game_objects; }
	void Clear();
	void SetAABBToDraw();
	void Partition();
	//bool Intersect(std::vector<GameObject*> &game_bjects, primitive);
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
	QuadTreeNodeObj root;
private:
	std::vector<GameObject*> game_objects;
	
};

