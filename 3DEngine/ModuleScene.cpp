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
	box = CreateGameObject("Box1", root);
	box1 = CreateGameObject("Box2", root);
	box2 = CreateGameObject("Box3", root);
	box3 = CreateGameObject("Box4", root);
	box4 = CreateGameObject("Box5", root);
	box5 = CreateGameObject("Box6", root);
	camera = CreateGameObject("Camera1", root);
	camera->CreateComponent_Camera(0.5, 5, true);
	camera->CreateComponent_Transform();
	scene_quadtree.root.Insert(box);
	scene_quadtree.root.Insert(box1);
	scene_quadtree.root.Insert(box2);
	scene_quadtree.root.Insert(box3);
	scene_quadtree.root.Insert(box4);
	scene_quadtree.root.Insert(box5);
	scene_quadtree.root.Insert(camera);
	scene_quadtree.root.Partition();
	return true;
}

update_status ModuleScene::Update(float dt)
{

	root->Update();
	/*if (root->GetChildrens().size() != NULL)
	{
		for (std::vector<GameObject*>::iterator it = root->GetChildrens().begin(); it != root->GetChildrens().end(); ++it)
		{
			if ((*it) != NULL)
				scene_quadtree.root.Insert((*it));
		}
	}*/	
	
	scene_quadtree.root.SetAABBToDraw();

	//Check debug key
	if (App->input->GetKey(DEBUG_NORMALS_KEY) == KEY_DOWN)
		debug_normals = !debug_normals;
	if (App->input->GetKey(DEBUG_BOXES_KEY) == KEY_DOWN)
		debug_boxes = !debug_boxes;
	
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->fs->LoadSceneFromOwnFormat("BakerHouse.carca");

	return UPDATE_CONTINUE;
}

bool ModuleScene::SaveConfig(JSON_Object * config_data)
{
	return true;
}

bool ModuleScene::SaveScene()
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

