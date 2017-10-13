#pragma once
#include "Component.h"
#include "Assimp\include\quaternion.h"
#include "Assimp\include\vector3.h"
class CompTransform : public Component
{
public:
	CompTransform(ComponentType type, GameObject* game_object);
	~CompTransform() {}

private:
	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;
};
