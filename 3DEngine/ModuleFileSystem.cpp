
#include "Application.h"
#include "Globals.h"
#include ".\mmgr\mmgr.h"
#include "ModuleFileSystem.h"
#include "GameObject.h"

ModuleFileSystem::ModuleFileSystem(bool start_enabled) : Module(start_enabled)
{
	name = "file_system";
}


ModuleFileSystem::~ModuleFileSystem()
{
}


bool ModuleFileSystem::SaveDataToLibrary(const char* data, uint size, const char* name, const char* directory, const char* extension) const
{
	bool ret = true;

	std::string file_path = CreateDirectoryInLibrary(directory);
	file_path += (std::experimental::filesystem::path(name).stem().string());
	file_path += extension;


	//Write all to new file
	std::ofstream new_file(file_path.c_str(), std::ofstream::binary);

	if (new_file.good())
	{
		new_file.write(data, size);
		new_file.close();
	}
	else
	{
		LOG("ERROR: Could not save file to %s", extension);
		ret = false;
	}

	return ret;
}

bool ModuleFileSystem::LoadDataFromLibrary(char ** data, const char * name, const char * directory, const char * extension) const
{
	std::string path;
#if _DEBUG
	path = "../Library/";
#else
	path = "Library/";
#endif

	path += directory;
	path += '/';
	path += name;
	path += extension;

	//Search file
	std::ifstream file(path, std::ifstream::binary);

	//Get file length
	file.seekg(0, file.end);
	std::streamsize length = file.tellg();
	file.seekg(0, file.beg);
	
	//Load data to buffer-----------------------------------------
	if (file.good() && file.is_open())
	{
		data[0] = new char[length];
		file.read(data[0], length);
		file.close();
		return true;
	}
	else
	{
		LOG("ERROR: could not load mesh %s.carca", name);
		return false;
	}
}

std::string ModuleFileSystem::CreateDirectoryInLibrary(const char * folder) const
{
	std::string path;

	#if _DEBUG
	path = "../Library";

	#else 
	path = "./Library";
	#endif

	CreateDirectory(path.c_str(), NULL);
	SetFileAttributes(path.c_str(), FILE_ATTRIBUTE_HIDDEN);

	path += "/";
	path += folder;

	CreateDirectory(path.c_str(), NULL);
	SetFileAttributes(path.c_str(), FILE_ATTRIBUTE_HIDDEN);

	path += "/";

	return path;
}


//Save methodology-------------------------------------------



//DEPRECATED---------------------------------







GameObject * ModuleFileSystem::LoadSceneFromOwnFormat(const char * name)
{
	//DATA ORDER: tag - num objects - [size of object name - object name]

	std::string path;
#if _DEBUG
	path = "..\\Library";
#else 
	path = "Library";
#endif

	path += "\\Scenes\\";
	path += name;
	//path += FORMAT_EXTENSION;


	//Search file
	std::ifstream file(path, std::ifstream::binary);

	//Get file length
	file.seekg(0, file.end);
	std::streamsize length = file.tellg();
	file.seekg(0, file.beg);

	char* data = nullptr;

	//Load data to buffer-----------------------------------------
	if (file.good() && file.is_open())
	{
		data = new char[length];
		file.read(data, length);
		file.close();
	}
	else
	{
		LOG("ERROR: could not load scene  %s.carca", name);
		return nullptr;
	}


	//Get data from buffer---------------
	//DATA ORDER: tag - num objects - [size of object name - object name]

	//Get tag and check that its a scene
	char* cursor = data;
	uint tag[] = { -1 };
	uint size_of = sizeof(uint);
	memcpy(tag, cursor, size_of);

	if (*tag != SCENE_SAVETAG)
	{
		LOG("ERROR: this is not a scene");
		return nullptr;
	}
	cursor += size_of;



	std::vector<std::string> objects;


	//Load Num of OBJ
	uint num_objects[] = { -1 };
	size_of = sizeof(uint);
	memcpy(num_objects, cursor, size_of);
	cursor += size_of;


	//Load obj names
	for (int i = 0; i < num_objects[0]; i++)
	{
		uint object_name_size[] = { -1 };
		size_of = sizeof(uint);
		memcpy(object_name_size, cursor, size_of);
		cursor += size_of;

		char* obj_name = new char[object_name_size[0] + 1];

		size_of = object_name_size[0];
		memcpy(obj_name, cursor, size_of);
		cursor += size_of;
		obj_name[object_name_size[0]] = '\0';
		objects.push_back(obj_name);

		delete[] obj_name;
	}


	delete[] data;
	LOG("Loading scene %s", name);

	//Load objs
	for (int i = 0; i < objects.size(); i++)
	{
		LoadObjectFromOwnFormat(objects[i].c_str());
	}


	return nullptr;
}

GameObject * ModuleFileSystem::LoadObjectFromOwnFormat(const char * name)
{
	std::string path;
#if _DEBUG
	path = "..\\Library";
#else 
	path = "Library";
#endif

	path += "\\GameObjects\\";
	path += name;
	path += FORMAT_EXTENSION;

	//Search file
	std::ifstream file(path, std::ifstream::binary);

	//Get file length
	file.seekg(0, file.end);
	std::streamsize length = file.tellg();
	file.seekg(0, file.beg);

	char* data = nullptr;

	//Load data to buffer-----------------------------------------
	if (file.good() && file.is_open())
	{
		data = new char[length];
		file.read(data, length);
		file.close();
	}
	else
	{
		LOG("ERROR: could not load object %s", name);
		return nullptr;
	}


	//Get data from buffer---------------
	//DATA ORDER: pos - scale - rot - num of childs- [size of child name - child name] - num of meshes - [size of mesh name - mesh name] - size of material name - material name


	//Get tag and check that its a mesh
	char* cursor = data;
	uint tag[] = { -1 };
	uint size_of = sizeof(uint);
	memcpy(tag, cursor, size_of);

	if (*tag != OBJECT_SAVETAG)
	{
		LOG("ERROR: this is not a object");
		return nullptr;
	}
	cursor += size_of;

	//Load position
	float position[3] = { 0, 0, 0 };
	size_of = sizeof(float) * 3;
	memcpy(position, cursor, size_of);
	cursor += size_of;

	//Load scale
	float scale[3] = { 1, 1, 1 };
	size_of = sizeof(float) * 3;
	memcpy(scale, cursor, size_of);
	cursor += size_of;

	//Load rot
	float rot[4] = { 0, 0, 0, 1 };
	size_of = sizeof(float) * 4;
	memcpy(rot, cursor, size_of);
	cursor += size_of;

	GameObject* new_obj = App->scene->CreateGameObject(name, App->scene->root);
	new_obj->CreateComponent_Transform(float3(position[0], position[1], position[2]), float3(scale[0], scale[1], scale[2]), Quat(rot[0], rot[1], rot[2], rot[3]));

	delete[] data;
	return nullptr;
}
