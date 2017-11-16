#include "ResourceMesh.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Application.h"
#include "ModuleScene.h"


ResourceMesh::ResourceMesh(uint id) : Resource(id, RESOURCE_TYPE::MESH)
{
}


ResourceMesh::~ResourceMesh()
{
	if(vertices)
		delete[] vertices;	
	if(indices) 
		delete[] indices;
	if (normals)
		delete[] normals;
	if(texture_coords)
		delete[] texture_coords;

	if(text_coord_id != 0)
		glDeleteBuffers(1, &text_coord_id);
	if (id_vertices != 0)
		glDeleteBuffers(1, &id_vertices);
	if (id_indices != 0)
		glDeleteBuffers(1, &id_indices);
	if (id_normals != 0)
		glDeleteBuffers(1, &id_normals);
}

void ResourceMesh::SetData(float* ver, uint* ind, uint num_vert, uint num_ind, float* norm, float* text_coords)
{
	vertices = ver;
	indices = ind;
	num_vertices = num_vert;
	num_indices = num_ind;
	normals = norm;
	texture_coords = text_coords;

	//alloc vertices
	glGenBuffers(1, (uint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);

	//alloc indices
	glGenBuffers(1, (uint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, indices, GL_STATIC_DRAW);

	if (normals)
	{
		//Alloc normals
		glGenBuffers(1, (uint*)&(id_normals));
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, normals, GL_STATIC_DRAW);
	}

	if (texture_coords != nullptr)
	{
		//alloc texture coords
		glGenBuffers(1, (uint*)&(text_coord_id));
		glBindBuffer(GL_ARRAY_BUFFER, text_coord_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 2, texture_coords, GL_STATIC_DRAW);
	}
}

void ResourceMesh::Draw(const float* transform, uint texture_id) const
{
	//Bind vertices
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	if(transform != nullptr)
	{ 
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(transform);
	}

	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if(normals)
	{ 
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}

	if (text_coord_id != 0)
	{
		//Bind textures
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glBindBuffer(GL_ARRAY_BUFFER, text_coord_id);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}
	
	//Bind indices and draw
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	
	if (App->scene->debug_normals && normals != nullptr)
	{
		//Draw Normals	
		for (uint i = 0; i < num_vertices * 3; i += 3)
		{
			glColor3f(3.0f, 0.0f, 1.0f);
			glBegin(GL_LINES);
			glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
			glVertex3f(vertices[i] + normals[i], vertices[i + 1] + normals[i + 1], vertices[i + 2] + normals[i + 2]);
			glEnd();
			glColor3f(1.0f, 1.0f, 1.0f);
		}		
	}

	if (transform != nullptr)
	{
		glPopMatrix();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
