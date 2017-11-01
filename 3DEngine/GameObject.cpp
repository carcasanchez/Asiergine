#include "GameObject.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "Component.h"
#include "CompTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "MathGeoLib\include\Algorithm\Random\LCG.h"

GameObject::GameObject(const char* name): name(name)
{
	//bounding_box.SetNegativeInfinity();
	LCG rand_gen;
	UID = rand_gen.Int(1 , 32000);
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	std::vector<Component*>::iterator it = components.begin();
	while (it != components.end())
	{
		(*it)->Update();
		++it;
	}

	for (int i = 0; i < children.size(); i++)
	{
	    //Check parent correlation
		if (children[i]->GetParent() != this)
		{
			LOG("YOU ARE ADOPTED");
			assert(1 == 0);
		}

		children[i]->Update();
	}
	

	Component* t = GetComponentByType(COMPONENT_TRANSFORM);
	//Update bounding box -------------------------------------------------------------------------
	if (t && bounding_box.IsFinite())
	{
		float4x4 matrix = ((CompTransform*)t)->GetMatrix();
		matrix.Transpose();
		float4 new_min_point = matrix * float4(bounding_box.minPoint, 1);
		float4 new_max_point = matrix * float4(bounding_box.maxPoint, 1);
		

		transformed_bounding_box = bounding_box;
		transformed_bounding_box.minPoint = { new_min_point.x, new_min_point.y, new_min_point.z };
		transformed_bounding_box.maxPoint = { new_max_point.x, new_max_point.y, new_max_point.z };
	
	
		//Debug Bounding Box
		if (App->scene->debug_boxes)
		{
			App->renderer3D->SetBoxToDraw(transformed_bounding_box);
		}
	}

}

void GameObject::CleanUp()
{

	//Delete components
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		delete (*it);
	}

	//Delete children
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->CleanUp();
		delete (*it);
	}
}

Component* GameObject::GetComponentByType(ComponentType type)
{
	if (components.empty() == false)
	{
		std::vector<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			if ((*it)->GetType() == type)
			{
				return (*it);
			}
			++it;
		}
	}
	return nullptr;
}

std::vector<Component*> GameObject::GetAllComponentOfType(ComponentType type)
{

	std::vector<Component*> objects;

	if (components.empty() == false)
	{
		std::vector<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			if ((*it)->GetType() == type)
			{
				objects.push_back((*it));
			}
			++it;
		}
	}

	return objects;
}

void GameObject::SetParent(GameObject* new_parent)
{

	if (parent)
	{//Delete this object from the old parent childrens	
		for (std::vector<GameObject*>::iterator it = parent->children.begin(); it != parent->children.end(); it++)
		{
			if ((*it) == this)
			{
				parent->children.erase(it);
			}
		}
	}

	parent = new_parent;
	parent->children.push_back(this);

}

//CREATE COMPONENT METHODS----------------------------------------------------
CompTransform * GameObject::CreateComponent_Transform(float3 trans , float3 scale, Quat rot, int16_t UID)
{
	if (GetComponentByType(COMPONENT_TRANSFORM) != nullptr)
	{
		LOG("ERROR: GameObject %s already has a transform component!", name.c_str());
		return nullptr;
	}

	CompTransform* new_transform = new CompTransform(this);

	new_transform->SetTranslation(trans.x, trans.y, trans.z);
	new_transform->SetScale(scale.x, scale.y, scale.z);
	new_transform->SetRotation(rot);

	if (UID >= 0)
		new_transform->SetID(UID);

	components.push_back(new_transform);

	LOG("Creating new Transform Component in %s", name.c_str());

	return new_transform;
}

ComponentMesh * GameObject::CreateComponent_Mesh(const char* m_name, float * ver, uint * ind, uint num_vert, uint num_ind, float* normals, float * texture_coords, int16_t UID)
{
	ComponentMesh* new_mesh = new ComponentMesh(this, ver, ind, num_vert, num_ind, normals, texture_coords);
	new_mesh->name = m_name;

	//Adapt bounding box to geometry-----------------
	std::vector <float3> vertex_array;


	for (int i = 0; i < num_vert*3; i += 3)
		vertex_array.push_back(float3(ver[i], ver[i + 1], ver[i + 2]));

	bounding_box.Enclose(&vertex_array[0], vertex_array.size());


	if (UID >= 0)
		new_mesh->SetID(UID);

	components.push_back(new_mesh);
	LOG("Creating new Mesh with %i vertices in %s", new_mesh->GetNumVertices(), name.c_str());
	return new_mesh;
}

ComponentMaterial * GameObject::CreateComponent_Material(uint texture_id, const char* txt_name, int16_t UID)
{
	if (GetComponentByType(COMPONENT_MATERIAL) != nullptr)
	{
		LOG("ERROR: GameObject %s already has a material component!", name.c_str());
		return nullptr;
	}

	ComponentMaterial* new_mat = new ComponentMaterial(this, texture_id);
	new_mat->texture_name = txt_name;

	if (UID >= 0)
		new_mat->SetID(UID);

	components.push_back(new_mat);
	LOG("Creating new Material in %s", name.c_str());
	return new_mat;
}

ComponentCamera * GameObject::CreateComponent_Camera(float near_dist, float far_dist, bool active, int16_t UID)
{
	if (GetComponentByType(COMPONENT_CAMERA) != nullptr)
	{
		LOG("ERROR: GameObject %s already has a camera component!", name.c_str());
		return nullptr;
	}

	ComponentCamera* new_camera = new ComponentCamera(this, near_dist, far_dist, active);

	if (active)
		App->renderer3D->SetActiveCamera(new_camera);

	if (UID >= 0)
		new_camera->SetID(UID);

	components.push_back(new_camera);
	LOG("Creating new Camera in %s", name.c_str());

	return new_camera;
}

bool GameObject::PutInQuadTree(QuadTreeNodeObj* node)
{
	bool ret = true;
	if (node->box.Intersects(transformed_bounding_box))
	{
		if (node->IsFull())
			ret = false;
		else {
			node->Insert(this);
			for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
			{
				ret = (*it)->PutInQuadTree(node);
				if (ret == false)
				{
					node->Insert(this);
					for (std::vector<GameObject*>::iterator it1 = children.begin(); it1 != children.end(); ++it1)
					{
						node->Insert((*it));
					}
					break;
				}
			}
		}
	}
	return ret;
}

void GameObject::OnEditor()
{
	ImGui::TextWrapped("%s", name.c_str());
	ImGui::Separator();
	ImGui::Checkbox("Static", &obj_static);
	int id = 0;
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		ImGui::PushID(id);
			if (ImGui::CollapsingHeader((*it)->GetComponentNameByType()))
			{
				if (ImGui::Checkbox("Active", &(*it)->active))
				{
					if ((*it)->active == true)
						(*it)->Enable();
					if ((*it)->active == false)
						(*it)->Disable();
				}
				ImGui::SameLine();
				if (ImGui::Button("Up"))
				{
					if (it != components.begin())
						std::iter_swap(it, it - 1);
				}
				ImGui::SameLine();
				if (ImGui::Button("Down"))
				{
					if (it + 1 != components.end())
						std::iter_swap(it, it + 1);
				}

				(*it)->OnEditor();
			}
			ImGui::PopID();
			id++;
	}
	
}
