#include "CompTransform.h"
#include "Application.h"
#include "imgui\imgui.h"
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


		if (ImGui::Button("Reset Transform") && GetGameObject()->IsStatic() == false)
		{
			SetTranslation(0, 0, 0);
			SetRotation(Quat::identity);
			SetScale(1, 1, 1);
			rot_in_euler = math::float3::zero;
		}
	}
}

void CompTransform::Update()
{
	if (GetGameObject()->IsStatic() == false) {
		float4x4 rotation_matrix = float4x4::FromQuat(rotation);
		matrix = float4x4::FromTRS(translation, rotation_matrix, scale);
		matrix.Transpose();
		matrix = GetParentTransform();
	}

}



const float * CompTransform::GetMatrixPtr()
{
	return matrix.ptr();
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


	return matrix * parent_transform;
}
