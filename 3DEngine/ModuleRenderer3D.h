#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "Primitive.h"

class ComponentMesh;

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

	bool SaveConfig(JSON_Object* config_data);

	void SetMeshToDraw(ComponentMesh*);

public:

	std::vector<Light> lights;
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;


	//Rendering options--------
	bool gl_depth_enabled;
	bool gl_cull_face_enabled;
	bool gl_lighting_enabled;
	bool gl_color_material_enabled;
	bool gl_texture_2D_enabled;
	bool gl_wireframe_enabled;
	bool hard_poly_enabled;


private:
	std::queue<ComponentMesh*> meshes_to_draw;

	void DrawGeometry();

	Plane_prim plane;
};