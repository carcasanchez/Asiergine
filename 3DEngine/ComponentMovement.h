#ifndef _COMPONENT_MOVEMENT_H_
#define _COMPONENT_MOVEMENT_H_

#include "Component.h"
#include "Globals.h"


class ComponentMovement : public Component
{
public:
	ComponentMovement(GameObject* game_object);
	~ComponentMovement() {}

	void Update(float real_dt, float game_dt);

	void SetFirstPoint(float x, float y, float z);
	void SetSecondPoint(float x, float y, float z);

	void OnEditor();
	bool IsActive() { return active; }

private:
	Component* transform;

	math::float3 point1 = math::float3(0, 0, 0);
	math::float3 point2 = math::float3(0, 0, 0);
};

#endif
