#include "Geometry.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include ".\mmgr\mmgr.h"


Geometry::Geometry(float* ver, uint* ind, uint num_vert, uint num_ind, uint tex_id, uint* texture_coords): vertices(ver), indices(ind), num_vertices(num_vert), num_indices(num_ind), texture_id(tex_id)
{
	//alloc vertices
	glGenBuffers(1, (uint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);

	//alloc indices
	glGenBuffers(1, (uint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, indices, GL_STATIC_DRAW);


	if (texture_id != 0)
	{
		//alloc texture coords
		glGenBuffers(1, (uint*)&(texture_id));
		glBindBuffer(GL_ARRAY_BUFFER, texture_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * num_indices*2, texture_coords, GL_STATIC_DRAW);
	}
	else LOG("Warning: --------Loading Mesh Without Texture");

}

Geometry::~Geometry()
{
//	delete[] vertices;
//	delete[] indices;
}

void Geometry::Draw()
{
	glBindTexture(GL_TEXTURE, texture_id);

	//Bind vertices
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//Bind textures
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, texture_id);
	glTexCoordPointer(2, GL_INT, 0, NULL);


	//Bind indices and draw
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE, 0);

}

void Geometry::DebugDraw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
		for (uint i = 0; i < num_vertices * 3; i += 3)
		{
			glColor3f(3.0f, 0.0f, 1.0f);
			glBegin(GL_LINES);
			glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
			glVertex3f(vertices[i] + normals[i], vertices[i + 1] + normals[i + 1], vertices[i + 2] + normals[i + 2]);
			glEnd();
			glColor3f(1.0f, 1.0f, 1.0f);
		}
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
}