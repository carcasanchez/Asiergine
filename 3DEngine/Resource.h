#pragma once
#include "Globals.h"

class Resource
{
public:

	enum RESOURCE_TYPE
	{
		NONE = 0,
		TEXTURE,
		MESH
	};

	Resource(uint uid, RESOURCE_TYPE type);
	virtual ~Resource();

	RESOURCE_TYPE GetType() const
	{
		return type;
	}

	uint GetID() const
	{
		return UID;
	}

	void IncreaseInstancies()
	{
		instancies++;
	}

	void DecreaseInstancies()
	{
		if(instancies>0)
			instancies--;
	}

	bool HasInstancies() const
	{
		return instancies > 0;
	}

	uint GetInstancies() const
	{
		return instancies;
	}


	void SetFile(const char* asset_f = nullptr, const char* library_f = nullptr)
	{
		if (asset_f)
			assets_file = asset_f;

		if (library_f)
			library_file = library_f;
	}

	const char* GetAssetFile() const
	{
		return assets_file.c_str();
	}

	const char* GetLibraryFile() const
	{
		return library_file.c_str();
	}


protected:
	RESOURCE_TYPE type = RESOURCE_TYPE::NONE;
	uint UID = 0;
	std::string assets_file;
	std::string library_file;	
	uint instancies = 0;
};

