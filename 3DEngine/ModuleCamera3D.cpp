#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"


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

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else {
		//LoadData from Config
		JSON_Value * config_data = json_parse_file("config.json");

		assert(config_data != nullptr);

		JSON_Object * object_data = json_value_get_object(config_data);
		JSON_Object * application_data = json_object_dotget_object(object_data, "App");
		JSON_Object * camera_data = json_object_dotget_object(application_data, "camera");
		Position.x = json_object_dotget_number(camera_data, "x");
		Position.y = json_object_dotget_number(camera_data, "y");
		Position.z = json_object_dotget_number(camera_data, "z");		
	}
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

	Cube_prim c;
	c.SetPos(Reference.x, Reference.y, Reference.z);
	c.Render();
		

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

	return true;
	
}


