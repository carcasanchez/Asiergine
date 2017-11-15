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

update_status ModuleResourceManager::PreUpdate(float real_dt, float game_dt)
{
	if (!to_delete.empty())
	{
		//delete all marked resources
		for (int i = 0; i < to_delete.size(); i++)
		{
			DeleteResource(to_delete[i]);
		}
		to_delete.clear();
	}

	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	for (std::map<uint, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		delete it->second;
	}
	resources.clear();
	to_delete.clear();

	return true;
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
	else if (extension.compare("png") == 0 || extension.compare("dds") == 0 || extension.compare("tga") == 0 || extension.compare("jpg") == 0)
		resource_id = ManageTexture(path, extension.c_str());
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

	if (!App->fs->ExistsFile(path))
	{
		return resource_id;
	}

	std::string meta_file = path;
	meta_file += META_EXTENSION;

	//Check if meta exists

	//IF NOT: create meta and import to library
	if (!App->fs->ExistsFile(meta_file.c_str()))
	{
		resource_id = CreateMeshMeta(path);
		
		//Extract file name
		std::string file_name = std::experimental::filesystem::path(path).stem().string().c_str();

		//Construct path to library
		std::string library_path = App->fs->GetLibraryDirectory();
		library_path += App->fs->CreateDirectoryInLibrary("Meshes") + file_name + FORMAT_EXTENSION;
		
		ResourceMesh* new_mesh = App->importer->LoadMeshFromOwnFormat(path, resource_id);

		if (new_mesh)
		{	
			//Import mesh to library
			App->importer->SaveMeshToOwnFormat(library_path.c_str(), new_mesh);
			new_mesh->SetFile(path, library_path.c_str());
		}
	}	
	else
	{		
		//TODO: check if meta timestamp doesn't match
				//IF HASN'T: reimport asset

		//Read UID from meta 
		JSON_Value * value = json_parse_file(meta_file.c_str());
		JSON_Object* obj_data = json_value_get_object(value);
		resource_id = json_object_dotget_number(obj_data, "UID");

		//Chek if mesh has been already loaded
		Resource* new_mesh = GetResource(resource_id);
		
		//IF HASN'T: load asset from library		
		if (new_mesh == nullptr)
		{
			//Extract file name
			std::string file_name = std::experimental::filesystem::path(path).stem().string().c_str();

			//Construct path to library
			std::string library_path = App->fs->GetLibraryDirectory();
			library_path += "Meshes/" + file_name + FORMAT_EXTENSION;
			new_mesh = App->importer->LoadMeshFromOwnFormat(library_path.c_str(), resource_id);
			if(new_mesh)			
				new_mesh->SetFile(path, library_path.c_str());
			else
			{
				LOG("Mesh %s missing from library!", file_name.c_str());
				resource_id = 0;
			}
		}

	}


	return resource_id;
}

uint ModuleResourceManager::ManageTexture(const char * path, const char* image_extension)
{
	uint resource_id = 0;

	if (!App->fs->ExistsFile(path))
	{
		return resource_id;
	}


	std::string meta_file = path;
	meta_file += META_EXTENSION;

	//Check if meta exists

	//IF NOT: create meta and import to library
	if (!App->fs->ExistsFile(meta_file.c_str()))
	{			
		resource_id = CreateTextureMeta(path);

		//Construct path to library
		std::string library_path = App->fs->CreateDirectoryInLibrary("Textures") + std::experimental::filesystem::path(path).stem().string().c_str() + TEXTURE_EXTENSION;
		
		ResourceTexture* new_texture = (ResourceTexture*)CreateResource(Resource::TEXTURE, resource_id);

		std::string texture_name = std::experimental::filesystem::path(path).stem().string().c_str();
		
		uint texture_id = App->importer->LoadTexture(path, true);
		if (texture_id != 0)
		{
			new_texture->SetData(App->importer->LoadTexture(path, true), (texture_name + image_extension).c_str());
			App->importer->SaveTextureToDDS(texture_name.c_str());
			new_texture->SetFile(path, library_path.c_str());
		}
	}
	else
	{
		//TODO: check if meta timestamp doesn't match
		//IF HASN'T: reimport asset

		//Read UID from meta 
		JSON_Value * value = json_parse_file(meta_file.c_str());
		JSON_Object* obj_data = json_value_get_object(value);
		resource_id = json_object_dotget_number(obj_data, "UID");

		//Chek if mesh has been already loaded
		ResourceTexture* new_texture = (ResourceTexture*)GetResource(resource_id);

		//IF HASN'T: load asset from library		
		if (new_texture == nullptr)
		{
			new_texture = (ResourceTexture*)CreateResource(Resource::TEXTURE, resource_id);
			
			//Extract file name
			std::string file_name = std::experimental::filesystem::path(path).stem().string().c_str();
		

			//Construct path to library
			std::string library_path = App->fs->GetLibraryDirectory();
			library_path += "Textures/" + file_name + TEXTURE_EXTENSION;
			new_texture->SetData(App->importer->LoadTexture(library_path.c_str()), (file_name + "." + image_extension).c_str());
			new_texture->SetFile(path, library_path.c_str());
		}

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

uint ModuleResourceManager::CreateTextureMeta(const char * path)
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

	json_object_dotset_string(obj_data, "Resource Type", "Texture");
	json_object_dotset_string(obj_data, "Binary file", file_name.c_str());


	json_object_dotset_number(obj_data, "UID", UID);

	std::time_t result = std::time(nullptr);
	std::string timestamp = std::asctime(std::localtime(&result));

	json_object_dotset_string(obj_data, "Time Stamp", timestamp.c_str());

	json_object_dotset_boolean(obj_data, "Flip", false);
	json_serialize_to_file(meta_file, meta_path.c_str());

	return UID;
}


//Utility
bool ModuleResourceManager::CheckTimestamp(const char* file, const char* meta)
{
	JSON_Value* meta_file = json_parse_file(meta);

	if (meta_file == nullptr)
	{
		LOG("Error: could not open %s", meta);
		return 0;
	}

	//Extract timestamp from meta
	JSON_Object * object_data = json_value_get_object(meta_file);
	std::string timestamp = json_object_dotget_string(object_data, "Time Stamp");

	//Check last modification of file
	struct stat st;
	stat(file, &st);
	std::string last_modified_date = std::asctime(std::localtime(&st.st_mtime));

	return timestamp.compare(last_modified_date);
}

Resource * ModuleResourceManager::GetResource(uint id)
{
	Resource* res = nullptr;
	if (resources.count(id) >0)
		res = resources[id];

	return res;
}

void ModuleResourceManager::SetToDelete(uint id)
{
	to_delete.push_back(id);
}

bool ModuleResourceManager::DeleteResource(uint id)
{
	if (resources.count(id) > 0)
	{
		Resource* res = resources[id];
		if (res)
		{
			delete res;
			resources.erase(id);
			return true;
		}
	}

	return false;
}


