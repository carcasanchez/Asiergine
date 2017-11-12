#include "Globals.h"
#include "Application.h"
#include "ModuleResourceManager.h"

#include "mmgr\mmgr.h"

#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "parson.h"

ModuleResourceManager::ModuleResourceManager()
{
	name = "resource manager";
}


ModuleResourceManager::~ModuleResourceManager()
{
}


bool ModuleResourceManager::Init(const JSON_Object* config_data)
{
	bool ret = true;

	return ret;
}

bool ModuleResourceManager::CleanUp()
{
	for (std::map<uint, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		delete it->second;
	}

	return true;
}

Resource * ModuleResourceManager::GetResource(uint id)
{
	Resource* res = nullptr;
	if(resources.count(id) >0)
			res = resources[id];

	return res;
}

Resource * ModuleResourceManager::CreateResource(Resource::RESOURCE_TYPE type, uint id)
{
	Resource* ret = nullptr;
	LCG rand_gen;
	uint UID;
	if(id == 0)
		UID = rand_gen.Int();
	else UID = id;
	
	switch (type) 
	{
		case Resource::TEXTURE: ret = (Resource*) new ResourceTexture(UID);
			break;
		case Resource::MESH: ret = (Resource*) new ResourceMesh(UID);
			break;
	}

	if (ret != nullptr)
		resources[UID] = ret;
	
	return ret;
}

uint ModuleResourceManager::LoadResource(const char * path)
{
	std::string tmp = path;
	std::string extension;

	uint resource_id = 0;

	LOG("-----------------------Loading file from %s", path);

	//Get file extension (reversed)
	while (tmp.back() != '.')
	{
		extension.push_back(tmp.back());
		tmp.pop_back();

		if (tmp.empty())
			break;
	}

	//Normalize to lower case
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	//Reverse to be human readable
	std::reverse(extension.begin(), extension.end());

	//Manage file depending on extension
	if (extension.compare("fbx") == 0)
	{
		ManageFBX(path);
	}
	else LOG("ERROR: File extension '.%s' not allowed", extension.c_str());
		
	return resource_id;
}

void ModuleResourceManager::ManageFBX(const char* path)
{
	App->importer->ImportFBX(path);	
	App->importer->LoadFBX(path);
}

uint ModuleResourceManager::ManageMesh(const char * path)
{

	uint resource_id = 0;
	std::string meta_file = path;
	meta_file += META_EXTENSION;

	//Check if meta exists

	//IF NOT: create meta
	if (!App->fs->ExistsFile(meta_file.c_str()))
	{

	}

	return resource_id;
}

uint ModuleResourceManager::CreateMeshMeta(const char * path)
{

	return uint();
}



//Utility
bool ModuleResourceManager::CheckTimestamp(const char * path)
{
	JSON_Value* meta_file = json_parse_file(path);

	if (meta_file == nullptr)
	{
		LOG("Error: could not open %s", path);
		return 0;
	}

	//Extract timestamp from meta
	JSON_Object * object_data = json_value_get_object(meta_file);
	std::string timestamp = json_object_dotget_string(object_data, "Time Stamp");

	//Check last modification of file
	struct stat st;
	stat(path, &st);
	std::string last_modified_date = std::asctime(std::localtime(&st.st_mtime));

	return timestamp.compare(last_modified_date);
}




