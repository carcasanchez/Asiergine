#include "ComponentCamera.h"
#include "QuadTree.h"
#include "mmgr\mmgr.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "imgui\imgui.h"
#include "GameObject.h"
#include "CompTransform.h"


ComponentCamera::ComponentCamera(GameObject* g, float near_distance, float far_distance, bool active) : Component(g), cam_active(active)
{
	type = COMPONENT_CAMERA;
	
	frustum.nearPlaneDistance = near_distance;
	frustum.farPlaneDistance = far_distance;

	frustum.type = PerspectiveFrustum;
	frustum.front = float3(0, 0, 1);
	frustum.up = float3(0, 1, 0);

	frustum.pos = float3(0, 0, 0);
	frustum.verticalFov = 1.0;
	aspect_ratio = App->window->GetAspectRatio();

	frustum.horizontalFov = math::Atan(aspect_ratio*math::Tan(frustum.verticalFov/2))*2;

}


ComponentCamera::~ComponentCamera(){}


void ComponentCamera::Update(float real_dt, float game_dt)
{
	if (IsActive() == true)
	{
		aspect_ratio = App->window->GetAspectRatio();

		Component* t = game_object->GetComponentByType(COMPONENT_TRANSFORM);
		if (t)
		{
			frustum.pos = ((CompTransform*)t)->GetTranslation();

			frustum.front = ((CompTransform*)t)->GetMatrix().Row3(2);

			frustum.up = ((CompTransform*)t)->GetMatrix().Row3(1);
		}
		
		App->renderer3D->SetBoxToDraw(&frustum);
		if(App->renderer3D->frustum_culling)
			SendQuadTreeGameObjectsToPaint(&App->scene->scene_quadtree.root);
	}	
}

void ComponentCamera::SendQuadTreeGameObjectsToPaint(QuadTreeNodeObj* node)
{	
	for (std::vector<QuadTreeNodeObj*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
	{
		if (frustum.Contains((*it)->box))
		{
			SendQuadTreeGameObjectsToPaint((*it));
			for (std::vector<GameObject*>::iterator obj_it = (*it)->game_objects.begin(); obj_it != (*it)->game_objects.end(); obj_it++)
			{
				if(frustum.Contains(*(*obj_it)->GetTransformedBox()))
					(*obj_it)->SendAllMeshesToDraw();
			}
		}
	}
}

void ComponentCamera::OnEditor()
{
	if (IsActive() == true)
	{
		ImGui::TextWrapped("Aspect ratio:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.3f", aspect_ratio);


		ImGui::DragFloat("Near Distance", &frustum.nearPlaneDistance, 0.1, 0.0, frustum.farPlaneDistance);

		ImGui::DragFloat("Far Distance", &frustum.farPlaneDistance, 0.1);


		static int selected_option = 0;
		if (ImGui::Combo("Perspective type", &selected_option, "Perspective\0Orthographic", 2))
		{
			if (selected_option == 0)
				frustum.type = PerspectiveFrustum;
			else if (selected_option == 1)
				frustum.type = OrthographicFrustum;
		}


		if (ImGui::DragFloat("Field of View", &frustum.verticalFov, 0.1, 0.1))
		{
			frustum.horizontalFov = math::Atan(aspect_ratio*math::Tan(frustum.verticalFov / 2)) * 2;
		}
	}
}

