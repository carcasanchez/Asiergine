#include "ComponentMovement.h"
#include "Application.h"
#include "GameObject.h"

ComponentMovement::ComponentMovement(GameObject * game_object) :Component(game_object)
{
	type = COMPONENT_MOVEMENT;
	transform = game_object->GetComponentByType(COMPONENT_TRANSFORM);
}

void ComponentMovement::Update(float real_dt, float game_dt)
{
	
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
			SetFirstPoint(first_point[0], first_point[1], first_point[2]);
		}

		ImGui::TextWrapped("Point2:");
		ImGui::SameLine();
		if (ImGui::DragFloat3(" ", second_point, drag_speed) && GetGameObject()->IsStatic() == false)
		{
			SetSecondPoint(second_point[0], second_point[1], second_point[2]);
		}

		if (ImGui::Button("Reset Movement") && GetGameObject()->IsStatic() == false)
		{
			SetFirstPoint(0.0f, 0.0f, 0.0f);
			SetSecondPoint(0.0f, 0.0f, 0.0f);
		}
	}
}
