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

Resource* ModuleResourceManager::LoadResource(const char * path)
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
		ManageFBX(path);
	else if (extension.compare("carca") == 0)
		resource_id = ManageMesh(path);
	else LOG("ERROR: File extension '.%s' not allowed", extension.c_str());
		

	Resource* res = GetResource(resource_id);

	return res;
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
		resource_id = CreateMeshMeta(path);
		App->importer->LoadMeshFromOwnFormat(path, resource_id);
		//TODO: import to library
	}	
	else
	{
		//TODO:Delete this
		resource_id = CreateMeshMeta(path);
		App->importer->LoadMeshFromOwnFormat(path, resource_id);

		//TODO: Read UID from meta and check if it has been already loaded
		/*JSON_Value * value = json_parse_file(meta_file.c_str());
		JSON_Object* obj_data = json_value_get_object(value);
		resource_id = json_object_dotget_number(obj_data, "UID");*/
	}


	return resource_id;
}

uint ModuleResourceManager::CreateMeshMeta(const char * path)
{
	LCG rand;
	uint UID = rand.Int();



	//Extract file name
	std::string file_path = path;
	size_t begin_name = file_path.find_last_of('/');
	std::string file_name = file_path.substr(begin_name + 1);
	std::string meta_path = path;
	meta_path += META_EXTENSION;

	//Serialize to JSON
	JSON_Value * meta_file = json_value_init_object();
	JSON_Object* obj_data = json_value_get_object(meta_file);

	json_object_dotset_string(obj_data, "Resource Type:", "Mesh");
	json_object_dotset_string(obj_data, "Binary file:", file_name.c_str());


	json_object_dotset_number(obj_data, "UID", UID);

	std::time_t result = std::time(nullptr);
	std::string timestamp = std::asctime(std::localtime(&result));

	json_object_dotset_string(obj_data, "Time Stamp", timestamp.c_str());
	json_serialize_to_file(meta_file, meta_path.c_str());

	return UID;
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




