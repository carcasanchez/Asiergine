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

	camera_box.minPoint = float3(-1, -1, -1);
	camera_box.minPoint = float3(1, 1, 1);

}


ComponentCamera::~ComponentCamera(){}


void ComponentCamera::Update(float real_dt, float game_dt)
{
	
	if (IsActive() == true)
	{
		//Update camera aabb
		CompTransform* t = (CompTransform*)game_object->GetComponentByType(COMPONENT_TRANSFORM);
		if (t)
		{
			camera_box.minPoint = t->GetTranslation() - float3(1, 1, 1);
			camera_box.maxPoint = t->GetTranslation() + float3(1, 1, 1);
			frustum.pos = t->GetTranslation();
			frustum.front = t->GetGlobalTransform().Row3(2);
			frustum.up = t->GetGlobalTransform().Row3(1);
		}

		aspect_ratio = App->window->GetAspectRatio();

		
		App->renderer3D->SetBoxToDraw(&frustum);

		if (App->scene->debug_boxes)
		{
			App->renderer3D->SetBoxToDraw(camera_box);
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


		ImGui::DragFloat("Near Dist.", &frustum.nearPlaneDistance, 0.1, 0.0, frustum.farPlaneDistance);

		ImGui::DragFloat("Far Dist.", &frustum.farPlaneDistance, 0.1);


		static int selected_option = 0;
		if (ImGui::Combo("Persp. type", &selected_option, "Perspective\0Orthographic", 2))
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

uint ComponentCamera::PrepareToSave()
{
	uint size = 0;

	size += sizeof(bool); //Active
	size += sizeof(float) * 2; //Far and near distance
	return size;
}

void ComponentCamera::Save(char *& cursor)
{	
		float n_distance = frustum.nearPlaneDistance;
		float f_distance = frustum.farPlaneDistance;
		bool active = IsActive();

		uint size_of = sizeof(float);
		memcpy(cursor, &n_distance, size_of);
		cursor += size_of;

		memcpy(cursor, &f_distance, size_of);
		cursor += size_of;

		size_of = sizeof(bool);
		memcpy(cursor, &active, size_of);
		cursor += size_of;
	
}

