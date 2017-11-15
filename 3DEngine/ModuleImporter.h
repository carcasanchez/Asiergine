#pragma once
#include "Module.h"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
class ResourceMesh;

struct FBX_data
{
	std::vector<std::string> mesh_names;
	std::vector<std::string> texture_names;
};


class ModuleImporter : public Module
{
public:
	ModuleImporter();
	ModuleImporter(bool start_enabled);
	~ModuleImporter();

	bool Init(const JSON_Object* config_data);
	bool CleanUp();
	


	uint LoadTexture(const char * path, bool flip = false) const;


	bool ImportFBX(const char* path);
	bool LoadFBX(const char* path);


	//Import from FBX-------------------------
	void ImportScene(const char* path, const aiScene* scene, FBX_data&);
	int ImportTextureFromFBX(const aiScene* scene, const char* path, int material_index, FBX_data&);
	std::string ImportMeshFromFBX(const aiMesh*, const char*, FBX_data&);
	void CreateFBXmeta(FBX_data& d, const char* path) const;


	//Save resources to library after import
	void SaveTextureToDDS(const char * name) const;

	//Load hierarchy from fbx
	std::string  SearchFBXNode(const aiNode* n, const aiScene* scene, GameObject* parent, const char* scene_name);
	std::string ExtractTexturePath(const aiMaterial* material);

	//Save methodology
	bool SaveMeshToOwnFormat(const char* path, ResourceMesh* m) const;
	ResourceMesh* LoadMeshFromOwnFormat(const char * path, uint UID)const;
	
		
private:
	//For meshes without name
	int mesh_id = 0;
	//For texture referencing
	std::string fbx_path;
};
