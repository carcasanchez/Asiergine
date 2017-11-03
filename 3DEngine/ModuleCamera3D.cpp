#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"



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

	if (App->editor->IsInputLocked())
		return;
		
	//Displacement
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
		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			//Pan the Camera			
			if (y != 0)
				Move(Y*camera_sensitivity*dt*y);

			if (x != 0)
				Move(X*-camera_sensitivity*dt*x);
		}
		else
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
					Y = rotate(Y, (float)y * camera_sensitivity*dt, X);
					Z = rotate(Z, (float)y * camera_sensitivity*dt, X);
				}

			}

			//Adjust reference
			vec3 distance = Reference - Position;
			Reference = Position - (Z * length(distance));
		}
		
	
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

		//Cap Camera Y axis
		if (Y.y < 0.0f)
		{
			Y = rotate(Y, (float)y * camera_sensitivity*dt, X);
			Z = rotate(Z, (float)y * camera_sensitivity*dt, X);
		}
		
		//Locate camera around reference
		if (Y.y > 0.0f)
			Position = Reference + Z * length(Position);
	}

	//Move camera in the local Z axis
	if (App->input->GetMouseZ() == 1)
		Position -= Z;
	else if (App->input->GetMouseZ() == -1)
		Position += Z;
		
	//Reset Camera to 0, 0, 0 
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		ResetCamera();

	//Adapt camera to geometry in scene
	/*if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		AdaptToGeometry();*/
		
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

/*void ModuleCamera3D::AdaptToGeometry()
{
	math::AABB box(float3(0, 0, 0), float3(0, 0, 0));
	std::vector <float3> vertex_array;
	
	for (int i = 0; i < g->num_vertices*3; i+=3)
	{
		vertex_array.push_back(float3(g->vertices[i], g->vertices[i+1], g->vertices[i+2]));			
	}

	box.Enclose(&vertex_array[0], g->num_vertices);


	Position.x = box.maxPoint.x + 5;
	Position.y = box.maxPoint.y + 5;
	Position.z = box.maxPoint.z + 5;

	Reference.x = box.CenterPoint().x;
	Reference.y = box.CenterPoint().y;
	Reference.z = box.CenterPoint().z;


	LookAt(Reference);
}*/

void ModuleCamera3D::AdaptToGeometry(GameObject* game_object)
{
	if (game_object == nullptr)
		return;

	std::vector<float3> vertices;
	ComponentMesh* it = (ComponentMesh*)game_object->GetComponentByType(COMPONENT_MESH);
	
	if (it == nullptr)
		return;

	//Generate AABBS for each geom in scene
	math::AABB new_aabb(float3(0, 0, 0), float3(0, 0, 0));
	std::vector <float3> vertex_array;

	const float* original_vertices = it->GetVertices();

	for (int j = 0; j < it->GetNumVertices() * 3; j += 3)
	{
		vertex_array.push_back(float3(original_vertices[j], original_vertices[j + 1], original_vertices[j + 2]));
	}

	new_aabb.Enclose(&vertex_array[0], it->GetNumVertices());

	//Stores the 8 vertices of the box in a general array
	for (int j = 0; j < 8; j++)
	{
		vertices.push_back(new_aabb.CornerPoint(j));
	}

	Position.x = new_aabb.maxPoint.x + 5;
	Position.y = new_aabb.maxPoint.y + 5;
	Position.z = new_aabb.maxPoint.z + 5;

	Reference.x = new_aabb.CenterPoint().x;
	Reference.y = new_aabb.CenterPoint().y;
	Reference.z = new_aabb.CenterPoint().z;

	LookAt(Reference);
}


