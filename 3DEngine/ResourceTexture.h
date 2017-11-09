#pragma once
#include "Resource.h"
class ResourceTexture :
	public Resource
{
public:
	ResourceTexture(uint id);
	~ResourceTexture();

	void SetData(uint text_id, const char* t_name);

	uint GetTextureId() const
	{
		return texture_buffer_id;
	}

	std::string GetTextureName()const
	{
		return texture_name;
	}

private:
	uint texture_buffer_id = 0;
	uint width = 0, height = 0;
	std::string texture_name;
	bool flip = false;
};
