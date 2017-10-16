#include "CompTransform.h"
#include "Application.h"
#include "imgui\imgui.h"

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

void CompTransform::SetRotation(float x, float y, float z, float w)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	rotation.w = w;
}

void CompTransform::OnEditor()
{
	float x = GetTranslation().x;
	float y = GetTranslation().y;
	float z = GetTranslation().z;

	ImGui::TextWrapped("Translation: ");
	ImGui::DragFloat("X: ", &x);

	ImGui::DragFloat("Y: ", &y);

	ImGui::DragFloat("Z: ", &z);;
}
