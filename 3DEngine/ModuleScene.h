#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Globals.h"
#include "Module.h"
#include "QuadTree.h"

class GameObject;

class ModuleScene : public Module
{
public:

	ModuleScene(bool start_enabled = true);

	// Destructor
	virtual ~ModuleScene();

	bool Init(const JSON_Object* config_data);
	bool Start();
	update_status Update(float real_dt, float game_dt);
	bool CleanUp();

	GameObject* CreateGameObject(const char* name, GameObject* parent = nullptr, uint UID = 0);
	void DeleteSelectedGameObject();

	GameObject* root = nullptr;

	bool debug_normals = false;
	bool debug_boxes = false;

	QuadTreeObj scene_quadtree;

	bool wants_to_reset = false;
	bool wants_to_save = false;
	bool wants_to_load = false;

	//Cleans Scene
	void CleanScene();

	//Cleans and adds a camera to scene
	void ResetScene();

	//Scene serialization
	bool SaveSceneToOwnFormat(const char* name);
	void LoadSceneFromOwnFormat(const char* path);
	uint SaveGameObjectToOwnFormat(std::list<std::pair<char*, uint>>& buffer, GameObject* to_save);
	uint LoadObjectFromOwnFormat(char*& cursor);

	std::string scene_name = "AsiergineScene";
	

private:
	bool SaveConfig(JSON_Object* config_data);



	//Temporal storing of textures when loaded scene
	std::vector<std::pair <std::string, int>> loaded_textures;

	//Temporal storing of parent id's
	std::vector<uint> tmp_parent_ids;
};

#endif 