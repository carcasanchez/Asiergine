#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

enum CarState
{
	BOTTOM,
	CHANGING,
	TOP,
};
struct PhysVehicle3D;

#define MAX_ACCELERATION 6000.0f
#define MAX_VELOCITY 200
#define TURN_DEGREES 5.0f * DEGTORAD
#define BRAKE_POWER 1000.0f
#define ROTATION_VALUE 6.0f
#define ROTATION_LIMIT 180.0f
#define LOW_LIMIT -30
#define UP_LIMIT 80

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
	void RotateCar();
	bool CheckContact();
	float ReadTime();
	void ResetCar();
	void SetRecord(float newTime);
	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;

	bool win = false;
	Timer win_timer;
	float last_time;

private:
	CarState state;
	float totalRotation;
	bool gravityChange;
	bool startRotation;
	bool onFloor=true;
	bool record;
	float bestTime = 0.0f;
	Timer rot_timer;

	Timer playerTimer;

};