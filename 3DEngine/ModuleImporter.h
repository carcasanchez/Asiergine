#pragma once
#include "Module.h"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

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
	GLuint LoadTextureFromFBX(const char * path, const char * name) const;
	void ImportTextureToDDS(const char * name) const;

	//Save methodology
	bool SaveMeshToOwnFormat(const char* name, uint, uint, const float* vert, uint* ind, const float* normals = nullptr, const float* texture_coords = nullptr) const;
	void LoadMeshFromOwnFormat(const char* path, GameObject* obj)const;


	//Temporal storing of FBX file path
	std::string fbx_path;	

	//Temporal storing of materials to avoid double loading
	std::vector <std::pair <aiMaterial*, int>> materials;
	//if id == -1, still not loaded
	
	//For meshes without name
	int mesh_id = 0;
};
