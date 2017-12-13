#include "ComponentMovement.h"
#include "Application.h"
#include "GameObject.h"

ComponentMovement::ComponentMovement(GameObject * game_object) :Component(game_object)
{
	type = COMPONENT_MOVEMENT;
	transform = (CompTransform*)game_object->GetComponentByType(COMPONENT_TRANSFORM);
}

void ComponentMovement::Update(float real_dt, float game_dt)
{
	MoveFromPointToPoint(game_dt);
}

void ComponentMovement::SetFirstPoint(float x, float y, float z)
{
	point1.x = x;
	point1.y = y;
	point1.z = z;
}

void ComponentMovement::SetSecondPoint(float x, float y, float z)
{
	point2.x = x;
	point2.y = y;
	point2.z = z;
}

void ComponentMovement::MoveFromPointToPoint(float game_dt)
{
	if (game_dt > 0.0f && GetGameObject()->IsStatic() == false)
	{
		if ((point1.x != 0.0f || point1.y != 0.0f || point1.z != 0.0f) && (point2.x != 0.0f || point2.y != 0.0f || point2.z != 0.0f))
		{
			float speed = 0.0003 * game_dt;
			float3 my_pos = transform->GetTranslation();

			if (math::Sqrt((point1.x - my_pos.x)*(point1.x - my_pos.x) + (point1.y - my_pos.y)*(point1.y - my_pos.y) + (point1.z - my_pos.z)*(point1.z - my_pos.z)) < 1.1f)
				closest_point = math::float3((point2.x - my_pos.x), (point2.y - my_pos.y), (point2.z - my_pos.z));

			if (math::Sqrt((point2.x - my_pos.x)*(point2.x - my_pos.x) + (point2.y - my_pos.y)*(point2.y - my_pos.y) + (point2.z - my_pos.z)*(point2.z - my_pos.z)) < 1.1f)
				closest_point = math::float3((point1.x - my_pos.x), (point1.y - my_pos.y), (point1.z - my_pos.z));

			if (closest_calculated == false)
				closest_point = GetClosestPoint();

			transform->SetTranslation(my_pos.x + (closest_point.x * speed), my_pos.y + (closest_point.y * speed), my_pos.z + (closest_point.z * speed));
		}
	}
}

math::float3 ComponentMovement::GetClosestPoint()
{
	closest_calculated = true;
	float3 my_pos = transform->GetTranslation();
	float dist_point1 = math::Sqrt((point1.x - my_pos.x)*(point1.x - my_pos.x) + (point1.y - my_pos.y)*(point1.y - my_pos.y) + (point1.z - my_pos.z)*(point1.z - my_pos.z));
	float dist_point2 = math::Sqrt((point2.x - my_pos.x)*(point2.x - my_pos.x) + (point2.y - my_pos.y)*(point2.y - my_pos.y) + (point2.z - my_pos.z)*(point2.z - my_pos.z));
	if (dist_point1 < dist_point2)
		return math::float3((point1.x - my_pos.x),(point1.y - my_pos.y), (point1.z - my_pos.z));
	else
		return math::float3((point2.x - my_pos.x), (point2.y - my_pos.y), (point2.z - my_pos.z));
}

void ComponentMovement::OnEditor()
{
	if (IsActive() == true)
	{
		float drag_speed = 0.1f;
		float first_point[3] = { point1.x,point1.y,point1.z };
		float second_point[3] = { point2.x,point2.y,point2.z };

		ImGui::TextWrapped("Point1:");
		ImGui::SameLine();
		if (ImGui::DragFloat3("", first_point, drag_speed) && GetGameObject()->IsStatic() == false)
		{
			if(closest_calculated == false)
				SetFirstPoint(first_point[0], first_point[1], first_point[2]);
		}

		ImGui::TextWrapped("Point2:");
		ImGui::SameLine();
		if (ImGui::DragFloat3(" ", second_point, drag_speed) && GetGameObject()->IsStatic() == false)
		{
			if (closest_calculated == false)
				SetSecondPoint(second_point[0], second_point[1], second_point[2]);
		}

		if (ImGui::Button("Reset Movement") && GetGameObject()->IsStatic() == false)
		{
			SetFirstPoint(0.0f, 0.0f, 0.0f);
			SetSecondPoint(0.0f, 0.0f, 0.0f);
			closest_calculated = false;
		}
	}
}
