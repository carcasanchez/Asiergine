#pragma once
#include "Globals.h"

class GameObject;
template <typename TYPE>
{
public:
	math::AABB Create(float3 center, float3 half_width);

	void Insert(GameObject* game_object);
	void Remove(GameObject* game_object);

	void Clear();

	bool Intersect(std::vector<GameObject*> &game_bjects, const TYPE &primitive);


private:
	math::float3 center;
	math::float3 half_width;
};

template<typename TYPE>
{
public:

private:
};

