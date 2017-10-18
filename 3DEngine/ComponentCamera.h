#pragma once
#include "Component.h"
#include "Globals.h"

class ComponentCamera :	public Component
{
public:
	ComponentCamera(GameObject*, float near_distance = 0.1f, float far_distance = 30.0f, bool active = false);
	~ComponentCamera();

	void Update();

	float GetNearDistance()
	{
		return frustum.nearPlaneDistance;
	}

	float GetFarDistance()
	{
		return frustum.farPlaneDistance;
	}
	
	void SetNearDistance(float d)
	{
		frustum.nearPlaneDistance = d;
	}

	void SetFarDistance(float d)
	{
		frustum.farPlaneDistance = d;
	}


	void OnEditor();


	bool active = false;

private:
	math::Frustum frustum;
	float aspect_ratio;
};

