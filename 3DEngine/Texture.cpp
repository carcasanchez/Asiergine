#include "Texture.h"



Texture::Texture(int width, int height, GLubyte* pixmap)
{
	x = width;
	y = height;
	this->pixmap = pixmap;
}


Texture::~Texture()
{
}
