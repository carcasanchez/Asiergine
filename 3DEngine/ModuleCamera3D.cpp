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
	frustum.farPlaneDistance = 500;

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

	frustum.up.Normalize();
	frustum.front.Normalize();

	CalculatePickRay();
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

void ModuleCamera3D::SetAspectRatio(float new_aspect_ratio)
{
	aspect_ratio = new_aspect_ratio;
	frustum.horizontalFov = math::Atan(new_aspect_ratio*math::Tan(frustum.verticalFov / 2)) * 2;
}



void ModuleCamera3D::ControlCamera(float dt)
{
	App->editor->UnLockGizmos();
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

			App->editor->LockGizmos();
		}
		else
		{
			//FP control
			if (x != 0)
			{
				Quat rotation_quat = Quat::RotateY(-camera_sensitivity*dt*x * DEGTORAD);
				frustum.front = rotation_quat.Transform(frustum.front).Normalized();				
				frustum.up = rotation_quat.Transform(frustum.up).Normalized();
			}

			if (y != 0)
			{
				Quat rotation_quat = Quat::RotateAxisAngle(frustum.WorldRight(), -camera_sensitivity*dt*y * DEGTORAD);

				float3 new_Y_axis = rotation_quat.Transform(frustum.up).Normalized();

				if (new_Y_axis.y > 0.0f)
				{
					frustum.up = new_Y_axis;
					frustum.front = rotation_quat.Transform(frustum.front).Normalized();
				}

			}

			//Adjust reference
			float3 distance = pivot_point - frustum.pos;
			pivot_point = frustum.pos + (frustum.front.Normalized() * distance.Length());
			App->editor->LockGizmos();
		}
		
	
}
	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (x != 0)
		{

			float dist = frustum.pos.Distance(pivot_point);
			Quat rot;

			//rotate camera
			rot.SetFromAxisAngle({ 0,1,0 }, -x*dt*camera_sensitivity * DEGTORAD);
			frustum.Transform(rot);

			//put camera on correct distance
			frustum.pos = pivot_point - (frustum.front * dist);
			
		}

		if (y != 0)
		{
			float dist = frustum.pos.Distance(pivot_point);
			Quat rot;

			//rotate camera
			rot.SetFromAxisAngle(frustum.WorldRight(), dt * -y * camera_sensitivity * DEGTORAD);
			frustum.Transform(rot);

			//rotate camera back if y.y<0
			if (frustum.up.y < 0)
			{
				Quat rot_back;	
				rot_back.SetFromAxisAngle(frustum.WorldRight(), dt * y * camera_sensitivity * DEGTORAD);
				frustum.Transform(rot_back);
			}

			//put camera on correct distance
			frustum.pos = pivot_point - (frustum.front * dist);
		}



		App->editor->LockGizmos();
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
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		AdaptToGeometry(App->editor->GetSelectedGameObject());
	
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

	AABB box = game_object->GetBoundingBox();

	frustum.pos.x = box.maxPoint.x + 5;
	frustum.pos.y = box.maxPoint.y + 5;
	frustum.pos.z = box.maxPoint.z + 5;

	pivot_point.x = box.CenterPoint().x;
	pivot_point.y = box.CenterPoint().y;
	pivot_point.z = box.CenterPoint().z;

	LookAt(pivot_point);
}

//Mouse Picking---------------------------------------------
void ModuleCamera3D::CalculatePickRay()
{
	if (App->editor->IsInputLocked() || App->editor->IsSelectionLocked())
		return;

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
	{
		math::float2 normalized_click_pos;
		math::float2 click_pos;
		click_pos.x = App->input->GetMouseX();
		click_pos.y = App->input->GetMouseY();
	
		normalized_click_pos.x = -(1.0f - (float(click_pos.x) * 2.0f) / App->window->window_width);
		normalized_click_pos.y = 1.0f - (float(click_pos.y) * 2.0f) / App->window->window_height;		
		
		pick_ray = frustum.UnProjectLineSegment(normalized_click_pos.x, normalized_click_pos.y);

		float lenght = pick_ray.Length();
		GameObject* picked_obj = nullptr;

		App->scene->root->CheckMouseRayCollision(pick_ray, lenght, picked_obj);


		if(picked_obj)
			App->editor->SelectObject(picked_obj);
		else
		{
			App->editor->UnselectAll();
		}
	}
}



