
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

//Save methodology-------------------------------------------
bool ModuleFileSystem::SaveMeshToOwnFormat(uint num_vert, uint num_ind, float* vert, uint* ind, float* normals, float* texture_coords)
{

	//DATA ORDER: num vertex - num index - vertex - index - has normals - has text coords - normals - text coords
	
	bool has_normals = false;
	bool has_text_coords = false;

	//calculate base size
	uint size = sizeof(uint) * 2 + sizeof(float) * num_vert * 3 + sizeof(uint)*num_ind + sizeof(bool) * 2;

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

	uint ranges[2] = { num_vert, num_ind };
	uint size_of = sizeof(ranges);

	//Copy num vert & num ind
	memcpy(cursor, ranges, size_of);
	cursor += size_of;


	//Copy vert
	size_of = sizeof(float)*num_vert*3 ;
	memcpy(cursor, vert, size_of);
	cursor += size_of;

	//Copy ind
	size_of = sizeof(uint)*num_ind;
	memcpy(cursor, ind, size_of);
	cursor += size_of;

	//Write all to new file
	std::ofstream new_file("test.carca");
	new_file.write(data, size);
	new_file.close();


	delete[] data;
	return true;
}

ComponentMesh * ModuleFileSystem::LoadMeshFromOwnFormat(const char * path)
{



	return nullptr;
}
