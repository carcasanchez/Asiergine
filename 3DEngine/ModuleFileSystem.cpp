
#include "Application.h"
#include "Globals.h"
#include ".\mmgr\mmgr.h"
#include "ModuleFileSystem.h"

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
	#endif

	#if _RELEASE
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
bool ModuleFileSystem::SaveMeshToOwnFormat(const char* name, uint num_vert, uint num_ind, float* vert, uint* ind, float* normals, float* texture_coords)
{
	bool ret = true;

	//DATA ORDER: num vertex - num index - vertex - index - has normals - has text coords - normals - text coords

	bool has_normals = false;
	bool has_text_coords = false;


	//calculate base size
	uint size = sizeof(uint) * 3 + sizeof(float) * num_vert * 3 + sizeof(uint) * num_ind * 3 + sizeof(bool) * 2;

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
	size_of = sizeof(uint)*num_ind * 3;
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
		size_of = sizeof(float)*num_vert * 3;
		memcpy(cursor, normals, size_of);
		cursor += size_of;
	}


	std::string file_path = CreateDirectoryInLibrary("Meshes");
	file_path += name;
	file_path += ".carca";

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

ComponentMesh * ModuleFileSystem::LoadMeshFromOwnFormat(const char * path)
{
	//Search file
	std::ifstream file(path, std::ifstream::binary);

	//Get file length
	file.seekg(0, file.end);
	std::streamsize length = file.tellg();
	file.seekg(0, file.beg);

	char* data = new char[length];

	//Load data to buffer-----------------------------------------
	if(file.good() && file.is_open())
	{
		file.read(data, length);
		file.close();
	}
	else
	{
		LOG("ERROR: could not load .carca");
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
	unsigned int* ind = new unsigned int[num_ind*3];
	size_of = sizeof(float)*num_ind*3;
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


	delete[] data;

	return new ComponentMesh(App->scene->root, vert, ind, num_vert, num_ind, normals, texture_coord);
}
