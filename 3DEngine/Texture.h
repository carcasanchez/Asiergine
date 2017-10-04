#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

struct Texture
{
public:
	Texture(int width, int height, GLubyte* pixmap);
	~Texture();

	int x, y;
	GLubyte* pixmap;
};

