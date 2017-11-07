#include "CompTransform.h"
#include "Application.h"
#include "imgui\imgui.h"
#include "imgui\ImGuizmo.h"
#include "GameObject.h"

CompTransform::CompTransform(GameObject * game_object):Component(game_object)
{
	type = COMPONENT_TRANSFORM;
	translation = float3(0, 0, 0);
	scale = float3(1, 1, 1);
	rotation = Quat::identity;


	rot_in_euler = rotation.ToEulerXYZ()* RADTODEG;
}

math::float3 CompTransform::GetTranslation()
{
	return translation;
}

void CompTransform::SetTranslation(float x, float y, float z)
{
	translation.x = x;
	translation.y = y;
	translation.z = z;
}

math::float3 CompTransform::GetScale()
{
	return scale;
}

void CompTransform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

math::Quat CompTransform::GetRotation()
{
	return rotation;
}

void CompTransform::SetRotation(Quat rot)
{
	rotation = rot;
}

void CompTransform::OnEditor()
{
	if (IsActive() == true)
	{
		//Translation
		float x = GetTranslation().x;
		float y = GetTranslation().y;
		float z = GetTranslation().z;
		float location[3] = { x, y, z };


		float drag_speed = 0.1;

		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
		{
			drag_speed = 1;
		}


		ImGui::TextWrapped("Translation: ");
		ImGui::SameLine();
		if (ImGui::DragFloat3("", location, drag_speed) && GetGameObject()->IsStatic() == false)
			SetTranslation(location[0], location[1], location[2]);

		//Scale
		float s_x = GetScale().x;
		float s_y = GetScale().y;
		float s_z = GetScale().z;
		float scale[3] = { s_x, s_y, s_z };

		ImGui::TextWrapped("Scale:       ");
		ImGui::SameLine();
		if (ImGui::DragFloat3(" ", scale, drag_speed) && GetGameObject()->IsStatic() == false)
			SetScale(scale[0], scale[1], scale[2]);

		//Rotation
		float rotate[3] = { rot_in_euler.x,  rot_in_euler.y, rot_in_euler.z };

		ImGui::TextWrapped("Rotation:    ");
		ImGui::SameLine();
		if (ImGui::DragFloat3("  ", rotate, drag_speed) && GetGameObject()->IsStatic() == false)
		{
			rot_in_euler.x = rotate[0];
			rot_in_euler.y = rotate[1];
			rot_in_euler.z = rotate[2];

			Quat new_rot = Quat::FromEulerXYZ(rotate[0] * DEGTORAD, rotate[1] * DEGTORAD, rotate[2] * DEGTORAD);
			SetRotation(new_rot);
		}

		if (ImGuizmo::IsUsing())
		{

		}


		//GUIZMOS
		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			mCurrentGizmoOperation = ImGuizmo::SCALE;
			
	
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		float4x4 view_matrix = App->camera->frustum.ViewMatrix();
		float4x4 proj_matrix = App->camera->frustum.ProjectionMatrix();
		view_matrix.Transpose();
		proj_matrix.Transpose();

		
		ImGuizmo::Manipulate(view_matrix.ptr(), proj_matrix.ptr(), mCurrentGizmoOperation, mCurrentGizmoMode, matrix.ptr());


		//--------------------------------------------------------------------

		if (ImGui::Button("Reset Transform") && GetGameObject()->IsStatic() == false)
		{
			SetTranslation(0, 0, 0);
			SetRotation(Quat::identity);
			SetScale(1, 1, 1);
			rot_in_euler = math::float3::zero;
		}
	}
}

void CompTransform::Update(float real_dt, float game_dt)
{

	translation.x += 0.001 * game_dt;

	if (GetGameObject()->IsStatic() == false) {
		float4x4 rotation_matrix = float4x4::FromQuat(rotation);
		local_matrix = float4x4::FromTRS(translation, rotation_matrix, scale);
		local_matrix.Transpose();
		matrix = GetParentTransform();
	}

}



const float * CompTransform::GetMatrixPtr()
{
	return (float*) matrix.v;
}

float4x4 CompTransform::GetParentTransform()
{

	 GameObject* g = game_object->GetParent();
	 float4x4 parent_transform = float4x4::identity;
	
	if (g != nullptr)
	{
		 CompTransform* c_transform = ((CompTransform*)g->GetComponentByType(COMPONENT_TRANSFORM));
		 if(c_transform)
			 parent_transform = c_transform->GetParentTransform();
	}


	return local_matrix * parent_transform;
}
