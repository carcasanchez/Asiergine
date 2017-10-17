#include "ComponentCamera.h"
#include "mmgr\mmgr.h"
#include "Application.h"
#include "ModuleScene.h"



ComponentCamera::ComponentCamera(GameObject* g, float near_distance, float far_distance, bool active) : Component(g), active(active)
{
	type = COMPONENT_CAMERA;
	
	frustum.nearPlaneDistance = near_distance;
	frustum.farPlaneDistance = far_distance;

	frustum.pos = float3(0, 0, 0);
	frustum.horizontalFov = 1.0;
	frustum.verticalFov = 1.0;
	frustum.type = PerspectiveFrustum;
	frustum.front = float3(0, 0, 1);
	frustum.up = float3(0, 1, 0);


}


ComponentCamera::~ComponentCamera(){}


void ComponentCamera::Update()
{
	App->renderer3D->SetBoxToDraw(&frustum);
}

