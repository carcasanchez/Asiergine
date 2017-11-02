#include "ModuleScene.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "ModuleFileSystem.h"

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

	GameObject* camera = CreateGameObject("Camera1", root);
	camera->CreateComponent_Camera(0.5, 5, true);
	camera->CreateComponent_Transform();
	
	return true;
}

update_status ModuleScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->importer->SaveSceneToOwnFormat("BakerHouse");
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->importer->LoadSceneFromOwnFormat("BakerHouse");

	root->Update();
	
	
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
GameObject * ModuleScene::CreateGameObject(const char* object_name, GameObject* parent, int16_t UID)
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

	if (UID >= 0)
	{
		new_object->SetID(UID);
	}

	return new_object;
}

void ModuleScene::ResetScene()
{
	
		CleanUp();
		root = CreateGameObject("Root");
		GameObject* camera = CreateGameObject("Camera1", root);
		camera->CreateComponent_Camera(0.5, 5, true);
		camera->CreateComponent_Transform();
		App->editor->UnselectAll();
}

