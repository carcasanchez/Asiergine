#include "ModuleScene.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "ModuleFileSystem.h"

#include "Component.h"
#include "ComponentMaterial.h"
#include "CompTransform.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentLight.h"

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
	root = nullptr;
	name = "scene";
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init(const JSON_Object * config_data)
{

	root = CreateGameObject("Root");
	root->SetID(0);
	
	return true;
}

bool ModuleScene::Start()
{
	GameObject* camera = CreateGameObject("Camera1", root);
	camera->CreateComponent_Camera(0.5, 5, true);
	camera->CreateComponent_Transform();

	GameObject* light = CreateGameObject("Lamp", root);
	light->CreateComponent_Transform(float3(5, 5, 5));
	light->CreateComponent_Light();



	return true;
}

update_status ModuleScene::Update(float real_dt, float game_dt)
{

	if (wants_to_reset)
	{
		wants_to_reset = false;
		ResetScene();
	}
	else if (wants_to_save)
	{
		wants_to_save = false;
		SaveSceneToOwnFormat(scene_name.c_str());
	}
	else if (wants_to_load)
	{
		wants_to_load = false;
		LoadSceneFromOwnFormat(scene_name.c_str());
	}

	//Delete GameObject
	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN && !App->editor->UsingGizmos())
	{
		DeleteSelectedGameObject();
	}

	root->Update(real_dt, game_dt);
	
	if(App->editor->BakeQuadtreeOpen())
		scene_quadtree.root.SetAABBToDraw();
		

	//Check debug key
	if (App->input->GetKey(DEBUG_NORMALS_KEY) == KEY_DOWN)
		debug_normals = !debug_normals;
	if (App->input->GetKey(DEBUG_BOXES_KEY) == KEY_DOWN)
		debug_boxes = !debug_boxes;
	
	return UPDATE_CONTINUE;
}

bool ModuleScene::SaveConfig(JSON_Object * config_data)
{
	return true;
}


bool ModuleScene::CleanUp()
{
	//Delete root and all its children
	root->CleanUp();
	delete root;
	root = nullptr;
	return true;
}


//Game Object related Utility-----------------------------
GameObject * ModuleScene::CreateGameObject(const char* object_name, GameObject* parent, uint UID)
{
	GameObject* new_object = new GameObject(object_name);

	if (parent == nullptr && root != nullptr)
	{
		new_object->SetParent(root);
	}
	
	else if (parent)
	{
		new_object->SetParent(parent);
	}

	if (UID > 0)
	{
		new_object->SetID(UID);
	}

	return new_object;
}

void ModuleScene::DeleteSelectedGameObject()
{
	GameObject* del_game_object = App->editor->GetSelectedGameObject();

	if (del_game_object && del_game_object->GetParent())
	{
		del_game_object->GetParent()->EraseChild(del_game_object);
		del_game_object->CleanUp();
		delete del_game_object;
		App->editor->UnselectAll();
		App->editor->UnLockSelection();
	}
}

void ModuleScene::CleanScene()
{
	CleanUp();
	App->resource_m->CleanUp();
	scene_quadtree.Clear();
	root = CreateGameObject("Root");	
	root->SetID(0);
	App->editor->UnselectAll();
}

void ModuleScene::ResetScene()
{
		CleanScene();
		GameObject* camera = CreateGameObject("Camera1", root);
		camera->CreateComponent_Camera(0.5, 5, true);
		camera->CreateComponent_Transform();
		App->camera->ResetCamera();
}


//Scene serialization-------------------------------------------------

bool ModuleScene::SaveSceneToOwnFormat(const char* name)
{
	//DATA ORDER: tag - num objects - [object]

	bool ret = true;

	uint size = sizeof(uint) * 2;

	std::list<std::pair<char*, uint>> object_buffers;
	std::vector<GameObject*> objects_to_save = App->scene->root->GetChildrens();


	//Saves all object data to buffers
	for (int i = 0; i < objects_to_save.size(); i++)
	{
		size += SaveGameObjectToOwnFormat(object_buffers, objects_to_save[i]);
	}


	//Save tag
	char* data = new char[size];
	char* cursor = data;
	uint size_of = sizeof(uint);
	uint tag = SCENE_SAVETAG;

	memcpy(cursor, &tag, size_of);
	cursor += size_of;

	//Save num objs
	uint num_objs = object_buffers.size();
	memcpy(cursor, &num_objs, size_of);
	cursor += size_of;


	//Stores all object data
	for (std::list<std::pair<char*, uint>>::iterator it = object_buffers.begin(); it != object_buffers.end(); it++)
	{
		size_of = (*it).second;
		memcpy(cursor, (*it).first, size_of);
		cursor += size_of;

		delete[](*it).first;
	}


	std::string scenes_dir = App->fs->CreateDirectoryInAssets("Scenes");
	scenes_dir += name;
	scenes_dir += FORMAT_EXTENSION;
	App->fs->SaveDataTo(data, size, scenes_dir.c_str());
	

	delete[] data;
	LOG("Saved Scene %s to Library", name);

	return ret;
}

uint ModuleScene::SaveGameObjectToOwnFormat(std::list<std::pair<char*, uint>> &buffer, GameObject* to_save)
{
	//DATA ORDER: UID of obj - size of name - name - static -
	// UID of parent
	//  UID of transform - pos - scale - rot 
	//  num of meshes - [UID of mesh - mesh name size - mesh name]
	//  UID of material - size of texture name - texture name
	//UID of camera - near dist - far dist - active
	//UID of light

	//STORE SIZE-------------------------------------------------------------------
	uint size = 1;

	std::vector<Component*> meshes = to_save->GetAllComponentOfType(COMPONENT_MESH);

	size += sizeof(uint);
	std::string name = to_save->GetName();
	size += sizeof(uint);
	size += name.size();

	//Static option
	size += sizeof(bool);

	//UID parent
	size += sizeof(uint);

	//Transform
	CompTransform* transform = ((CompTransform*)to_save->GetComponentByType(COMPONENT_TRANSFORM));
	size += sizeof(uint);
	if (transform != nullptr)
	{
		size += transform->PrepareToSave();
	}

	//Meshes
	size += sizeof(uint); //num of meshes
	for (int i = 0; i < meshes.size(); i++)
	{
		size += meshes[i]->PrepareToSave();
	}

	//Material
	size += sizeof(uint);
	ComponentMaterial* mat = (ComponentMaterial*)to_save->GetComponentByType(COMPONENT_MATERIAL);
	if (mat != nullptr)
	{
		size += mat->PrepareToSave();
	}

	//Camera
	size += sizeof(uint);
	ComponentCamera* cam = (ComponentCamera*)to_save->GetComponentByType(COMPONENT_CAMERA);
	if (cam != nullptr)
	{
		size += cam->PrepareToSave();
	}

	//Light
	size += sizeof(uint);

	//COPY DATA------------------------------------------------------
	char* data = new char[size];
	char* cursor = data;
	uint size_of = 0;


	//Copy UID
	size_of = sizeof(uint);
	uint objID = to_save->GetID();
	memcpy(cursor, &objID, size_of);
	cursor += size_of;

	//Copy name size
	size_of = sizeof(uint);
	uint name_size[] = { -1 };
	name_size[0] = name.size();
	memcpy(cursor, name_size, size_of);
	cursor += size_of;

	//Copy name
	size_of = name_size[0];
	memcpy(cursor, name.data(), size_of);
	cursor += size_of;

	//Copy static option
	size_of = sizeof(bool);
	bool static_option = to_save->IsStatic();
	memcpy(cursor, &static_option, size_of);
	cursor += size_of;

	//Copy UID of parent
	uint parentID = to_save->GetParent()->GetID();
	size_of = sizeof(uint);
	memcpy(cursor, &parentID, size_of);
	cursor += size_of;

	//Copy UID of transform

	uint transformID = 0;
	if (transform != nullptr)
		transformID = transform->GetID();
	size_of = sizeof(uint);
	memcpy(cursor, &transformID, size_of);
	cursor += size_of;

	//Copy transform
	if (transform != nullptr)
	{
		transform->Save(cursor);
	}



	//Copy meshes
	uint num_of_meshes = meshes.size();
	size_of = sizeof(uint);
	memcpy(cursor, &num_of_meshes, size_of);
	cursor += size_of;

	for (int i = 0; i < num_of_meshes; i++)
	{
		meshes[i]->Save(cursor);
	}


	//copy material & texture
	uint matID = 0;
	if (mat != nullptr)
		matID = mat->GetID();

	size_of = sizeof(uint);
	memcpy(cursor, &matID, size_of);
	cursor += size_of;

	if (mat != nullptr)
	{
		mat->Save(cursor);
	}

	//copy cam settings
	uint camID = 0;
	if (cam != nullptr)
		camID = cam->GetID();

	size_of = sizeof(uint);
	memcpy(cursor, &camID, size_of);
	cursor += size_of;

	if (cam != nullptr)
	{
		cam->Save(cursor);
	}

	//Copy light
	ComponentLight* light = (ComponentLight*)to_save->GetComponentByType(COMPONENT_LIGHT);
	uint lightID = 0;
	if (light != nullptr)
		lightID = light->GetID();

	size_of = sizeof(uint);
	memcpy(cursor, &lightID, size_of);
	cursor += size_of;

	data[size - 1] = '\0';
	std::pair<char*, uint> pair_of_data;
	buffer.push_back(pair_of_data);
	buffer.back().first = data;
	buffer.back().second = size;

	//Load children 
	std::vector<GameObject*> children = to_save->GetChildrens();
	for (int i = 0; i < children.size(); i++)
	{
		size += SaveGameObjectToOwnFormat(buffer, children[i]);
	}

	return size;
}



GameObject * ModuleScene::LoadSceneFromOwnFormat(const char * name)
{

	char* data = nullptr;

	std::string data_path = App->fs->GetAssetDirectory();
	data_path += "Scenes/";
	data_path += name;
	data_path += FORMAT_EXTENSION;

	if (App->fs->LoadDataFrom(data, data_path.c_str()) == false)
	{
		return nullptr;
	}

	tmp_parent_ids.clear();
	loaded_textures.clear();

	//Clean Current Scene
	App->scene->CleanScene();

	//Get data from buffer---------------
	//DATA ORDER: tag - num objects - [object]

	//Get tag and check that its a scene
	char* cursor = data;
	uint tag[] = { -1 };
	uint size_of = sizeof(uint);
	memcpy(tag, cursor, size_of);

	if (*tag != SCENE_SAVETAG)
	{
		LOG("ERROR: this is not a scene");
		return nullptr;
	}
	cursor += size_of;

	//Load Num of OBJ
	uint num_objects[] = { -1 };
	size_of = sizeof(uint);
	memcpy(num_objects, cursor, size_of);
	cursor += size_of;

	//Load all obj 
	for (int i = 0; i < num_objects[0]; i++)
	{
		LoadObjectFromOwnFormat(cursor);
	}

	//Set all parents properly
	std::vector<GameObject*> objects_in_scene = App->scene->root->GetChildrens();
	for (int i = 0; i < objects_in_scene.size(); i++)
	{
		if (tmp_parent_ids[i] == 0)
			continue;


		for (int j = 0; j < objects_in_scene.size(); j++)
		{
			if (objects_in_scene[j] == objects_in_scene[i])
				continue;

			GameObject* parent = objects_in_scene[j]->FindChildByID(tmp_parent_ids[i]);

			if (parent != nullptr)
			{
				objects_in_scene[i]->SetParent(parent);
				break;
			}
		}
	}


	delete[] data;
	LOG("Loading scene %s", name);

	return nullptr;
}

uint ModuleScene::LoadObjectFromOwnFormat(char*& cursor)
{
	//DATA ORDER: UID of obj - size of name - name - static -
	// UID of parent
	//  UID of transform - pos - scale - rot 
	//  num of meshes - [UID of mesh - mesh name size - mesh name]
	//  UID of material - size of texture name - texture name
	// UID of camera - near distance - far distance
	//UID of light
	uint object_id = 0;

	uint size_of = sizeof(uint);
	memcpy(&object_id, cursor, size_of);
	cursor += size_of;

	uint size_of_name;
	size_of = sizeof(uint);
	memcpy(&size_of_name, cursor, size_of);
	cursor += size_of;

	char* obj_name = new char[size_of_name + 1];

	size_of = size_of_name;
	memcpy(obj_name, cursor, size_of);
	cursor += size_of;
	obj_name[size_of_name] = '\0';


	GameObject* new_obj = App->scene->CreateGameObject(obj_name, App->scene->root, object_id);

	delete[] obj_name;

	bool static_option;
	size_of = sizeof(bool);
	memcpy(&static_option, cursor, size_of);
	cursor += size_of;
	new_obj->SetStatic(static_option);

	uint parent_id = 0;
	size_of = sizeof(uint);
	memcpy(&parent_id, cursor, size_of);
	cursor += size_of;
	tmp_parent_ids.push_back(parent_id);


	uint transform_id = 0;
	size_of = sizeof(uint);
	memcpy(&transform_id, cursor, size_of);
	cursor += size_of;


	if (transform_id != 0)
	{
		float pos[] = { 0, 0, 0 };
		size_of = sizeof(float) * 3;
		memcpy(pos, cursor, size_of);
		cursor += size_of;

		float scale[] = { 1, 1, 1 };
		size_of = sizeof(float) * 3;
		memcpy(scale, cursor, size_of);
		cursor += size_of;

		float rot[] = { 0, 0, 0 , 1 };
		size_of = sizeof(float) * 4;
		memcpy(rot, cursor, size_of);
		cursor += size_of;

		new_obj->CreateComponent_Transform(float3(pos[0], pos[1], pos[2]), float3(scale[0], scale[1], scale[2]), Quat(rot[0], rot[1], rot[2], rot[3]), transform_id);
	}


	uint num_meshes = 0;
	size_of = sizeof(uint);
	memcpy(&num_meshes, cursor, size_of);
	cursor += size_of;

	//[UID of mesh - mesh name size - mesh name]
	for (int i = 0; i<num_meshes; i++)
	{
		uint mesh_id = 0;
		size_of = sizeof(uint);
		memcpy(&mesh_id, cursor, size_of);
		cursor += size_of;

		uint mesh_name_size = 0;
		size_of = sizeof(uint);
		memcpy(&mesh_name_size, cursor, size_of);
		cursor += size_of;

		char* mesh_name = new char[mesh_name_size + 1];

		size_of = mesh_name_size;
		memcpy(mesh_name, cursor, size_of);
		cursor += size_of;

		mesh_name[mesh_name_size] = '\0';

		std::string mesh_path = App->fs->GetAssetDirectory();
		mesh_path += "Meshes/";
		mesh_path += mesh_name;
		mesh_path += FORMAT_EXTENSION;

		new_obj->CreateComponent_Mesh(mesh_name, (ResourceMesh*)App->resource_m->LoadResource(mesh_path.c_str()));
		delete[] mesh_name;
	}

	//UID of material - size of texture name - texture name
	uint mat_id = 0;
	size_of = sizeof(uint);
	memcpy(&mat_id, cursor, size_of);
	cursor += size_of;

	if (mat_id != 0)
	{
		uint text_name_size = 0;
		size_of = sizeof(uint);
		memcpy(&text_name_size, cursor, size_of);
		cursor += size_of;

		char* text_name = new char[text_name_size + 1];

		size_of = text_name_size;
		memcpy(text_name, cursor, size_of);
		cursor += size_of;

		text_name[text_name_size] = '\0';
		std::string texture_path = App->fs->GetAssetDirectory() + "Textures/";
		texture_path +=  text_name;
		ResourceTexture* t = (ResourceTexture*)App->resource_m->LoadResource(texture_path.c_str());
		new_obj->CreateComponent_Material(t);

		delete[] text_name;
	}

	//UID of camera - near dist - far dist
	uint cam_id = 0;
	size_of = sizeof(uint);
	memcpy(&cam_id, cursor, size_of);
	cursor += size_of;

	if (cam_id != 0)
	{
		float n_dist = 0, f_dist = 0;
		bool active = false;

		size_of = sizeof(float);

		memcpy(&n_dist, cursor, size_of);
		cursor += size_of;
		memcpy(&f_dist, cursor, size_of);
		cursor += size_of;
		size_of = sizeof(bool);
		memcpy(&active, cursor, size_of);
		cursor += size_of;

		new_obj->CreateComponent_Camera(n_dist, f_dist, active, cam_id);
	}

	//Light
	uint light_id = 0;
	size_of = sizeof(uint);
	memcpy(&light_id, cursor, size_of);
	cursor += size_of;

	if (light_id != 0)
	{
		new_obj->CreateComponent_Light(light_id);
	}

	cursor++;
	return 0;
}


