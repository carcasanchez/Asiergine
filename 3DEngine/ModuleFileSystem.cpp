
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

std::string ModuleFileSystem::GetLibraryDirectory()const
{
	std::string path;
#if _DEBUG
	path = "../Library/";
#else
	path = "Library/";
#endif

	return path;
}

std::string ModuleFileSystem::GetAssetDirectory()const
{
	std::string path;
#if _DEBUG
	path = "../Assets/";
#else
	path = "Assets/";
#endif

	return path;
}



bool ModuleFileSystem::SaveDataTo(const char * data, uint size, const char * path) const
{
	bool ret = true;

	//Write all to new file
	std::ofstream new_file(path, std::ofstream::binary);

	if (new_file.good())
	{
		new_file.write(data, size);
		new_file.close();
	}
	else
	{
		LOG("ERROR: Could not save file %s", name);
		ret = false;
	}

	return ret;
}

uint ModuleFileSystem::LoadDataFrom(char* & data, const char * path) const
{
	//Search file
	std::ifstream file(path, std::ifstream::binary);

	//Get file length
	file.seekg(0, file.end);
	std::streamsize length = file.tellg();
	file.seekg(0, file.beg);

	//Load data to buffer-----------------------------------------
	if (file.good() && file.is_open())
	{
		data = new char[length];
		file.read(data, length);
		file.close();
		return length;
	}
	else
	{
		LOG("ERROR: could not load data from %s", path);
		return 0;
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
std::string ModuleFileSystem::CreateDirectoryInAssets(const char * folder) const
{
	std::string path;

#if _DEBUG
	path = "../Assets";

#else 
	path = "./Assets";
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


bool ModuleFileSystem::ExistsFile(const char * path) const
{
	std::fstream infile(path);
	return infile.good();
}

bool ModuleFileSystem::CloneFile(const char * source, const char * destination) const
{
	char* data = nullptr;

	uint size = LoadDataFrom(data, source);
	if (size > 0)
	{
		SaveDataTo(data, size, destination);
		delete[] data;
	}

	return true;
}



//Deprecated-----------------------------------------------------------------------
bool ModuleFileSystem::SaveDataToAssets(const char * data, uint size, const char * name, const char * extension) const
{
	bool ret = true;

	std::string file_path = GetAssetDirectory();
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
