#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"


ModuleCamera3D::ModuleCamera3D( bool start_enabled) : Module(start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
	pivotal_point = vec3(0.0f, 0.0f, 0.0f);


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
	//FP Control
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		Move(Z*-camera_speed*dt);
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		Move(Z*camera_speed*dt);
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		Move(X*-camera_speed*dt);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		Move(X*camera_speed*dt);
	}


	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int x = App->input->GetMouseXMotion();	
		int y = App->input->GetMouseYMotion();
		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
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
		else
		{
			if (y != 0)
			{
				if (y > 0)
					Position += Y*camera_sensitivity * dt*y;
				else if (y < 0)
					Position += Y*camera_sensitivity * dt*y;

				Look(Position, Reference, true);
			}
			if (x != 0)
			{
				if (x > 0)
					Position -= X*camera_sensitivity * dt*x;
				else if (x < 0)
					Position -= X*camera_sensitivity * dt*x;

				Look(Position, Reference, true);
			}
		}


	}


	if (App->input->GetMouseZ() == 1)
	{
		Position -= Z;
	}
	else if (App->input->GetMouseZ() == -1)
	{
		Position += Z;
	}

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