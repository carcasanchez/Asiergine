#include "Texture.h"



Texture::Texture(int width, int height, GLubyte* pixmap)
{
	x = width;
	y = height;
	pixmap = pixmap;
}


Texture::~Texture()
{
}
