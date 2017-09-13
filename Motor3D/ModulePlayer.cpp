#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	playerTimer.Start();
	LOG("Loading player");
	gravityChange = false;
	startRotation = false;
	record = false;
	state = BOTTOM;
	totalRotation = 0.0f;
	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(3, 1.5, 6);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 5.88f;
	car.suspensionCompression = 5.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 2.6f;
	float wheel_radius = 1.5f;
	float wheel_width = 0.8f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width + 1.0f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width - 1.0f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width + 1.0f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width - 1.0f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 12, 0);

	mat4x4 matrix;
	vehicle->GetTransform(&matrix);
	App->camera->ViewVector = {0,7,0};

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModulePlayer::RotateCar()
{
	float rotateValue = ROTATION_VALUE;
	totalRotation += rotateValue;

	mat4x4 rotated_matrix;
	rotated_matrix.rotate(rotateValue, {0, 0, 1});

	mat4x4 vehicle_matrix;
	App->player->vehicle->GetTransform(&vehicle_matrix);

	mat4x4 result = vehicle_matrix*rotated_matrix;
	vehicle->SetTransform(result.M);

}

bool ModulePlayer::CheckContact()
{
	bool ret = false;

	mat4x4 vehicle_matrix;
	App->player->vehicle->GetTransform(&vehicle_matrix);
	vec3 position = vehicle_matrix.translation();
	btVector3 btFrom;
	btVector3 btTo;
	if (totalRotation == ROTATION_LIMIT) {
		btFrom = btVector3(position.x, position.y - 1.5, position.z);
		btTo = btVector3(position.x, position.y + 2, position.z);
	}
	else if (totalRotation == 0.0f) {
		btFrom = btVector3(position.x, position.y + 1.5, position.z);
		btTo = btVector3(position.x, position.y - 2, position.z);
	}
	else {
		return ret;
	}
	btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

	
	App->physics->GetWorld()->rayTest(btFrom, btTo, res);
	if (App->physics->isDebug()) {
		App->physics->GetWorld()->getDebugDrawer()->drawLine(btFrom, btTo, btVector3(1, 1, 1));
	}
	if (res.hasHit()) {
		ret = true;
		if (totalRotation == ROTATION_LIMIT)
		{
			state = TOP;
		}
		else if (totalRotation == 0.0f)
		{
			state = BOTTOM;
		}
	}
	return ret;
}

float ModulePlayer::ReadTime()
{
	return (float)playerTimer.Read() / 1000.0f;
}

void ModulePlayer::ResetCar()
{
	if(win==false)
	App->audio->PlayFx(App->scene_intro->lose_song);

	if (App->physics->GetGravityState() == false)
	{
		App->physics->ChangeGravity();

	}
	state = BOTTOM;
	mat4x4 matrix;

	vehicle->SetTransform(matrix.M);
	App->camera->ViewVector.y = 7;

	vehicle->GetBody()->setAngularVelocity({ 0, 0, 0 });
	vehicle->GetBody()->setLinearVelocity({ 0, 0, 0 });
	vehicle->SetPos(0, 2, 0);
	totalRotation = 0;
	playerTimer.Start();
}

void ModulePlayer::SetRecord(float newTime)
{
	if (record == false) 
	{
		bestTime = newTime;
		record = true;
	}
	else 
	{
		if (bestTime > newTime) 
		{
			bestTime = newTime;
		}
	}
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{

	/*App->camera->X = rotate(App->camera->X, 5, vec3(0.0f, 1.0f, 0.0f));
	App->camera->Y = rotate(App->camera->Y, 5, vec3(0.0f, 1.0f, 0.0f));
	App->camera->Z = rotate(App->camera->Z, 5, vec3(0.0f, 1.0f, 0.0f));*/

	//App->camera->Position = { t.getX(),  t.getY() , t.getZ()-10 };
		

	turn = acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && vehicle->GetKmh()<MAX_VELOCITY)
	{
		if (vehicle->GetKmh()>= 0.0f) {
			acceleration = MAX_ACCELERATION;
		}
		else {
			brake = BRAKE_POWER;
		}
	}
	else if(vehicle->GetKmh() >= MAX_VELOCITY)
	{
		acceleration = 0;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (state == BOTTOM) {
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}
		else if (state == TOP) {
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}
	}

	else if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (state == BOTTOM) {
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}
		else if (state == TOP) {
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}
	}

	if (onFloor == true && state != CHANGING)
	{
		//will change "gravityChange != true" for "onFloor==true"
		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		{
			state = CHANGING;
			gravityChange = true;
			rot_timer.Start();
			if (App->physics->GetGravityState() == true)
			{
				vehicle->Push(0, 8000, 0);
			}
			else
				vehicle->Push(0, -8000, 0);

			App->physics->ChangeGravity();
			startRotation = true;
			onFloor = false;
		}
	}


	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() < 0.0f) {
			acceleration = -MAX_ACCELERATION;
		}
		else {
			brake = BRAKE_POWER;
		}
		
	}
	
	//Check car is on floor
	if (gravityChange != true) {
		onFloor = CheckContact();
	}
	
	/*
	if (gravityChange == true) {
		gravityChange = vehicle->vehicle->m_wheelInfo[0].m_raycastInfo.m_isInContact;
	}
	*/
	mat4x4 vehicle_matrix;
	App->player->vehicle->GetTransform(&vehicle_matrix);

	//Reset car
	if (vehicle_matrix.translation().y <= LOW_LIMIT || vehicle_matrix.translation().y > UP_LIMIT)
	{
		ResetCar();
	}
	

	
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	vehicle->Render();

	char title[80];

	
	if (win == false)
	{
		float time = ReadTime();	
		sprintf_s(title, "%.1f Km/h, Timer %.2f - Best time:%.2f ", vehicle->GetKmh(), time, bestTime);
	}
	else
	{
		sprintf_s(title, "You won! Your time:%.2f - Best time:%.2f", last_time, bestTime);
		if (win_timer.Read() > 7000)
		{
			ResetCar();
			win = false;

		}
	}

	
	

	App->window->SetTitle(title);
	
	
	
	if (startRotation == true) 
	{
		RotateCar();
		LOG("TIME: %i", rot_timer.Read());
		
		if (rot_timer.Read() > 10)
		{
			if (App->physics->GetGravityState() != true)
			{
				App->camera->ViewVector.y -= 0.5;
				rot_timer.Start();
			}
			else
			{
				App->camera->ViewVector.y += 0.5;
				rot_timer.Start();
			}				
		}
	

		if (totalRotation == ROTATION_LIMIT) 
		{
			gravityChange=startRotation = false;
			
		}
		else if (totalRotation == ROTATION_LIMIT * 2) 
		{
			totalRotation = 0.0f;
			gravityChange=startRotation = false;
			
		}
	}
	return UPDATE_CONTINUE;
}



