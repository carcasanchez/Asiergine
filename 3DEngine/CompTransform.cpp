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

void CompTransform::Update(float real_dt, float game_dt)
{
	RefreshMatrices();
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
		float scale_tmp[3] = { s_x, s_y, s_z };

		ImGui::TextWrapped("Scale:       ");
		ImGui::SameLine();
		if (ImGui::DragFloat3(" ", scale_tmp, drag_speed) && GetGameObject()->IsStatic() == false)
			SetScale(scale_tmp[0], scale_tmp[1], scale_tmp[2]);

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

		if (ImGuizmo::IsOver())
		{
			App->editor->LockSelection();
		}
		else App->editor->UnLockSelection();

		//GUIZMOS


		if (App->editor->AreGizmosLocked())
			ImGuizmo::Enable(false);
		else ImGuizmo::Enable(true);

		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		
		static bool local_opt = false;

		ImGui::Separator();
		if (ImGui::Checkbox("Local", &local_opt))
		{
			if(local_opt)
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			else mCurrentGizmoMode = ImGuizmo::WORLD;
		}

		if (ImGui::Button("Translate") || App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();			
		if (ImGui::Button("Rotate") || App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::Button("Scale") || App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			mCurrentGizmoOperation = ImGuizmo::SCALE;
			
	
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		
		float4x4 view_matrix = App->camera->frustum.ViewMatrix();
		float4x4 proj_matrix = App->camera->frustum.ProjectionMatrix();
		view_matrix.Transpose();
		proj_matrix.Transpose();


		float4x4 trs_matrix = matrix;
		ImGuizmo::Manipulate(view_matrix.ptr(), proj_matrix.ptr(), mCurrentGizmoOperation, mCurrentGizmoMode, trs_matrix.ptr());

		if (ImGuizmo::IsUsing())
		{					
			trs_matrix.Transpose();
			trs_matrix = GetParentTransform().Transposed().Inverted() * trs_matrix;

			float3 new_pos;
			float3 new_scale;
			Quat new_q; 								
			trs_matrix.Decompose(new_pos, new_q, new_scale);
					
			if(mCurrentGizmoOperation==ImGuizmo::TRANSLATE)
				SetTranslation(new_pos.x, new_pos.y, new_pos.z);
			if (mCurrentGizmoOperation == ImGuizmo::SCALE)
				SetScale(new_scale.x, new_scale.y, new_scale.z);
			if (mCurrentGizmoOperation == ImGuizmo::ROTATE)
			{
				rot_in_euler = new_q.ToEulerXYZ()*RADTODEG;
				SetRotation(new_q);
			}

				
		}
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



float4x4 CompTransform::GetParentTransform() const
{
	GameObject* g = game_object->GetParent();
	float4x4 parent_transform = float4x4::identity;

	if (g != nullptr)
	{
		CompTransform* c_transform = ((CompTransform*)g->GetComponentByType(COMPONENT_TRANSFORM));
		if (c_transform)
			parent_transform = c_transform->GetGlobalTransform();

		return parent_transform;
	}	
}

void CompTransform::RefreshMatrices()
{
	float4x4 rotation_matrix = float4x4::FromQuat(rotation);
	local_matrix = float4x4::FromTRS(translation, rotation_matrix, scale);
	local_matrix.Transpose();
	matrix = UpdateParentTransform();

	std::vector<GameObject*> childrens = game_object->GetChildrens();

	for (int i = 0; i < childrens.size(); i++)
	{
		Component* transform = childrens[i]->GetComponentByType(COMPONENT_TRANSFORM);
		((CompTransform*)transform)->RefreshMatrices();
	}

}



const float * CompTransform::GetMatrixPtr() const
{
	return (float*) matrix.v;
}

float4x4 CompTransform::UpdateParentTransform()
{
	return local_matrix * GetParentTransform();
}


float4x4 CompTransform::GetAllParentTransform() const
{
	float4x4 all_parent_trsf = float4x4::identity;

	GameObject* parent = game_object->GetParent();

	if (parent)
	{
				CompTransform* c_transform = ((CompTransform*)parent->GetComponentByType(COMPONENT_TRANSFORM));
				if (c_transform)
				{
					all_parent_trsf = c_transform->GetLocalTransform().Transposed();
					all_parent_trsf = c_transform->GetAllParentTransform() * all_parent_trsf;
				}
	}
	
	return all_parent_trsf;
}


//-------------------------------------------------------------
uint CompTransform::PrepareToSave() const
{
	uint size = 0;

	//Translation
	size += sizeof(float) * 3;
	//Rotation
	size += sizeof(float) * 4;
	//Scale
	size += sizeof(float) * 3;
	
	return size;
}

void CompTransform::Save(char *& cursor) const
{	
	uint size_of = sizeof(float);

	//Copy position
	memcpy(cursor, &translation.x, size_of);
	cursor += size_of;
	memcpy(cursor, &translation.y, size_of);
	cursor += size_of;
	memcpy(cursor, &translation.z, size_of);
	cursor += size_of;
	
	//Copy scale
	memcpy(cursor, &scale.x, size_of);
	cursor += size_of;
	memcpy(cursor, &scale.y, size_of);
	cursor += size_of;
	memcpy(cursor, &scale.z, size_of);
	cursor += size_of;
	
	//Copy rot
	memcpy(cursor, &rotation.x, size_of);
	cursor += size_of;
	size_of = sizeof(float);
	memcpy(cursor, &rotation.y, size_of);
	cursor += size_of;
	size_of = sizeof(float);
	memcpy(cursor, &rotation.z, size_of);
	cursor += size_of;
	size_of = sizeof(float);
	memcpy(cursor, &rotation.w, size_of);
	cursor += size_of;
}