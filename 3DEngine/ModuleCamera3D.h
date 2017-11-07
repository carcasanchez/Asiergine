#pragma once
#include "Module.h"
#include "Globals.h"


class GameObject;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(const JSON_Object* config_data);
	bool Start();
	update_status Update(float real_dt, float game_dt);
	bool CleanUp();

	void LookAt(const float3 &Spot);

	void Move(const float3 &Movement);

	float* GetViewMatrix() const;
	float* GetProjectionMatrix() const;
	float* GetViewMatrixTransposed() const;
	float* GetProjectionMatrixTransposed() const;

	void SetAspectRatio(float);

	float GetAspectRatio() const
	{
		return aspect_ratio;
	};

	bool SaveConfig(JSON_Object* config_data);
	void AdaptToGeometry(GameObject*);

	void ResetCamera();

	//For mouse picking
	void CalculatePickRay();


private:

	void ControlCamera(float dt);


public:	

	math::Frustum frustum;
	math::LineSegment pick_ray;


	float3 pivot_point;

	float camera_speed = 0.01;
	float camera_sensitivity = 0.01;

	
private:
	float aspect_ratio = 1;
};