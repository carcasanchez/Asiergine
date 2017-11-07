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
	bounding_box.SetNegativeInfinity();
	LCG rand_gen;
	UID = rand_gen.Int();
}

GameObject::~GameObject()
{
}

void GameObject::Update(float real_dt, float game_dt)
{
	/*std::vector<Component*>::iterator del_comp = components.begin();
	for (del_comp; del_comp != components.end(); ++del_comp)
	{
		if ((*del_comp)->deleted)
			components_to_erase->push_back((*del_comp));
	}
	std::vector<Component*>::iterator &del_all_comp = components_to_erase->begin();
	for (del_all_comp; del_all_comp != components_to_erase->end(); ++del_all_comp)
	{
		components.erase(del_all_comp);
	}*/


	std::vector<Component*>::iterator it = components.begin();
	while (it != components.end())
	{
		(*it)->Update(real_dt, game_dt);
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
		children[i]->Update(real_dt, game_dt);	
	}
	

	Component* t = GetComponentByType(COMPONENT_TRANSFORM);
	//Update bounding box -------------------------------------------------------------------------
	if (t && bounding_box.IsFinite())
	{
		float4x4 matrix = ((CompTransform*)t)->GetGlobalTransform();
		matrix.Transpose();	
		transformed_bounding_box = bounding_box;
		transformed_bounding_box.TransformAsAABB(matrix);
	
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
				break;
			}
		}
	}

	parent = new_parent;
	parent->children.push_back(this);

}

//CREATE COMPONENT METHODS----------------------------------------------------
CompTransform * GameObject::CreateComponent_Transform(float3 trans , float3 scale, Quat rot, uint UID)
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

	if (UID > 0)
		new_transform->SetID(UID);

	components.push_back(new_transform);

	LOG("Creating new Transform Component in %s", name.c_str());

	return new_transform;
}

ComponentMesh * GameObject::CreateComponent_Mesh(const char* m_name, float * ver, uint * ind, uint num_vert, uint num_ind, float* normals, float * texture_coords, uint UID)
{
	ComponentMesh* new_mesh = new ComponentMesh(this, ver, ind, num_vert, num_ind, normals, texture_coords);
	new_mesh->name = m_name;

	//Adapt bounding box to geometry-----------------
	std::vector <float3> vertex_array;


	for (int i = 0; i < num_vert*3; i += 3)
		vertex_array.push_back(float3(ver[i], ver[i + 1], ver[i + 2]));

	bounding_box.Enclose(&vertex_array[0], vertex_array.size());


	if (UID > 0)
		new_mesh->SetID(UID);

	components.push_back(new_mesh);
	LOG("Creating new Mesh with %i vertices in %s", new_mesh->GetNumVertices(), name.c_str());
	return new_mesh;
}

ComponentMaterial * GameObject::CreateComponent_Material(uint texture_id, const char* txt_name, uint UID)
{
	if (GetComponentByType(COMPONENT_MATERIAL) != nullptr)
	{
		LOG("ERROR: GameObject %s already has a material component!", name.c_str());
		return nullptr;
	}

	ComponentMaterial* new_mat = new ComponentMaterial(this, texture_id);
	new_mat->texture_name = txt_name;

	if (UID > 0)
		new_mat->SetID(UID);

	components.push_back(new_mat);
	LOG("Creating new Material in %s", name.c_str());
	return new_mat;
}

ComponentCamera * GameObject::CreateComponent_Camera(float near_dist, float far_dist, bool active, uint UID)
{
	if (GetComponentByType(COMPONENT_CAMERA) != nullptr)
	{
		LOG("ERROR: GameObject %s already has a camera component!", name.c_str());
		return nullptr;
	}

	ComponentCamera* new_camera = new ComponentCamera(this, near_dist, far_dist, active);

	if (active)
		App->renderer3D->SetActiveCamera(new_camera);

	if (UID > 0)
		new_camera->SetID(UID);


	float3 corners[8];
	new_camera->frustum.GetCornerPoints(corners);

	bounding_box.Enclose(&corners[0], 8);

	components.push_back(new_camera);
	LOG("Creating new Camera in %s", name.c_str());

	return new_camera;
}


//Recursive methods-------------------------------------------------------------------
bool GameObject::PutInQuadTree(QuadTreeNodeObj* node)
{
	bool ret = true;
	if (!IsStatic())
		return ret;
	if (node->IsFull() && !node->IsOfMinSize())
		ret = false;
	else {
		if (node->box.Intersects(transformed_bounding_box))
			node->Insert(this);
		if (node->IsFull() && !node->IsOfMinSize())
			ret = false;
		for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
		{
			ret = (*it)->PutInQuadTree(node);
			if (ret == false || !node->IsOfMinSize())
			{
				break;
			}
		}
	}
	
	return ret;
}


GameObject* GameObject::FindChildByID(uint other_uid) const
{
	GameObject* ret = nullptr;
	
	if (UID == other_uid)
	{
		ret = (GameObject*)this;
	}

	else for (int i = 0; i < children.size(); i++)
	{
		ret = children[i]->FindChildByID(other_uid);
		if (ret != nullptr)
			break;
	}


	return ret;
}

//MOUSE PICKING---------------------------------------------------------------------
void GameObject::CheckMouseRayCollision(math::LineSegment &l, float& distance, GameObject* &best_candidate)
{
	for (int i = 0; i < children.size(); i++)
	{
		if (l.Intersects(*children[i]->GetTransformedBox()))
		{
			children[i]->CheckTriangleCollision(l, distance, best_candidate);
		}

		children[i]->CheckMouseRayCollision(l, distance, best_candidate);
	}

}

void GameObject::CheckTriangleCollision(math::LineSegment &line, float& distance, GameObject* &best_candidate)
{
	CompTransform* transf = (CompTransform*)GetComponentByType(COMPONENT_TRANSFORM);
	std::vector<Component*> meshes = GetAllComponentOfType(COMPONENT_MESH);

	//Check against meshes
	for (int i = 0; i < meshes.size(); i++)
	{
		//Get vertices and indices from every mesh
		 float* vertices = (float*)((ComponentMesh*)meshes[i])->GetVertices();
		 uint* indices = (uint*)((ComponentMesh*)meshes[i])->GetIndices();

		math::Triangle triangle;

		//Put the ray in local space of obj
		math::LineSegment segment_localized(line);
		float4x4 inverted_m = transf->GetGlobalTransform().Transposed().Inverted();
		segment_localized = inverted_m*segment_localized;


		for (int j = 0; j < ((ComponentMesh*)meshes[i])->GetNumIndices();)
		{


			/*triangle.a.x = vertices[indices[j]];
			triangle.a.y = vertices[indices[j] + 1];
			triangle.a.z = vertices[indices[j] + 2];

			triangle.b.x = vertices[indices[j + 1]];
			triangle.b.y = vertices[indices[j + 1] + 1];
			triangle.b.z = vertices[indices[j + 1] + 2];

			triangle.c.x = vertices[indices[j + 2]];
			triangle.c.y = vertices[indices[j + 2] + 1];
			triangle.c.z = vertices[indices[j + 2] + 2];*/

			//TODO: ????????????????????????????????????????????????????????????
			triangle.a.Set(&vertices[indices[j++] * 3]); 
			triangle.b.Set(&vertices[indices[j++] * 3]);
			triangle.c.Set(&vertices[indices[j++] * 3]);
			
			float tmp_distance;
			if (segment_localized.Intersects(triangle, &tmp_distance, nullptr))
			{
				if (tmp_distance < distance)
				{
					distance = tmp_distance;
					best_candidate = this;
				}
			}
		}

	}

	//Check against frustum if camera component
	ComponentCamera* cam = (ComponentCamera*)GetComponentByType(COMPONENT_CAMERA);
	if (cam)
	{	
		float tmp_distance, out;
		for (int i = 0; i < 6; i++)
		{
			if (line.Intersects(transformed_bounding_box, tmp_distance, out))
				if (tmp_distance < distance)
				{
					distance = tmp_distance;
					best_candidate = this;
					break;
				}
		}
	}
}
//-------------------------------------------------------------------


void GameObject::SendAllMeshesToDraw()
{
	for(std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->GetType() == COMPONENT_MESH)
			App->renderer3D->SetMeshToDraw((ComponentMesh*)(*it));
	}
}




//Editor draw--------------------------------------------------------------------------
void GameObject::OnEditor()
{
	ImGui::TextWrapped("%s", name.c_str());
	ImGui::Separator();
	ImGui::Checkbox("Static", &obj_static);

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->deleted == false) {
			ImGui::PushID((*it)->GetID());
			if (ImGui::CollapsingHeader((*it)->GetComponentNameByType(), "", true, true))
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
				ImGui::SameLine();
				if (ImGui::Button("Delete Component"))
				{
					(*it)->deleted = true;
					ImGui::PopID();
					break;
				}
				(*it)->OnEditor();
			}
			ImGui::PopID();
		}
	}
}
