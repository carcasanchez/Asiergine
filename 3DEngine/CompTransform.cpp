#include "CompTransform.h"
#include "Application.h"

CompTransform::CompTransform(ComponentType type, GameObject * game_object):Component(type,game_object)
{

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
	return scaling;
}

void CompTransform::SetScale(float x, float y, float z)
{
	scaling.x = x;
	scaling.y = y;
	scaling.z = z;
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
