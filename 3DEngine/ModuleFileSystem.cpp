
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

std::string ModuleFileSystem::CreateDirectoryInLibrary(const char * folder)
{
	std::string path;

	#if _DEBUG
	path = "..\\Library";

	#else 
	path = ".\\Library";
	#endif

	CreateDirectory(path.c_str(), NULL);
	SetFileAttributes(path.c_str(), FILE_ATTRIBUTE_HIDDEN);

	path += "\\";
	path += folder;

	CreateDirectory(path.c_str(), NULL);
	SetFileAttributes(path.c_str(), FILE_ATTRIBUTE_HIDDEN);

	path += "\\";

	return path;
}


//Save methodology-------------------------------------------
bool ModuleFileSystem::SaveSceneToOwnFormat(const char* name, std::vector<std::string> objects)
{
	//DATA ORDER: tag - num objects - [size of object name - object name]

	bool ret = true;

	uint size = sizeof(uint) * 2;

	for (int i = 0; i < objects.size(); i++)
	{
		size += sizeof(uint);
		size += sizeof(char)*objects[i].size();
	}

	//Save tag
	char* data = new char[size];
	char* cursor = data;
	uint size_of = sizeof(uint);
	uint tag = SCENE_SAVETAG;

	memcpy(cursor, &tag, size_of);
	cursor += size_of;

	//Save num objs
	uint num_objs = objects.size();
	memcpy(cursor, &num_objs, size_of);
	cursor += size_of;


	//Save obj names
	for (int i = 0; i < objects.size(); i++)
	{
		//copy child name size
		uint size_of_name = objects[i].size();
		size_of = sizeof(uint);
		memcpy(cursor, &size_of_name, size_of);
		cursor += size_of;

		//copy child name
		size_of = sizeof(char)*objects[i].size();
		memcpy(cursor, objects[i].data(), size_of);
		cursor += size_of;
	}

	std::string file_path = CreateDirectoryInLibrary("Scenes");
	file_path += name;
	file_path += FORMAT_EXTENSION;

	//Write all to new file
	std::ofstream new_file(file_path.c_str(), std::ofstream::binary);

	if (new_file.good())
	{
		new_file.write(data, size);
		new_file.close();
	}
	else
	{
		LOG("ERROR: Could not save scene to .carca");
		ret = false;
	}

	delete[] data;
	LOG("Saved Scene %s to Library", name);

	return ret;
}

bool ModuleFileSystem::SaveGameObjectToOwnFormat(const char * name, float3 pos, float3 scale, math::Quat rot, std::vector<std::string> childs, std::vector<std::string> meshes, const char* material)
{
	//DATA ORDER: pos - scale - rot - num of childs- [size of child name - child name] - num of meshes - [size of mesh name - mesh name] - size of material name - material name

	bool ret = true;

	uint size_of_childs = sizeof(uint);
	for (int i = 0; i < childs.size(); i++)
	{
		size_of_childs += sizeof(uint);
		size_of_childs += sizeof(char) * childs[i].size();
	}

	uint size_of_meshes = sizeof(uint);
	for (int i = 0; i < meshes.size(); i++)
	{
		size_of_meshes += sizeof(uint);
		size_of_meshes += sizeof(char) * meshes[i].size();
	}

	uint size_of_mat = sizeof(uint) + strlen(material);

	uint size = sizeof(float) * 10 + size_of_meshes + size_of_childs + size_of_mat + sizeof(uint);

	char* data = new char[size];
	char* cursor = data;
	uint size_of = sizeof(uint);


	//copy tag

	uint tag = OBJECT_SAVETAG;
	memcpy(cursor, &tag, size_of);
	cursor += size_of;

	
	//Copy transform;
	float transform[10] = {pos.x, pos.y, pos.z, scale.x, scale.y, scale.z, rot.x, rot.y, rot.z, rot.w};
	size_of = sizeof(float) * 10;
	memcpy(cursor, transform, size_of);
	cursor += size_of;

	//Copy childs

	uint num_of_childs = childs.size();
	size_of = sizeof(uint);
	memcpy(cursor, &num_of_childs, size_of);
	cursor += size_of;

	for (int i = 0; i < num_of_childs; i++)
	{
		//copy child name size
		uint size_of_name = childs[i].size();
		size_of = sizeof(uint);
		memcpy(cursor, &size_of_name, size_of);
		cursor += size_of;

		//copy child name
		size_of = sizeof(char)*childs[i].size();
		memcpy(cursor, childs[i].data(), size_of);
		cursor += size_of;
	}


	//Copy meshes
	uint num_of_meshes = meshes.size();
	size_of = sizeof(uint);
	memcpy(cursor, &num_of_meshes, size_of);
	cursor += size_of;

	for (int i = 0; i < num_of_meshes; i++)
	{
		//copy mesh name size
		uint size_of_name = meshes[i].size();
		size_of = sizeof(uint);
		memcpy(cursor, &size_of_name, size_of);
		cursor += size_of;

		//copy mesh name
		size_of = sizeof(char)*meshes[i].size();
		memcpy(cursor, meshes[i].data(), size_of);
		cursor += size_of;
	}

	//copy material
	size_of_mat = strlen(material);
	size_of = sizeof(uint);
	memcpy(cursor, &size_of_mat, size_of);
	cursor += size_of;
	
	if (size_of_mat > 0)
	{
		size_of = size_of_mat;
		memcpy(cursor, material, size_of);
		cursor += size_of;
	}


	std::string file_path = CreateDirectoryInLibrary("GameObjects");
	file_path += name;
	file_path += FORMAT_EXTENSION;

	//Write all to new file
	std::ofstream new_file(file_path.c_str(), std::ofstream::binary);

	if (new_file.good())
	{
		new_file.write(data, size);
		new_file.close();
	}
	else
	{
		LOG("ERROR: Could not save file to .carca");
		ret = false;
	}

	delete[] data;
	LOG("Saved GameObject %s to Library", name);

	return ret;
}
bool ModuleFileSystem::SaveMeshToOwnFormat(const char* name, uint num_vert, uint num_ind, const float* vert, uint* ind, const float* normals, const float* texture_coords)
{
	bool ret = true;

	//DATA ORDER: tag - num vertex - num index - vertex - index - has normals - has text coords - normals - text coords

	bool has_normals = false;
	bool has_text_coords = false;


	//calculate base size
	uint size = sizeof(uint) * 3 + sizeof(float) * num_vert * 3 + sizeof(uint) * num_ind + sizeof(bool) * 2;

	if (normals)
	{
		has_normals = true;
		size += sizeof(float)*num_vert * 3;
	}
	if (texture_coords)
	{
		has_text_coords = true;
		size += sizeof(float)*num_vert * 2;
	}


	char* data = new char[size];
	char* cursor = data;

	uint ranges[3] = { MESH_SAVETAG, num_vert, num_ind };
	uint size_of = sizeof(ranges);


	//Copy num vert & num ind
	memcpy(cursor, ranges, size_of);
	cursor += size_of;


	//Copy vert
	size_of = sizeof(float)*num_vert * 3;
	memcpy(cursor, vert, size_of);
	cursor += size_of;

	//Copy ind
	size_of = sizeof(uint)*num_ind;
	memcpy(cursor, ind, size_of);
	cursor += size_of;

	//Copy normals and text coords
	bool has[2] = { has_normals, has_text_coords };

	size_of = sizeof(bool);	
	memcpy(cursor, &has[0], size_of);
	cursor += size_of;	
	
	if (has_normals)
	{
		size_of = sizeof(float)*num_vert * 3;
		memcpy(cursor, normals, size_of);
		cursor += size_of;
	}

	size_of = sizeof(bool);
	memcpy(cursor, &has[1], size_of);
	cursor += size_of;

	if (has_text_coords)
	{
		size_of = sizeof(float)*num_vert * 2;
		memcpy(cursor, texture_coords, size_of);
		cursor += size_of;
	}


	std::string file_path = CreateDirectoryInLibrary("Meshes");
	file_path += name;
	file_path += FORMAT_EXTENSION;

	//Write all to new file
	std::ofstream new_file(file_path.c_str(), std::ofstream::binary);

	if (new_file.good())
	{
		new_file.write(data, size);
		new_file.close();
	}
	else
	{
		LOG("ERROR: Could not save file to .carca");
		ret = false;
	}

	delete[] data;
	
	return ret;
}

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

		char* obj_name = new char[object_name_size[0]+1];

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
	for (int i = 0; i < objects.size();i++)
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
	size_of = sizeof(float)*3;
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

ComponentMesh * ModuleFileSystem::LoadMeshFromOwnFormat(const char * name)
{
	std::string path;
	#if _DEBUG
		path = "..\\Library";
	#else
		path = "Library";
	#endif

	path += "\\Meshes\\";
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
	if(file.good() && file.is_open())
	{
		data = new char[length];
		file.read(data, length);
		file.close();
	}
	else
	{
		LOG("ERROR: could not load mesh %s.carca", name);
		return nullptr;
	}


	//Get data from buffer---------------
	//DATA ORDER: num vertex - num index - vertex - index - has normals - has text coords - normals - text coords

	//Data to load
	uint num_vert = 0;
	uint num_ind = 0;

	bool has_normals = false;
	bool has_text_coords = false;

	
	
	//Get tag and check that its a mesh
	char* cursor = data;
	uint tag[] = {-1};
	uint size_of = sizeof(uint);
	memcpy(tag, cursor, size_of);

	if (*tag != MESH_SAVETAG)
	{
		LOG("ERROR: this is not a mesh");
		return nullptr;
	}
	cursor += size_of;

	//If OK, load num vertx and indx
	uint ranges[2];
	size_of = sizeof(ranges);
	memcpy(ranges, cursor, size_of);
	num_vert = ranges[0];
	num_ind = ranges[1];
	cursor += size_of;


	//Load vertx
	float* vert = new float[num_vert*3];
	size_of = sizeof(float)*num_vert * 3;
	cursor += size_of;
	memcpy(vert, cursor, size_of);

	//Load ind
	unsigned int* ind = new unsigned int[num_ind];
	size_of = sizeof(float)*num_ind;
	cursor += size_of;
	memcpy(ind, cursor, size_of);
	

	//Load normals and text coords

	bool hases[2];

	float* normals = nullptr;
	float* texture_coord = nullptr;

	//has normals
	size_of = sizeof(bool);
	cursor += size_of;
	memcpy(&hases[0], cursor, size_of);
	if (hases[0])
	{
		normals = new float[num_vert * 3];
		size_of = sizeof(float) * num_vert*3;
		cursor += size_of;
		memcpy(normals, cursor, size_of);
	}

	//has text coords
	size_of = sizeof(bool);
	cursor += size_of;
	memcpy(&hases[1], cursor, size_of);
	if (hases[1])
	{
		texture_coord = new float[num_vert * 2];
		size_of = sizeof(float) * num_vert * 2;
		cursor += size_of;
		memcpy(texture_coord, cursor, size_of);
	}

	LOG("Loaded %s successfully", name);

	delete[] data;

	return new ComponentMesh(App->scene->root, vert, ind, num_vert, num_ind, normals, texture_coord);
}

