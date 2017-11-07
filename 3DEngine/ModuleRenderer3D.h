#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "Primitive.h"


class ComponentMesh;
class ComponentCamera;
class QuadTreeNodeObj;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(const JSON_Object* config_data);
	update_status PreUpdate(float real_dt, float game_dt);
	update_status PostUpdate(float real_dt, float game_dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void ChangeBackgroundColor(Color c);

	bool SaveConfig(JSON_Object* config_data);

	void SetMeshToDraw(ComponentMesh*);
	void SetBoxToDraw(math::AABB);
	void SetBoxToDraw(math::Frustum*);
	void SendQuadTreeGameObjectsToPaint(QuadTreeNodeObj* node);

	void SetActiveCamera(ComponentCamera* c)
	{
		active_camera = c;
	};

public:

	std::vector<Light> lights;
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;


	//Rendering options--------
	bool gl_depth_enabled;
	bool gl_cull_face_enabled;
	bool gl_lighting_enabled;
	bool gl_color_material_enabled;
	bool gl_texture_2D_enabled;
	bool gl_wireframe_enabled;
	bool hard_poly_enabled;
	bool frustum_culling;

private:
	std::queue<ComponentMesh*> meshes_to_draw;
	std::queue<math::AABB> boxes_to_draw;
	std::queue<math::Frustum*> frustums_to_draw;

	void DrawGeometry();
	void DrawDebugBoxes();
	void DrawCameraFrustums();

	bool CheckFrustumCulling(const ComponentMesh*);

	Plane_prim plane;


	ComponentCamera* active_camera = nullptr;
};