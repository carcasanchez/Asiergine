#ifndef _COMPONENT_MOVEMENT_H_
#define _COMPONENT_MOVEMENT_H_

#include "Component.h"
#include "CompTransform.h"
#include "Globals.h"

class CompTransform;

class ComponentMovement : public Component
{
public:
	ComponentMovement(GameObject* game_object);
	~ComponentMovement() {}

	void Update(float real_dt, float game_dt);

	void SetFirstPoint(float x, float y, float z);
	void SetSecondPoint(float x, float y, float z);

	void MoveFromPointToPoint(float game_dt);
	math::float3 GetClosestPoint();
	void SetSpeed(float speed);

	void OnEditor();
	bool IsActive() { return active; }

	uint PrepareToSave() const;
	void Save(char *& cursor) const;

private:
	CompTransform* transform;

	math::float3 point1 = math::float3(0, 0, 0);
	math::float3 point2 = math::float3(0, 0, 0);
	float speed = 0.0003f;

	float3 closest_point = math::float3(0, 0, 0);

	bool closest_calculated = false;
};

#endif
