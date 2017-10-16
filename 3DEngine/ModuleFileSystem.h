#pragma once
#include "Module.h"

struct aiNode;
struct aiMesh;
struct aiScene;

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem();
	ModuleFileSystem(bool start_enabled);
	~ModuleFileSystem();

	bool Init(const JSON_Object* config_data);
	bool CleanUp();

	void LoadFile(const char* path);


private:

	
	bool LoadFBX(const char* path);
	GameObject* SearchNode(const aiNode* n, const aiScene* scene);

	GameObject* LoadNewObject(const aiNode* n);
	bool LoadGeometry(const aiMesh*, GameObject*);

	int SearchForTexture(const aiScene* scene, const char* path);
	GLuint LoadTexture(const char * path);


	int texture_id = 0;

};
