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

	frustum.pos.x = json_object_dotget_number(config_data, "x");
	frustum.pos.y = json_object_dotget_number(config_data, "y");
	frustum.pos.z = json_object_dotget_number(config_data, "z");
	pivot_point.x = json_object_dotget_number(config_data, "reference_x");
	pivot_point.y = json_object_dotget_number(config_data, "reference_y");
	pivot_point.z = json_object_dotget_number(config_data, "reference_z");

	frustum.type = PerspectiveFrustum;
	frustum.front = float3(0, 0, 1);
	frustum.up = float3(0, 1, 0);
	aspect_ratio = App->window->GetAspectRatio();

	frustum.nearPlaneDistance = 1;
	frustum.farPlaneDistance = 200;

	frustum.verticalFov = 1;
	frustum.horizontalFov = math::Atan(aspect_ratio*math::Tan(frustum.verticalFov / 2)) * 2;

	camera_speed = json_object_dotget_number(config_data, "speed");
	camera_sensitivity = json_object_dotget_number(config_data, "sensitivity");


	return ret;
}
// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	LookAt(pivot_point);

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float real_dt, float game_dt)
{	
	ControlCamera(real_dt);

	return UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3 &Spot)
{
	pivot_point = Spot;
	frustum.front = (pivot_point - frustum.pos).Normalized();
	
	float3 crossYZ = math::Cross(float3(0,1,0), frustum.front).Normalized();
	frustum.up = math::Cross(frustum.front, crossYZ);
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	frustum.pos += Movement;
	pivot_point += Movement;
}

const float * ModuleCamera3D::GetViewMatrixTransposed() const
{
	float4x4 view_m = frustum.ViewMatrix();
	view_m.Transpose();

	return view_m.ptr();
}


const float * ModuleCamera3D::GetProjectionMatrixTransposed() const
{
	return frustum.ProjectionMatrix().Transposed().ptr();
}

void ModuleCamera3D::SetAspectRatio(float new_aspect_ratio)
{
	aspect_ratio = new_aspect_ratio;
	frustum.horizontalFov = math::Atan(new_aspect_ratio*math::Tan(frustum.verticalFov / 2)) * 2;
}



void ModuleCamera3D::ControlCamera(float dt)
{

	if (App->editor->IsInputLocked())
		return;
		
	//Displacement
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		Move(frustum.front*camera_speed*dt);

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		Move(frustum.front*-camera_speed*dt);

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		Move(math::Cross(frustum.front, frustum.up)*-camera_speed*dt);

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		Move(math::Cross(frustum.front, frustum.up)*camera_speed*dt);

	int x = App->input->GetMouseXMotion();
	int y = App->input->GetMouseYMotion();

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			//Pan the Camera			
			if (y != 0)
				Move((frustum.up*camera_speed*dt*y));

			if (x != 0)
				Move((math::Cross(frustum.front, frustum.up)*-camera_speed*dt*x));
		}
		else
		{
			//FP control
			if (x != 0)
			{
				Quat rotation_quat = Quat::RotateY(-camera_sensitivity*dt*x);
				frustum.front = rotation_quat.Transform(frustum.front).Normalized();				
				frustum.up = rotation_quat.Transform(frustum.up).Normalized();
			}

			if (y != 0)
			{
				Quat rotation_quat = Quat::RotateAxisAngle(frustum.WorldRight(), -camera_sensitivity*dt*y);

				float3 new_Y_axis = rotation_quat.Transform(frustum.up).Normalized();

				if (new_Y_axis.y > 0.0f)
				{
					frustum.up = new_Y_axis;
					frustum.front = rotation_quat.Transform(frustum.front).Normalized();
				}

			}

			//Adjust reference
			//float3 distance = pivot_point - frustum.pos;
			//pivot_point = frustum.pos - (frustum.front * distance.Length());
		}
		
	
}
	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		float3 distance = frustum.pos - pivot_point;

		Quat rot_y(frustum.up, -x*camera_sensitivity*dt);
		Quat rot_x(frustum.WorldRight(), -y*camera_sensitivity*dt);

		//Calculate point around pivot
		distance = rot_x.Transform(distance);
		distance = rot_y.Transform(distance);

		//Place camera in point
		frustum.pos = distance + pivot_point;

		//Reorient camera to pivot
		LookAt(pivot_point);
	}

	//Move camera in the local Z axis
	if (App->input->GetMouseZ() == 1)
		frustum.pos += frustum.front * camera_speed * 100;
	else if (App->input->GetMouseZ() == -1)
		frustum.pos -= frustum.front * camera_speed * 100;
		
	//Reset Camera to 0, 0, 0 
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		ResetCamera();

	//Adapt camera to geometry in scene
	/*if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		AdaptToGeometry();*/
		
}

void ModuleCamera3D::ResetCamera()
{
	frustum.pos = float3(5.0f, 5.0f, 5.0f);
	pivot_point = float3(0.0f, 0.0f, 0.0f);
	
	LookAt(pivot_point);
}

/*float4x4 ModuleCamera3D::CalculateProjectionMatrix(float vertical_fov, float aspect_ratio) const
{
	float4x4 projection_matrix;
	float cotan_y = 1.0f / math::Tan(vertical_fov * (float)PI / 360.0f);

	projection_matrix.ptr()[0] = cotan_y / aspect_ratio;
	projection_matrix.ptr()[5] = cotan_y;
	projection_matrix.ptr()[10] = (near_distance + far_distance) / (near_distance - far_distance);
	projection_matrix.ptr()[11] = -1.0f;
	projection_matrix.ptr()[14] = 2.0f * near_distance * far_distance / (near_distance - far_distance);
	projection_matrix.ptr()[15] = 0.0f;

	return projection_matrix;
}*/



bool ModuleCamera3D::SaveConfig(JSON_Object* config_data)
{
	LOG("Saving data to config--------");

	//Save window data

	json_object_dotset_number(config_data, "x", frustum.pos.x);
	json_object_dotset_number(config_data, "y", frustum.pos.y);
	json_object_dotset_number(config_data, "z", frustum.pos.z);
	json_object_dotset_number(config_data, "reference_x", pivot_point.x);
	json_object_dotset_number(config_data, "reference_y", pivot_point.y);
	json_object_dotset_number(config_data, "reference_z", pivot_point.z);

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

	frustum.pos.x = new_aabb.maxPoint.x + 5;
	frustum.pos.y = new_aabb.maxPoint.y + 5;
	frustum.pos.z = new_aabb.maxPoint.z + 5;

	pivot_point.x = new_aabb.CenterPoint().x;
	pivot_point.y = new_aabb.CenterPoint().y;
	pivot_point.z = new_aabb.CenterPoint().z;

	LookAt(pivot_point);
}


