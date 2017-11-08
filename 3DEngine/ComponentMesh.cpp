#include "ComponentMesh.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "mmgr\mmgr.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "CompTransform.h"

ComponentMesh::ComponentMesh(GameObject* game_object, float* ver, uint* ind, uint num_vert, uint num_ind, float* normals, float* texture_coords): vertices(ver), indices(ind), num_vertices(num_vert), num_indices(num_ind), Component(game_object), normals(normals), texture_coords(texture_coords)
{
	type = COMPONENT_MESH;

	//alloc vertices
	glGenBuffers(1, (uint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);
	
	//alloc indices
	glGenBuffers(1, (uint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, indices, GL_STATIC_DRAW);
		
	if (texture_coords != nullptr)
	{
		//alloc texture coords
		glGenBuffers(1, (uint*)&(text_coord_id));
		glBindBuffer(GL_ARRAY_BUFFER, text_coord_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 2, texture_coords, GL_STATIC_DRAW);
	}

	
}

ComponentMesh::~ComponentMesh()
{
	delete[] vertices;
	delete[] indices;
	delete[] normals;
	delete[] texture_coords;

	glDeleteBuffers(1, &text_coord_id);
	glDeleteBuffers(1, &id_vertices);
	glDeleteBuffers(1, &id_indices);
}

void ComponentMesh::Draw()
{
	if (IsActive() == true)
	{
		//Bind vertices
		glEnableClientState(GL_VERTEX_ARRAY);


		Component* transform = game_object->GetComponentByType(COMPONENT_TRANSFORM);
		if (transform != nullptr && transform->IsActive())
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMultMatrixf(((CompTransform*)transform)->GetMatrixPtr());
		}
		

		glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		Component* mat = game_object->GetComponentByType(COMPONENT_MATERIAL);
		
			if (mat != nullptr && mat->IsActive())
			{
				glBindTexture(GL_TEXTURE, ((ComponentMaterial*)mat)->GetTextureID());
				if (text_coord_id != 0)
				{
					//Bind textures
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glBindTexture(GL_TEXTURE_2D, 0);
					glBindTexture(GL_TEXTURE_2D, ((ComponentMaterial*)mat)->GetTextureID());
					glBindBuffer(GL_ARRAY_BUFFER, text_coord_id);
					glTexCoordPointer(2, GL_FLOAT, 0, NULL);
				}
			}
		

		//Bind indices and draw
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);


		glBindTexture(GL_TEXTURE_2D, 0);



		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if (App->scene->debug_normals && normals != nullptr)
		{
			DebugDraw();
		}

		if (transform != nullptr)
		{
			glPopMatrix();
		}
	}

}

void ComponentMesh::DebugDraw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	//Draw Normals
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


void ComponentMesh::Update(float real_dt, float game_dt)
{
	if (game_object->IsStatic() && App->renderer3D->frustum_culling && deleted == false)
		return;
	App->renderer3D->SetMeshToDraw(this);
}

void ComponentMesh::OnEditor()
{
	if (IsActive() == true)
	{
		ImGui::TextWrapped("Number of vertices: %i", GetNumVertices());
	}
}


//----------------------------------------------------------
uint ComponentMesh::PrepareToSave()const
{
	uint size = 0;
	size += sizeof(uint); //UID
	size += sizeof(uint); //size of mesh name
	size += sizeof(char) * name.length(); //mesh name

	return size;
}

void ComponentMesh::Save(char *& cursor) const
{
	//copy mesh UID
	uint size_of = sizeof(uint);
	memcpy(cursor, &UID, size_of);
	cursor += size_of;

	//copy mesh name size
	uint size_of_name = name.length();
	size_of = sizeof(uint);
	memcpy(cursor, &size_of_name, size_of);
	cursor += size_of;

	//copy mesh name
	size_of = size_of_name;
	memcpy(cursor, name.data(), size_of);
	cursor += size_of;
}
