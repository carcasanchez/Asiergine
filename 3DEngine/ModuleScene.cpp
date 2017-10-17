#include "ModuleScene.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "GameObject.h"
#include "ModuleInput.h"

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
	camera->CreateComponent_Camera(0.5, 5);
	camera->CreateComponent_Transform();

	return true;
}

update_status ModuleScene::Update(float dt)
{

	root->Update();


	//Check debug key
	if (App->input->GetKey(DEBUG_KEY) == KEY_DOWN)
	{
		debug_draw = !debug_draw;
	}



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
GameObject * ModuleScene::CreateGameObject(const char* object_name, GameObject* parent)
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

	return new_object;
}

