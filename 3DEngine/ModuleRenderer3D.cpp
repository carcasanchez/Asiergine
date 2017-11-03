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

			Light first_l;
			first_l.ref = GL_LIGHT0;
			first_l.ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
			first_l.diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
			first_l.SetPos(0.0f, 0.0f, 2.5f);
			first_l.Init();
			lights.push_back(first_l);

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

			lights[0].Active(true);
		}
	}

	// Projection matrix for
	OnResize(App->window->window_width, App->window->window_height);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(std::vector<Light>::iterator it = lights.begin(); it!= lights.end(); it++)
		(*it).Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//Call all draw operative

	//Draw Geometries
	DrawGeometry();


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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

	return true;
}

// Draw geometry-----------------------------------------------------------
void ModuleRenderer3D::SetMeshToDraw(ComponentMesh * m)
{
	meshes_to_draw.push(m);
}

void ModuleRenderer3D::DrawGeometry()
{
	plane.Render();
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

// Draw debug AABB's-----------------------------------------------------------
void ModuleRenderer3D::SetBoxToDraw(math::AABB b) 
{
	boxes_to_draw.push(b);
}

void ModuleRenderer3D::SetBoxToDraw(math::Frustum * b)
{
	frustums_to_draw.push(b);
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

bool ModuleRenderer3D::CheckFrustumCulling(const ComponentMesh * m)
{
	return active_camera->frustum.Contains(*m->GetGameObject()->GetTransformedBox());	
}

