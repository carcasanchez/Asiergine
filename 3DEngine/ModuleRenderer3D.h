#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"


class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(const JSON_Object* config_data);
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void ChangeBackgroundColor(Color c);

public:

	std::vector<Light> lights;
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool gl_depth_enabled = true;
	bool gl_cull_face_enabled = true;
	bool gl_lighting_enabled = true;
	bool gl_color_material_enabled = true;
	bool gl_texture_2D_enabled = true;
	bool gl_wireframe_enabled = false;

};