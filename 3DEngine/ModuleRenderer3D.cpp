#include "Globals.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>



#include "Application.h"
#include ".\mmgr\mmgr.h"

#include "ModuleRenderer3D.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "GameObject.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "3DEngine/Glew/libx86/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	name = "renderer";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(const JSON_Object* config_data)
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else {
		//LoadData from Config
		assert(config_data != nullptr);

		gl_depth_enabled = json_object_dotget_boolean(config_data, "gl_depth_enabled");
		gl_cull_face_enabled = json_object_dotget_boolean(config_data, "gl_cull_face_enabled");
		gl_lighting_enabled = json_object_dotget_boolean(config_data, "gl_lighting_enabled");
		gl_color_material_enabled = json_object_dotget_boolean(config_data, "gl_color_material_enabled");
		gl_texture_2D_enabled = json_object_dotget_boolean(config_data, "gl_texture_2D_enabled");
		gl_wireframe_enabled = json_object_dotget_boolean(config_data, "gl_wireframe_enabled");
		hard_poly_enabled = json_object_dotget_boolean(config_data, "hard_poly_enabled");
		frustum_culling = json_object_dotget_boolean(config_data, "frustum_culling");


		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);

		//Create context
		context = SDL_GL_CreateContext(App->window->window);
		if (context == NULL)
		{
			LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}

		//Init Glew
		GLenum glew_init = glewInit();
		if (glew_init != 0)
		{
			LOG("GLOW ERROR: could not init %s\n", glewGetErrorString(glew_init));
			ret = false;
		}
		else
			LOG("Glew succesfully init %s", glewGetString(GLEW_VERSION));

		if (ret == true)
		{
			//Use Vsync
			if (App->window->vsync && SDL_GL_SetSwapInterval(1) < 0)
				LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

			//Initialize Projection Matrix
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			//Check for error
			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
			{
				LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
				ret = false;
			}

			//Initialize Modelview Matrix
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			//Check for error
			error = glGetError();
			if (error != GL_NO_ERROR)
			{
				LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
				ret = false;
			}

			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glClearDepth(1.0f);

			//Initialize clear color
			glClearColor(0.0f, 0.0f, 0.0f, 1.f);
			//Initialize alpha
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//Check for error
			error = glGetError();
			if (error != GL_NO_ERROR)
			{
				LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
				ret = false;
			}

			GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);			

			GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

			GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

			gl_depth_enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			gl_cull_face_enabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
			gl_lighting_enabled ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
			gl_color_material_enabled ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
			gl_texture_2D_enabled ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
			gl_wireframe_enabled ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			hard_poly_enabled ? glShadeModel(GL_FLAT) : glShadeModel(GL_SMOOTH);			
		}
	}

	// Projection matrix for
	OnResize(App->window->window_width, App->window->window_height);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float real_dt, float game_dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(App->camera->frustum.ViewProjMatrix().Transposed().ptr());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float real_dt, float game_dt)
{
	//Call all draw operative

	plane.Render();

	//Render lights
	RenderLights();

	//Draw Geometries
	DrawGeometry();

	//Draw gizmo OBBS
	DrawOBBS();

	//Draw gizmo spheres
	DrawSpheres();


	//Draw Debug Axis
	glLineWidth(3.f);
	glBegin(GL_LINES);

	//x
	glColor3f(3.0f, 0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(3, 0, 0);

	//y
	glColor3f(0.0f, 3.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 3, 0);

	//z
	glColor3f(0.0f, 0.0f, 3.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 3);

	glLineWidth(1.0);


	
	//Draw debug
	
	DrawCameraFrustums();
	DrawDebugBoxes();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();

	//Draw ImGui
	App->editor->DrawUI();
	

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	App->window->window_height = height;
	App->window->window_width = width;
	App->camera->SetAspectRatio((float)width / (float)height);
	App->window->SetAspectRatio((float)width / (float)height);
}

void ModuleRenderer3D::ChangeBackgroundColor(Color c)
{
	glClearColor(c.r, c.g, c.b, c.a);
}


bool ModuleRenderer3D::SaveConfig(JSON_Object* config_data)
{
	LOG("Saving data to config--------");

	//Save renderer data

	json_object_dotset_boolean(config_data, "gl_depth_enabled", gl_depth_enabled);
	json_object_dotset_boolean(config_data, "gl_cull_face_enabled", gl_cull_face_enabled);
	json_object_dotset_boolean(config_data, "gl_lighting_enabled", gl_lighting_enabled);
	json_object_dotset_boolean(config_data, "gl_color_material_enabled", gl_color_material_enabled);
	json_object_dotset_boolean(config_data, "gl_texture_2D_enabled", gl_texture_2D_enabled);
	json_object_dotset_boolean(config_data, "gl_wireframe_enabled", gl_wireframe_enabled);
	json_object_dotset_boolean(config_data, "hard_poly_enabled", hard_poly_enabled);
	json_object_dotset_boolean(config_data, "frustum_culling", frustum_culling);

	return true;
}

// Draw geometry-----------------------------------------------------------
void ModuleRenderer3D::SetMeshToDraw(ComponentMesh * m)
{
	meshes_to_draw.push(m);
}

void ModuleRenderer3D::SetSphereToDraw(math::Sphere s)
{
	spheres_to_draw.push(s);
}

void ModuleRenderer3D::DrawGeometry()
{
	if(frustum_culling)
		SendQuadTreeGameObjectsToPaint(&App->scene->scene_quadtree.root);
	
	
	while (meshes_to_draw.empty() == false)
	{
		if (frustum_culling)
		{
			if (CheckFrustumCulling(meshes_to_draw.front()))
				meshes_to_draw.front()->Draw();
		}
		else meshes_to_draw.front()->Draw();
		
		
		meshes_to_draw.pop();		
	}

}



void ModuleRenderer3D::SendQuadTreeGameObjectsToPaint(QuadTreeNodeObj* node)
{
	if (active_camera->frustum.Contains(node->box))
	{
		
		for (std::vector<GameObject*>::iterator obj_it = node->game_objects.begin(); obj_it != node->game_objects.end(); obj_it++)
		{

			std::vector<Component*> meshes = (*obj_it)->GetAllComponentOfType(COMPONENT_MESH);
			for(int i = 0;i<meshes.size();i++)
			{ 
				if (active_camera->frustum.Contains(*((ComponentMesh*)meshes[i])->GetTransformedBox()))
					App->renderer3D->SetMeshToDraw(((ComponentMesh*)meshes[i]));
			}
		}

		for (std::vector<QuadTreeNodeObj*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
		{
			SendQuadTreeGameObjectsToPaint((*it));
		}
	}
	
}

void ModuleRenderer3D::SetLightToRender(Light* l)
{
	lights_to_render.push(l);
}

// Draw primitives -----------------------------------------------------------
void ModuleRenderer3D::SetBoxToDraw(math::AABB b) 
{
	boxes_to_draw.push(b);
}

void ModuleRenderer3D::SetBoxToDraw(math::Frustum * b)
{
	frustums_to_draw.push(b);
}

void ModuleRenderer3D::SetBoxToDraw(math::OBB b)
{
	obbs_to_draw.push(b);
}

void ModuleRenderer3D::DrawDebugBoxes()
{
	glLineWidth(0.1);
	glColor3f(0.0f, 3.0f, 0.0f);

	while (boxes_to_draw.empty() == false)
	{

		//Draw to main faces
		for (int i = 0; i <= 4; i += 4)
		{
			glVertex3f(boxes_to_draw.front().CornerPoint(i).x, boxes_to_draw.front().CornerPoint(i).y, boxes_to_draw.front().CornerPoint(i).z);
			glVertex3f(boxes_to_draw.front().CornerPoint(i+1).x, boxes_to_draw.front().CornerPoint(i+1).y, boxes_to_draw.front().CornerPoint(i+1).z);


			glVertex3f(boxes_to_draw.front().CornerPoint(i+2).x, boxes_to_draw.front().CornerPoint(i+2).y, boxes_to_draw.front().CornerPoint(i+2).z);
			glVertex3f(boxes_to_draw.front().CornerPoint(i + 3).x, boxes_to_draw.front().CornerPoint(i + 3).y, boxes_to_draw.front().CornerPoint(i + 3).z);

			glVertex3f(boxes_to_draw.front().CornerPoint(i).x, boxes_to_draw.front().CornerPoint(i).y, boxes_to_draw.front().CornerPoint(i).z);
			glVertex3f(boxes_to_draw.front().CornerPoint(i + 2).x, boxes_to_draw.front().CornerPoint(i + 2).y, boxes_to_draw.front().CornerPoint(i + 2).z);

			glVertex3f(boxes_to_draw.front().CornerPoint(i+1).x, boxes_to_draw.front().CornerPoint(i+1).y, boxes_to_draw.front().CornerPoint(i+1).z);
			glVertex3f(boxes_to_draw.front().CornerPoint(i + 3).x, boxes_to_draw.front().CornerPoint(i + 3).y, boxes_to_draw.front().CornerPoint(i + 3).z);
		}

		//Draw restant lines
		for (int i = 0; i < 4; i++)
		{
			glVertex3f(boxes_to_draw.front().CornerPoint(i+4).x, boxes_to_draw.front().CornerPoint(i+4).y, boxes_to_draw.front().CornerPoint(i+4).z);
			glVertex3f(boxes_to_draw.front().CornerPoint(i).x, boxes_to_draw.front().CornerPoint(i).y, boxes_to_draw.front().CornerPoint(i).z);
		}

		boxes_to_draw.pop();
	}
}

void ModuleRenderer3D::DrawCameraFrustums()
{
	glColor3f(0.0f, 1.0f, 1.0f);

	while (frustums_to_draw.empty() == false)
	{

		float3 point = frustums_to_draw.front()->CornerPoint(1);

		//Draw to main faces
		for (int i = 0; i <= 4; i += 4)
		{
			glVertex3f(frustums_to_draw.front()->CornerPoint(i).x, frustums_to_draw.front()->CornerPoint(i).y, frustums_to_draw.front()->CornerPoint(i).z);
			glVertex3f(frustums_to_draw.front()->CornerPoint(i + 1).x, frustums_to_draw.front()->CornerPoint(i + 1).y, frustums_to_draw.front()->CornerPoint(i + 1).z);


			glVertex3f(frustums_to_draw.front()->CornerPoint(i + 2).x, frustums_to_draw.front()->CornerPoint(i + 2).y, frustums_to_draw.front()->CornerPoint(i + 2).z);
			glVertex3f(frustums_to_draw.front()->CornerPoint(i + 3).x, frustums_to_draw.front()->CornerPoint(i + 3).y, frustums_to_draw.front()->CornerPoint(i + 3).z);

			glVertex3f(frustums_to_draw.front()->CornerPoint(i).x, frustums_to_draw.front()->CornerPoint(i).y, frustums_to_draw.front()->CornerPoint(i).z);
			glVertex3f(frustums_to_draw.front()->CornerPoint(i + 2).x, frustums_to_draw.front()->CornerPoint(i + 2).y, frustums_to_draw.front()->CornerPoint(i + 2).z);

			glVertex3f(frustums_to_draw.front()->CornerPoint(i + 1).x, frustums_to_draw.front()->CornerPoint(i + 1).y, frustums_to_draw.front()->CornerPoint(i + 1).z);
			glVertex3f(frustums_to_draw.front()->CornerPoint(i + 3).x, frustums_to_draw.front()->CornerPoint(i + 3).y, frustums_to_draw.front()->CornerPoint(i + 3).z);
		}

		//Draw restant lines
		for (int i = 0; i < 4; i++)
		{
			glVertex3f(frustums_to_draw.front()->CornerPoint(i + 4).x, frustums_to_draw.front()->CornerPoint(i + 4).y, frustums_to_draw.front()->CornerPoint(i + 4).z);
			glVertex3f(frustums_to_draw.front()->CornerPoint(i).x, frustums_to_draw.front()->CornerPoint(i).y, frustums_to_draw.front()->CornerPoint(i).z);
		}

		frustums_to_draw.pop();
	}
}

void ModuleRenderer3D::DrawSpheres()
{
	glLineWidth(3.0f);
	glColor3f(2.0f, 2.0f, 2.0f);

	while (spheres_to_draw.empty() == false)
	{		
		float radius = spheres_to_draw.front().r;
		float3 pos = spheres_to_draw.front().pos;
		float degInRad = 360.0f / 12;
		degInRad *= DEGTORAD;
		glBegin(GL_LINE_LOOP);
		for (unsigned int i = 0; i < 12; i++)
			glVertex3f(cos(degInRad * i) * radius + pos.x, pos.y, sin(degInRad * i) * radius + pos.z);
		glEnd();
		glBegin(GL_LINE_LOOP);
		for (unsigned int i = 0; i < 12; i++)
			glVertex3f(cos(degInRad * i) * radius + pos.x, sin(degInRad * i) * radius +pos.y, pos.z);
		glEnd();
		glBegin(GL_LINE_LOOP);
		for (unsigned int i = 0; i < 12; i++)
			glVertex3f(pos.x, sin(degInRad * i) * radius + pos.y, cos(degInRad * i) * radius + pos.z);
		glEnd();

		spheres_to_draw.pop();
	}
	glLineWidth(1.0f);
}

void ModuleRenderer3D::DrawOBBS()
{
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 1.0f);

	while (obbs_to_draw.empty() == false)
	{

		for (uint i = 0; i < 12; i++) 
		{ 
			glVertex3f(obbs_to_draw.front().Edge(i).a.x, obbs_to_draw.front().Edge(i).a.y, obbs_to_draw.front().Edge(i).a.z);
			glVertex3f(obbs_to_draw.front().Edge(i).b.x, obbs_to_draw.front().Edge(i).b.y, obbs_to_draw.front().Edge(i).b.z); 
		}

		obbs_to_draw.pop();
	}
	glEnd();
}

void ModuleRenderer3D::RenderLights()
{
	while (lights_to_render.empty() == false)
	{
		lights_to_render.back()->Render();
		lights_to_render.pop();
	}
}

bool ModuleRenderer3D::CheckFrustumCulling(const ComponentMesh * m)
{
	return active_camera->frustum.Contains(*m->GetTransformedBox());	
}

