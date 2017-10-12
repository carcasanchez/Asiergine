#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "Geometry.h"


ModuleCamera3D::ModuleCamera3D( bool start_enabled) : Module(start_enabled)
{

	ResetCamera();

	name = "camera";
}

ModuleCamera3D::~ModuleCamera3D()
{}
//---------------------------------------------------------------
bool ModuleCamera3D::Init(const JSON_Object* config_data)
{
	bool ret = true;

	//LoadData from Config
	assert(config_data != nullptr);

	Position.x = json_object_dotget_number(config_data, "x");
	Position.y = json_object_dotget_number(config_data, "y");
	Position.z = json_object_dotget_number(config_data, "z");
	Reference.x = json_object_dotget_number(config_data, "reference_x");
	Reference.y = json_object_dotget_number(config_data, "reference_y");
	Reference.z = json_object_dotget_number(config_data, "reference_z");

	camera_speed = json_object_dotget_number(config_data, "speed");
	camera_sensitivity = json_object_dotget_number(config_data, "sensitivity");


	return ret;
}
// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	Look(Position, Reference, true);

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{	
	ControlCamera(dt);

	// Recalculate matrix -------------
	CalculateViewMatrix();		

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}


void ModuleCamera3D::ControlCamera(float dt)
{


	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		Move(Z*-camera_speed*dt);

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		Move(Z*camera_speed*dt);

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		Move(X*-camera_speed*dt);

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		Move(X*camera_speed*dt);

	int x = App->input->GetMouseXMotion();
	int y = App->input->GetMouseYMotion();

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		//FP control
		if (x != 0)
		{
			X = rotate(X, -camera_sensitivity*x*dt, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, -camera_sensitivity*x*dt, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, -camera_sensitivity*x*dt, vec3(0.0f, 1.0f, 0.0f));
		}

		if (y != 0)
		{
			Y = rotate(Y, -camera_sensitivity*y*dt, X);
			Z = rotate(Z, -camera_sensitivity*y*dt, X);
			
			//Cap Camera Y axis
			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		//Adjust reference
		vec3 distance = Reference - Position;
		Reference = Position - (Z * length(distance));
		
		/*	Pan the Camera
			{
				if (y != 0)
				{
					Move(Y*camera_speed*dt*y);

				}
				if (x != 0)
				{
					Move(X*-camera_speed*dt*x);
				}
			}
			*/

	}
	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		//Orbital Control
		Position -= Reference;

		if (x != 0)
		{
			X = rotate(X, -(float)x * camera_sensitivity*dt , vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, -(float)x * camera_sensitivity*dt , vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, -(float)x * camera_sensitivity*dt , vec3(0.0f, 1.0f, 0.0f));
		}

		if (y != 0)
		{
			Y = rotate(Y, -(float)y * camera_sensitivity*dt, X);
			Z = rotate(Z, -(float)y * camera_sensitivity*dt, X);
		}

		if (Y.y > 0.0f)
			Position = Reference + Z * length(Position);	

		//Cap Camera Y axis
		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

		//Move camera in the local Z axis
		if (App->input->GetMouseZ() == 1)
		{
			Position -= Z;
		}
		else if (App->input->GetMouseZ() == -1)
		{
			Position += Z;
		}
		//Reset Camera to 0, 0, 0 
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			ResetCamera();
		}
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			AdaptToGeometry();
		}	
}

void ModuleCamera3D::ResetCamera()
{
	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(5.0f, 5.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
	

	LookAt(Reference);

	CalculateViewMatrix();
}

bool ModuleCamera3D::SaveConfig(JSON_Object* config_data)
{
	LOG("Saving data to config--------");

	//Save window data

	json_object_dotset_number(config_data, "x", Position.x);
	json_object_dotset_number(config_data, "y", Position.y);
	json_object_dotset_number(config_data, "z", Position.z);
	json_object_dotset_number(config_data, "reference_x", Reference.x);
	json_object_dotset_number(config_data, "reference_y", Reference.y);
	json_object_dotset_number(config_data, "reference_z", Reference.z);

	json_object_dotset_number(config_data, "speed", camera_speed);
	json_object_dotset_number(config_data, "sensitivity", camera_sensitivity);

	return true;
	
}

void ModuleCamera3D::AdaptToGeometry(const Geometry * g)
{
	math::AABB box(float3(0, 0, 0), float3(0, 0, 0));
	std::vector <float3> vertex_array;
	
	for (int i = 0; i < g->num_vertices*3; i+=3)
	{
		vertex_array.push_back(float3(g->vertices[i], g->vertices[i+1], g->vertices[i+2]));			
	}

	box.Enclose(&vertex_array[0], g->num_vertices);


	Position.x = box.maxPoint.x*1.5;
	Position.y = box.maxPoint.y*1.5;
	Position.z = box.maxPoint.z*1.5;

	Reference.x = box.CenterPoint().x;
	Reference.y = box.CenterPoint().y;
	Reference.z = box.CenterPoint().z;


	LookAt(Reference);
}

void ModuleCamera3D::AdaptToGeometry()
{
	if (App->file_system->geometries.empty())
		return;

	std::vector<float3> vertices;

	for (int i = 0; i < App->file_system->geometries.size(); i++)
	{
		//Generate AABBS for each geom in scene
		math::AABB new_aabb(float3(0, 0, 0), float3(0, 0, 0));
		std::vector <float3> vertex_array;

		Geometry* g = App->file_system->geometries[i];
		for (int j = 0; j < g->num_vertices * 3; j += 3)
		{
			vertex_array.push_back(float3(g->vertices[j], g->vertices[j + 1], g->vertices[j + 2]));
		}

		new_aabb.Enclose(&vertex_array[0], g->num_vertices);

		//Stores the 8 vertices of the box in a general array
		for (int j = 0; j < 8; j++)
		{
			vertices.push_back(new_aabb.CornerPoint(j));
		}
	}

	
	//Creates a general AABB 
	math::AABB general(float3(0, 0, 0), float3(0, 0, 0));
	general.Enclose(&vertices[0], vertices.size());
	
	Position.x = general.maxPoint.x*1.5;
	Position.y = general.maxPoint.y*1.5;
	Position.z = general.maxPoint.z*1.5;

	Reference.x = general.CenterPoint().x;
	Reference.y = general.CenterPoint().y;
	Reference.z = general.CenterPoint().z;

	LookAt(Reference);
}


