#pragma once
#include "Component.h"
#include "Globals.h"

class ComponentCamera :	public Component
{
public:
	ComponentCamera(GameObject*, float near_distance = 0.1f, float far_distance = 30.0f, bool active = false);
	~ComponentCamera();

	void Update();



	bool active = false;

private:
	math::Frustum frustum;

};

