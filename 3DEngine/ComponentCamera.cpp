#include "ComponentCamera.h"



ComponentCamera::ComponentCamera(GameObject* g, float near_distance, float far_distance, bool active) : Component(g), active(active)
{
	type = COMPONENT_CAMERA;


	frustum.nearPlaneDistance = near_distance;
	frustum.farPlaneDistance = far_distance;

}


ComponentCamera::~ComponentCamera()
{
}

