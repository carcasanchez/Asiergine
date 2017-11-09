#include "Globals.h"
#include "Application.h"
#include "ModuleResourceManager.h"

#include "mmgr\mmgr.h"

#include "ResourceTexture.h"
#include "ResourceMesh.h"

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

Resource * ModuleResourceManager::CreateResource(Resource::RESOURCE_TYPE type)
{
	Resource* ret = nullptr;
	LCG rand_gen;
	uint UID = rand_gen.Int();
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

void ModuleResourceManager::CheckFileStatus(const char * path)
{
	std::string meta_path = path;	
	meta_path += META_EXTENSION;

	//Check if the file has meta 	
	//--IF NOT: Import file to library, create meta file and load in memory
	if (!App->fs->ExistsFile(meta_path.c_str()))
	{
		LoadFile(path);
	}
	

		//--IF HAS: Check if its already loaded in memory
				//-IF HAS: return the resource ID and skip load
				//-IF NOT: load the file from library using the meta data
		
}

void ModuleResourceManager::LoadFile(const char* path)
{
	std::string tmp = path;
	std::string extension;

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

	//Load file depending on extension
	if (extension.compare("fbx") == 0)
	{
		App->importer->LoadFBX(path);
	}


	else LOG("ERROR: File extension '.%s' not allowed", extension.c_str());
}




