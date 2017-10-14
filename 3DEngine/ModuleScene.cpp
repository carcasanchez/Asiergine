#include "ModuleScene.h"
#include "GameObject.h"


ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
	name = "scene";
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init(const JSON_Object * config_data)
{
	return true;
}

update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleScene::SaveConfig(JSON_Object * config_data)
{
	return true;
}

bool ModuleScene::CleanUp()
{
	return true;
}

