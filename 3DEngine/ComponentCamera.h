#pragma once
#include "Component.h"
#include "Globals.h"
#include <vector>

class QuadTreeNodeObj;
class ComponentCamera :	public Component
{
public:
	ComponentCamera(GameObject*, float near_distance = 0.1f, float far_distance = 30.0f, bool active = false);
	~ComponentCamera();

	void Update(float real_dt, float game_dt);

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

	const AABB* GetBox() const { return &camera_box; }

	bool IsActive() { return active; }

	void OnEditor();


	bool cam_active = false;

	math::Frustum frustum;

	//Scene serialization--------------------------------------------------
	uint PrepareToSave();
	void Save(char* &cursor);

private:
	float aspect_ratio;

	math::AABB camera_box;
};

