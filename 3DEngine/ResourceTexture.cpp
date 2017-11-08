#include "ResourceTexture.h"



ResourceTexture::ResourceTexture(uint id) : Resource(id, RESOURCE_TYPE::TEXTURE)
{
}


ResourceTexture::~ResourceTexture()
{
	//glDeleteTextures(1, &texture_id);
}
