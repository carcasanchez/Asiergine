#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(const JSON_Object* config_data);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	bool SaveConfig(JSON_Object* config_data);
private:

	void CalculateViewMatrix();
	void ControlCamera();

public:
	
	vec3 X, Y, Z, Position, Reference;
	vec3 ViewVector;
private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};