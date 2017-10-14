#pragma once
#include "Component.h"
#include "Globals.h"

class CompTransform : public Component
{
public:
	CompTransform(ComponentType type, GameObject* game_object);
	~CompTransform() {}

	math::float3 GetTranslation();
	void SetTranslation(float x, float y, float z);

	math::float3 GetScale();
	void SetScale(float x, float y, float z);

	math::Quat GetRotation();
	void SetRotation(float x, float y, float z, float w);

private:
	math::float3 translation;
	math::float3 scaling;
	math::Quat rotation;
};
