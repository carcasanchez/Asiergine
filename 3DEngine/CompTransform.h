#pragma once
#include "Component.h"
#include "Globals.h"

class CompTransform : public Component
{
public:
	CompTransform(GameObject* game_object);
	~CompTransform() {}

	math::float3 GetTranslation();
	void SetTranslation(float x, float y, float z);

	math::float3 GetScale();
	void SetScale(float x, float y, float z);

	math::Quat GetRotation();
	void SetRotation(Quat rot);

	void OnEditor();
	bool IsActive() { return active; }
	void Update(float real_dt, float game_dt);

	const float* GetMatrixPtr();
	float4x4 GetMatrix()
	{
		return matrix;
	}


	float4x4 GetParentTransform();

private:
	math::float3 translation;
	math::float3 scale;
	math::Quat rotation;

	math::float3 rot_in_euler;

	math::float4x4 matrix;
	math::float4x4 local_matrix;

};
