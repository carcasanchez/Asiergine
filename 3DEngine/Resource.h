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

protected:
	RESOURCE_TYPE type = RESOURCE_TYPE::NONE;
	uint UID = 0;
	std::string assets_file;
	std::string library_file;	
	uint instancies = 0;
};

