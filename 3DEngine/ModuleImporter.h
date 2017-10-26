#pragma once
#include "Module.h"

struct aiNode;
struct aiMesh;
struct aiScene;


class ModuleImporter : public Module
{
public:
	ModuleImporter();
	ModuleImporter(bool start_enabled);
	~ModuleImporter();

	bool Init(const JSON_Object* config_data);
	bool CleanUp();

	void LoadFile(const char* path);


private:

	
	bool LoadFBX(const char* path);


	void ImportScene(const aiScene* scene);
	std::string  SearchNode(const aiNode* n, const aiScene* scene, GameObject* parent);
	std::string ImportGeometry(const aiMesh*, const char*);

	int SearchForTexture(const aiScene* scene, const char* path, int material_index);
	GLuint LoadTexture(const char * path);


	//Temporal storing of FBX file path
	std::string fbx_path;	

	//For meshes without name
	int mesh_id = 0;
};
