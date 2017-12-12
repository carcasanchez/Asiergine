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

bool ModuleResourceManager::Start()
{
	//ReimportAllAssets();
	return true;
}

update_status ModuleResourceManager::PreUpdate(float real_dt, float game_dt)
{
	if (!to_delete.empty())
	{
		//delete all marked resources
		for (int i = 0; i < to_delete.size(); i++)
		{
			Resource* res = GetResource(to_delete[i]);
			if(res->GetInstancies() == 0)
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

void ModuleResourceManager::FileDroppedInEditor(const char * path)
{
	std::string tmp = path;
	std::string name;
	std::string extension;

	//Get file name
	name = std::experimental::filesystem::path(path).stem().string().c_str();

	//Get file extension
	extension = std::experimental::filesystem::path(path).extension().string().c_str();

	//Normalize to lower case
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

	//Manage file depending on extension
	if (extension.compare(".fbx") == 0 || extension.compare(".obj") == 0)
		LoadResource(path);
	else if (extension.compare(".carca") == 0)
	{
		std::string asset_dir = App->fs->CreateDirectoryInAssets("Meshes");
		asset_dir += name + extension;
		App->fs->CloneFile(path, asset_dir.c_str());
		ImportMesh(asset_dir.c_str(), true);
	}
	else if (extension.compare(".png") == 0 || extension.compare(".dds") == 0 || extension.compare(".tga") == 0 || extension.compare(".jpg") == 0 || extension.compare(".jpeg") == 0)
	{
		std::string asset_dir = App->fs->CreateDirectoryInAssets("Textures");
		asset_dir += name + extension;
		App->fs->CloneFile(path, asset_dir.c_str());
		ImportTexture(asset_dir.c_str(), true);
	}
	else LOG("ERROR: File extension '.%s' not allowed", extension.c_str());
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

	//Get file extension
	extension = std::experimental::filesystem::path(path).extension().string().c_str();

	//Normalize to lower case
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

	//Manage file depending on extension
	if (extension.compare(".fbx") == 0 || extension.compare(".obj") == 0)
		ManageFBX(path);
	else if (extension.compare(".carca") == 0)
		resource_id = ManageMesh(path);
	else if (extension.compare(".png") == 0 || extension.compare(".dds") == 0 || extension.compare(".tga") == 0 || extension.compare(".jpg") == 0 || extension.compare(".jpeg") == 0)
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
		resource_id = ImportMesh(path);
	}	
	else
	{		
		//Construct path to library
		//Extract file name
		std::string file_name = std::experimental::filesystem::path(path).stem().string().c_str();
		std::string library_path = App->fs->GetLibraryDirectory();
		library_path += "Meshes/" + file_name + FORMAT_EXTENSION;
				
		//Read UID from meta 
		JSON_Value * value = json_parse_file(meta_file.c_str());
		JSON_Object* obj_data = json_value_get_object(value);
		resource_id = json_object_dotget_number(obj_data, "UID");

		//If timestamp doen't match, reimport
		if (!CheckTimestamp(path, meta_file.c_str()))
		{
			resource_id = ImportMesh(path);
		}
			
		//Check if mesh has been already loaded
		Resource* new_mesh = GetResource(resource_id);
		
		//IF HASN'T: load asset from library		
		if (new_mesh == nullptr)
		{			
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

uint ModuleResourceManager::ImportMesh(const char * path, bool unload_after_import)
{
	uint resource_id = CreateMeshMeta(path);

	//Extract file name
	std::string file_name = std::experimental::filesystem::path(path).stem().string().c_str();

	//Construct path to library
	std::string library_path;
	library_path += App->fs->CreateDirectoryInLibrary("Meshes") + file_name + FORMAT_EXTENSION;

	ResourceMesh* new_mesh = App->importer->LoadMeshFromOwnFormat(path, resource_id);

	if (new_mesh)
	{
		//Import mesh to library
		App->importer->SaveMeshToOwnFormat(library_path.c_str(), new_mesh);
		new_mesh->SetFile(path, library_path.c_str());
	}

	//Unload if only wants to import to library
	if (unload_after_import)
	{
		DeleteResource(resource_id);
		resource_id = 0;
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
	
	//Check if meta exists (a.k.a if texture has already been imported)

	//IF NOT: create meta and import to library
	if (!App->fs->ExistsFile(meta_file.c_str()))
	{
		resource_id = ImportTexture(path);
	}
	else
	{
		//Read UID from meta 
		JSON_Value * value = json_parse_file(meta_file.c_str());
		JSON_Object* obj_data = json_value_get_object(value);
		resource_id = json_object_dotget_number(obj_data, "UID");

		//If timestamp doen't match, reimport
		if (!CheckTimestamp(path, meta_file.c_str()))
		{
			resource_id = ImportTexture(path);
		}

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
			float2 texture_dimensions;
			uint texture_id = App->importer->LoadTexture(library_path.c_str(), texture_dimensions);
			new_texture->SetData(texture_id, (file_name + image_extension).c_str(), texture_dimensions.x, texture_dimensions.y);
			new_texture->SetFile(path, library_path.c_str());
		}

	}

	return resource_id;
}

uint ModuleResourceManager::ImportTexture(const char * path, bool unload_after_import)
{
	uint resource_id = CreateTextureMeta(path);

	//Construct path to library
	std::string library_path = App->fs->CreateDirectoryInLibrary("Textures") + std::experimental::filesystem::path(path).stem().string().c_str() + TEXTURE_EXTENSION;

	ResourceTexture* new_texture = (ResourceTexture*)CreateResource(Resource::TEXTURE, resource_id);

	std::string texture_name = std::experimental::filesystem::path(path).stem().string().c_str();
	std::string texture_extension = std::experimental::filesystem::path(path).extension().string().c_str();

	//Import Texture and save it to library
	float2 texture_dimensions;
	uint texture_id = App->importer->LoadTexture(path, texture_dimensions, true);
	if (texture_id != 0)
	{
		App->importer->SaveTextureToDDS(library_path.c_str());
	
		//Unload texture and load it from DDS later (to be sure that you loaded it from dds format)
		glDeleteTextures(1, &texture_id);
	}

	//Load again if you want
	if (!unload_after_import)
	{
		new_texture->SetData(App->importer->LoadTexture(library_path.c_str(), texture_dimensions), (texture_name + texture_extension).c_str(), texture_dimensions.x, texture_dimensions.y);
		new_texture->SetFile(path, library_path.c_str());		
	}
	else
	{
		//Unload if only wants to import to library
		DeleteResource(resource_id);
		resource_id = 0;
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

	//Save timestamp of file
	struct stat st;
	stat(path, &st);
	std::string timestamp = std::asctime(std::localtime(&st.st_mtime));

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

	//Save timestamp of file
	struct stat st;
	stat(path, &st);
	std::string timestamp = std::asctime(std::localtime(&st.st_mtime));

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

	return (timestamp.compare(last_modified_date)==0);
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

//Reimports all .carca and images to library. Called every init.
void ModuleResourceManager::ReimportAllAssets()
{
	//Reimport Meshes
	std::string mesh_asset_directory = App->fs->GetAssetDirectory();
	mesh_asset_directory += "Meshes/";
	for (std::experimental::filesystem::recursive_directory_iterator::value_type it : std::experimental::filesystem::recursive_directory_iterator(mesh_asset_directory.c_str()))
	{
		std::string extension = std::experimental::filesystem::path(it.path().string().c_str()).extension().string().c_str();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		//Skips everything that it's not a mesh
		if (extension.compare(".carca") != 0)
			continue;

		ImportMesh(it.path().string().c_str(), true);
	}
	

	//Reimport Textures
	std::string texture_asset_directory = App->fs->GetAssetDirectory();
	texture_asset_directory += "Textures/";
	for (std::experimental::filesystem::recursive_directory_iterator::value_type it : std::experimental::filesystem::recursive_directory_iterator(texture_asset_directory.c_str()))
	{
		std::string extension = std::experimental::filesystem::path(it.path().string().c_str()).extension().string().c_str();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		//Skips everything that it's not a texture
		if (extension.compare(".png") != 0 && extension.compare(".jpg") != 0 && extension.compare(".dds") != 0 && extension.compare(".tga") != 0)
			continue;

		ImportTexture(it.path().string().c_str(), true);
	}
}

//Checks if assets have been changed and reloads them
void ModuleResourceManager::ReloadAllAssets()
{
	std::string fbx_directory = App->fs->GetAssetDirectory();
	fbx_directory += "FBX/";
	/*std::string meshes_directory = App->fs->GetAssetDirectory();
	std::string textures_directory = App->fs->GetAssetDirectory();
	meshes_directory += "Meshes/";
	textures_directory += "Textures/";*/

	//Reload FBX
	for (std::experimental::filesystem::recursive_directory_iterator::value_type it : std::experimental::filesystem::recursive_directory_iterator(fbx_directory.c_str()))
	{
		std::string filename = std::experimental::filesystem::path(it.path().string().c_str()).stem().string().c_str();
		std::string extension = std::experimental::filesystem::path(it.path().string().c_str()).extension().string().c_str();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
		if (extension.compare(".fbx") != 0)
			continue;

		if (CheckTimestamp((fbx_directory + filename + extension).c_str(), (fbx_directory + filename + extension + META_EXTENSION).c_str()))
		{
			App->importer->ImportFBX((fbx_directory + filename + extension).c_str());
		}

	}
	//TODO: Reload meshes and textures already in memory
	/*//Reload Meshes
	for (std::experimental::filesystem::recursive_directory_iterator::value_type it : std::experimental::filesystem::recursive_directory_iterator(meshes_directory.c_str()))
	{
		std::string filename = std::experimental::filesystem::path(it.path().string().c_str()).stem().string().c_str();
		std::string extension = std::experimental::filesystem::path(it.path().string().c_str()).extension().string().c_str();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
		if (extension.compare(FORMAT_EXTENSION) != 0)
			continue;

		std::string file = fbx_directory + filename + extension;
		std::string meta_file = file + META_EXTENSION;

		if (CheckTimestamp((fbx_directory + filename + extension).c_str(), meta_file.c_str()))
		{
			uint resource_uid = 0;
			
			JSON_Value* j_value = json_parse_file(meta_file.c_str());

			if (j_value == nullptr)
			{
				LOG("Error: could not open %s", meta_file.c_str());
				continue;
			}

			//Extract uid from meta
			JSON_Object * object_data = json_value_get_object(j_value);
			resource_uid = json_object_dotget_number(object_data, "UID");

			//Check if resource is already loaded
			Resource* res = GetResource(resource_uid);
			if (res)
			{
				ChangeResource(res, file.c_str());
			}

		}

	}*/
	

}

Resource* ModuleResourceManager::ChangeResource(Resource* res, const char * path) const
{
	if (res)
		res->DecreaseInstancies();

	res = App->resource_m->LoadResource(path);

	if (res)
	{
		res->IncreaseInstancies();
		return res;
	}
	else
		LOG("Resource %s does not exist!", path);
	
	return nullptr;
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


