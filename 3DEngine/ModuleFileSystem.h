#pragma once
#include "Module.h"
#include "Geometry.h"

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
	bool Start();
	bool CleanUp();

	void LoadFile(const char* path);
	void UnloadGeometry();
	
	int GetFirstTextureId()
	{
		return first_texture_id;
	}
	float2 GetFirstTextureDim()
	{
		return first_texture_dimensions;
	}


	std::vector<Geometry*> geometries;

private:

	
	bool LoadFBX(const char* path);
	void SearchNode(const aiNode* n, const aiScene* scene);


	bool LoadGeometry(const aiMesh*, const aiScene*, const aiNode*);

	int SearchForTexture(const aiScene* scene, const char* path);
	GLuint LoadTexture(const char * path);

	int first_texture_id = 0;
	math::float2 first_texture_dimensions;

};
