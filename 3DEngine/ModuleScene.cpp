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
	box6 = CreateGameObject("Box7", root);
	box7 = CreateGameObject("Box8", root);
	box8 = CreateGameObject("Box9", root);
	box9 = CreateGameObject("Box10", root);
	box10 = CreateGameObject("Box11", root);
	box11 = CreateGameObject("Box12", root);
	box12 = CreateGameObject("Box13", root);
	box13 = CreateGameObject("Box14", root);
	box14 = CreateGameObject("Box15", root);
	box15 = CreateGameObject("Box16", root);
	camera = CreateGameObject("Camera1", root);
	camera->CreateComponent_Camera(0.5, 5, true);
	camera->CreateComponent_Transform();
	box->bounding_box = math::AABB({ -1,-1,-1 }, { 1,1,1 });
	box1->bounding_box = math::AABB({ -1,-1,-1 }, { 1,1,1 });
	box2->bounding_box = math::AABB({ -1,-1,-1 }, { 1,1,1 });
	box3->bounding_box = math::AABB({ -1,-1,-1 }, { 1,1,1 });
	box4->bounding_box = math::AABB({ -1,-1,-1 }, { 1,1,1 });
	box5->bounding_box = math::AABB({ -1,-1,-1 }, { 1,1,1 });
	box6->bounding_box = math::AABB({ -1,-1,-1 }, { 1,1,1 });
	box7->bounding_box = math::AABB({ 5,5,5 }, { 9,9,9 });
	box8->bounding_box = math::AABB({ 5,5,5 }, { 9,9,9 });
	box9->bounding_box = math::AABB({ 5,5,5 }, { 9,9,9 });
	box10->bounding_box = math::AABB({ 5,5,5 }, { 9,9,9 });
	box11->bounding_box = math::AABB({ 5,5,5 }, { 9,9,9 });
	box12->bounding_box = math::AABB({ 5,5,5 }, { 9,9,9 });
	box13->bounding_box = math::AABB({ 5,5,5 }, { 9,9,9 });
	box14->bounding_box = math::AABB({ 5,5,5 }, { 9,9,9 });
	box15->bounding_box = math::AABB({ 5,5,5 }, { 9,9,9 });
	scene_quadtree.Calcutale();
	return true;
}

update_status ModuleScene::Update(float dt)
{

	root->Update();
	
	
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

