
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
	uint cursor = 0;

	//Copy num vert
	data[cursor] = num_vert;
	cursor += sizeof(uint);


	return true;
}
