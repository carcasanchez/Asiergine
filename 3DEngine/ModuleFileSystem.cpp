
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

std::string ModuleFileSystem::GetLibraryDirectory()
{
	std::string path;
#if _DEBUG
	path = "../Library/";
#else
	path = "Library/";
#endif

	return path;
}


bool ModuleFileSystem::SaveDataToLibrary(const char* data, uint size, const char* name, const char* directory, const char* extension) const
{
	bool ret = true;

	std::string file_path = CreateDirectoryInLibrary(directory);
	file_path += name;
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