#include "QuadTree.h"

template<typename TYPE>
QuadTreeNodeObj<TYPE>::QuadTreeNodeObj(float3 center, float3 half_width): center(center), half_width(half_width)
{
}

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
void QuadTreeNodeObj<TYPE>::Insert(GameObject * game_object)
{
}

template<typename TYPE>
void QuadTreeNodeObj<TYPE>::Remove(GameObject * game_object)
{
}

template<typename TYPE>
void QuadTreeNodeObj<TYPE>::Clear()
{
}

template<typename TYPE>
bool QuadTreeNodeObj<TYPE>::Intersect(std::vector<GameObject*>& game_bjects, const TYPE & primitive)
{
	return false;
}

template<typename TYPE>
QuadTreeObj<TYPE>::QuadTreeObj()
{}

template<typename TYPE>
QuadTreeObj<TYPE>::~QuadTreeObj()
{}
