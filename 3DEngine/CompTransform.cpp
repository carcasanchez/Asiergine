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
	if (ImGui::DragFloat3("", location, drag_speed))
		SetTranslation(location[0], location[1], location[2]);

	//Scale
	float s_x = GetScale().x;
	float s_y = GetScale().y;
	float s_z = GetScale().z;
	float scale[3] = { s_x, s_y, s_z };

	ImGui::TextWrapped("Scale:       ");
	ImGui::SameLine();
	if (ImGui::DragFloat3(" ", scale, drag_speed))
		SetScale(scale[0], scale[1], scale[2]);

	//Rotation
	float r_x = GetRotation().ToEulerXYZ().x;
	float r_y = GetRotation().ToEulerXYZ().y;
	float r_z = GetRotation().ToEulerXYZ().z;
	float rotate[3] = { r_x, r_y, r_z };

	ImGui::TextWrapped("Rotation:    ");
	ImGui::SameLine();
	if (ImGui::DragFloat3("  ", rotate, drag_speed))
	{
		Quat new_rot = Quat::FromEulerXYZ(rotate[0], rotate[1], rotate[2]);
		SetRotation(new_rot);
	}
}

void CompTransform::Update()
{

	float4x4 rotation_matrix = float4x4::FromQuat(rotation);	
	matrix = float4x4::FromTRS(translation, rotation_matrix, scale);
	matrix.Transpose();
	matrix = GetParentTransform();

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
