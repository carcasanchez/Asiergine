#include "GameObject.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "Component.h"
#include "CompTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ComponentLight.h"

#include "ComponentMovement.h"
#include "MathGeoLib\include\Algorithm\Random\LCG.h"

GameObject::GameObject(const char* name): name(name)
{
	LCG rand_gen;
	UID = rand_gen.Int();
	this->name.reserve(31);
}

GameObject::~GameObject()
{
}

void GameObject::Update(float real_dt, float game_dt)
{
	//Delete the selected component------
	std::vector<Component*>::iterator del_comp = components.begin();
	for (del_comp; del_comp != components.end(); ++del_comp)
	{
		if ((*del_comp)->deleted)
		{
			(*del_comp)->deleted == false;
			components.erase(del_comp);
			break;
		}
	}
	//-----------------------------------


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
	

}

void GameObject::EraseChild(GameObject* game_object)
{
	if (game_object)
	{
		for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		{
			if (game_object == (*it))
			{
				children.erase(it);
				break;
			}
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

ComponentMesh * GameObject::CreateComponent_Mesh(const char* m_name, ResourceMesh* m, uint UID)
{
	ComponentMesh* new_mesh = new ComponentMesh(this);
	if (m)
	{
		new_mesh->name = m_name;
		new_mesh->SetMesh(m);
	}
	if (UID > 0)
		new_mesh->SetID(UID);

	components.push_back(new_mesh);
	LOG("Creating new Mesh with %i vertices in %s", new_mesh->GetNumVertices(), name.c_str());
	return new_mesh;
}

ComponentMaterial * GameObject::CreateComponent_Material(ResourceTexture* t, uint UID)
{
	if (GetComponentByType(COMPONENT_MATERIAL) != nullptr)
	{
		LOG("ERROR: GameObject %s already has a material component!", name.c_str());
		return nullptr;
	}

	ComponentMaterial* new_mat = new ComponentMaterial(this);
	
	new_mat->SetTexture(t);

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

	components.push_back(new_camera);
	LOG("Creating new Camera in %s", name.c_str());

	return new_camera;
}

ComponentLight * GameObject::CreateComponent_Light(uint UID)
{
	Light l;
	l.ref = GL_LIGHT0;
	App->renderer3D->light_index++;
	l.ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
	l.diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
	l.Init();
	l.Active(true);
	ComponentLight* new_light = new ComponentLight(this, l);

	if (UID > 0)
		new_light->SetID(UID);
	
	components.push_back(new_light);
	return new_light;
}

ComponentAudio * GameObject::CreateComponent_Audio(uint UID, AUDIO_TYPE t)
{

	if (GetComponentByType(COMPONENT_AUDIO) != nullptr)
	{
		LOG("ERROR: GameObject %s already has an audio component!", name.c_str());
		return nullptr;
	}

	ComponentAudio* ret = nullptr;
	ComponentAudio* new_audio = new ComponentAudio(this);

	if (UID > 0)
		new_audio->SetID(UID);

	new_audio->SetAudioType(t);

	components.push_back(new_audio);

	return ret;
}

ComponentMovement * GameObject::CreateComponent_Movement(uint UID)
{
	if (GetComponentByType(COMPONENT_MOVEMENT) != nullptr)
	{
		LOG("ERROR: GameObject %s already has a movement component!", name.c_str());
		return nullptr;
	}

	ComponentMovement* new_mov = new ComponentMovement(this);

	if (UID > 0)
		new_mov->SetID(UID);

	components.push_back(new_mov);

	return new_mov;
}

//Recursive methods-------------------------------------------------------------------
bool GameObject::PutInQuadTree(QuadTreeNodeObj* node)
{
	bool ret = true;

	
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
			ret = (*it)->PutInQuadTree(node);
			if (ret == false && !node->IsOfMinSize())
			{
				break;
			}
	}


	if (!IsStatic())
		return ret;

	std::vector<Component*> meshes = GetAllComponentOfType(COMPONENT_MESH);
	for (int i = 0; i < meshes.size(); i++)
	{
		if (node->box.Intersects(*((ComponentMesh*)meshes[i])->GetTransformedBox()) && ret)
		{
			node->Insert(this);
			if (node->IsFull() && !node->IsOfMinSize())
				ret = false;
			break;
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

		if (l.Intersects(GetBoundingBox()))
		{
			children[i]->CheckTriangleCollision(l, distance, best_candidate);
		}

		children[i]->CheckMouseRayCollision(l, distance, best_candidate);
	}

}


AABB GameObject::GetBoundingBox()
{
	std::vector<Component*> meshes = GetAllComponentOfType(COMPONENT_MESH);
	ComponentCamera* cam = (ComponentCamera*)GetComponentByType(COMPONENT_CAMERA);
	ComponentAudio* audio = (ComponentAudio*)GetComponentByType(COMPONENT_AUDIO);
	math::AABB total_aabb;
	total_aabb.SetNegativeInfinity();

	for (int i = 0; i < meshes.size(); i++)
	{
		total_aabb.Enclose(*((ComponentMesh*)meshes[i])->GetTransformedBox());
	}
	if (cam)
	{
		total_aabb.Enclose(*cam->GetBox());
	}
	if (audio)
	{
		total_aabb.Enclose(audio->GetBox());
	}

	return total_aabb;
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
			if (line.Intersects(*cam->GetBox(), tmp_distance, out))
				if (tmp_distance < distance)
				{
					distance = tmp_distance;
					best_candidate = this;
					break;
				}
		}
	}

	ComponentAudio* audio = (ComponentAudio*)GetComponentByType(COMPONENT_AUDIO);
	if (audio)
	{
		float tmp_distance, out;
		for (int i = 0; i < 6; i++)
		{
			if (line.Intersects(audio->GetBox(), tmp_distance, out))
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

void GameObject::SetStatic(bool is_static)
{
	std::vector<GameObject*>::iterator it = children.begin();
	for (it; it != children.end(); ++it)
	{
		(*it)->SetStatic(is_static);
	}
	obj_static = is_static;
}


//Editor draw--------------------------------------------------------------------------
void GameObject::OnEditor()
{
	ImGui::InputText("", (char*)name.c_str(), 30);

	ImGui::SameLine();
	if (ImGui::Checkbox("Static", &obj_static))
	{
		//GetComponentByType(COMPONENT_TRANSFORM)->SetActive(!obj_static);
		SetStatic(obj_static);
		App->scene->scene_quadtree.Calculate();
	}

	ImGui::Separator();

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
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

			//ComponentTransform can't be deleted!
			if ((*it)->GetType() != COMPONENT_TRANSFORM)
			{
				ImGui::SameLine();
				if (ImGui::Button("Delete Component"))
				{
					(*it)->deleted = true;
					ImGui::PopID();
					break;
				}
			}

			ImGui::Separator();
			(*it)->OnEditor();
		}
		ImGui::PopID();
	}

	ImGui::Separator();
	ImGui::Separator();

	if (ImGui::Button("CREATE COMPONENT"))	
		ImGui::OpenPopup("CREATE COMPONENT");

	if (ImGui::BeginPopup("CREATE COMPONENT"))
	{
		if (ImGui::MenuItem("Camera"))
		{
			CreateComponent_Camera(0.5f, 5.0f, true);
		}
		if (ImGui::MenuItem("Material"))
		{
			CreateComponent_Material();
		}

		if (ImGui::MenuItem("Mesh"))
		{
			CreateComponent_Mesh("Mesh");
		}

		if (ImGui::MenuItem("Light"))
		{
			CreateComponent_Light();
		}
		if (ImGui::MenuItem("Sound Emitter"))
		{
			CreateComponent_Audio();
		}
		if (ImGui::MenuItem("Movement"))
		{
			CreateComponent_Movement();
		}


		ImGui::EndPopup();
	}
	
}
