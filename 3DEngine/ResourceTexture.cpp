#include "ResourceTexture.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "mmgr\mmgr.h"


ResourceTexture::ResourceTexture(uint id) : Resource(id, RESOURCE_TYPE::TEXTURE)
{

}


ResourceTexture::~ResourceTexture()
{
	glDeleteTextures(1, &texture_buffer_id);
}

void ResourceTexture::SetData(uint text_id, const char* t_name, int x, int y)
{
	texture_buffer_id = text_id;
	texture_name = t_name;
	width = x;
	height = y;
}
