#pragma once
#include "Resource.h"
class ResourceTexture :
	public Resource
{
public:
	ResourceTexture(uint id);
	~ResourceTexture();

	void SetData(uint text_id, const char* t_name, int x, int y);

	uint GetTextureId() const
	{
		return texture_buffer_id;
	}

	std::string GetTextureName()const
	{
		return texture_name;
	}

	float2 GetDimensions() const
	{
		return float2(width, height);
	}

private:
	uint texture_buffer_id = 0;
	uint width = 0, height = 0;
	std::string texture_name;
	bool flip = false;
};
