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

ComponentMesh::ComponentMesh(GameObject* game_object): Component(game_object)
{
	type = COMPONENT_MESH;
}

ComponentMesh::~ComponentMesh()
{
	if (mesh)
		mesh->DecreaseInstancies();
}

void ComponentMesh::Draw()
{
	if (IsActive() == true && mesh)
	{
		CompTransform* transform = (CompTransform*)game_object->GetComponentByType(COMPONENT_TRANSFORM);
		ComponentMaterial* mat = (ComponentMaterial*)game_object->GetComponentByType(COMPONENT_MATERIAL);
		
		const float* t_matrix = nullptr;
		uint text_id = 0;
		if (transform != nullptr && transform->IsActive())
			t_matrix = transform->GetMatrixPtr();
		if (mat != nullptr && mat->IsActive())
			text_id = mat->GetTextureID();
		
		mesh->Draw(t_matrix, text_id);
	}

}

void ComponentMesh::DebugDraw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	

	glDisableClientState(GL_VERTEX_ARRAY);
}


void ComponentMesh::Update(float real_dt, float game_dt)
{
	if (game_object->IsStatic() && App->renderer3D->frustum_culling)
		return;
	App->renderer3D->SetMeshToDraw(this);
}

void ComponentMesh::OnEditor()
{
	if (IsActive() == true)
	{
		bool want_to_change_mesh = false;

		ImGui::TextWrapped("Current Mesh:", GetNumVertices());

		if (mesh)
		{
			want_to_change_mesh = ImGui::Button(name.c_str());
			ImGui::TextWrapped("Instancies in scene: %i",mesh->GetInstancies());
		}
		else 	
			want_to_change_mesh = ImGui::Button("MISSING!");




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
