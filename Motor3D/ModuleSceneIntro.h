#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#define TOPFLOR_Y 27
#define RAMP_SCALE 14
#define MIDDLE_SCALE 13.5
#define MAX_SNAKE 2
#define OBSTACLE_SCALE 7
#define FLOOR_WIDTH 30
#define FLOOR_HEIGHT 48
enum FLOOR_STYLE {
	TOP_FLOOR,
	BOTTOM_FLOOR,
	BOTTOM_OBSTACLE_FLOOR,
	TOP_OBSTACLE_FLOOR,
	WALL,
	MIDDLE_FLOOR_B,
	MIDDLE_FLOOR_T,
	BOTTOM_TO_MIDDLE,
	MIDDLE_TO_BOTTOM,
	MIDDLE_TO_TOP,
	TOP_TO_MIDDLE,
	SLIDER_BOTTOM,
	SLIDER_TOP,
	GOAL_LEFT,
	GOAL_RIGHT,
	EMPTY_FLOOR,
};
struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void CreateFloor(vec3 scale, int posX, int posZ,FLOOR_STYLE floor1);
public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	FLOOR_STYLE floorStyle;
	p2DynArray<PhysBody3D*> pb_cubes;
	p2DynArray<Cube>s_cubes;


	PhysBody3D* pb_victory;
	Cube s_victory;


	int victory_song;
	int lose_song;
};
