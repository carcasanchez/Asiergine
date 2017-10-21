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
	
	std::string  SearchNode(const aiNode* n, const aiScene* scene);
	std::string ImportGeometry(const aiMesh*, const char*);

	int SearchForTexture(const aiScene* scene, const char* path);
	GLuint LoadTexture(const char * path);


	int texture_id = 0;

	//For meshes without name
	int mesh_id = 0;
};
