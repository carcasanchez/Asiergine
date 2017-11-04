#pragma once

#include "Globals.h"
#include "Timer.h"


#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
//#include "ModulePhysics3D.h"
#include "ModuleEditor.h"
#include "ModuleImporter.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"

struct HardwareSpecs
{
public:

	//Hardware specs
	SDL_version sdl_version;
	int cpus = 0;
	float system_ram = 0;
	int cpu_chache = 0;

	//caps
	bool rdtsc;
	bool mmx;
	bool sse;
	bool sse2;
	bool sse3;
	bool sse41;
	bool sse42;
	bool three_d_now;
	bool avx;
	bool avx2;
	bool altivec;

};


class Application
{
public:

	std::string app_name;
	std::string org_name;

	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
//	ModulePhysics3D* physics;
	ModuleEditor* editor;
	ModuleImporter* importer;
	ModuleScene* scene;
	ModuleFileSystem* fs;

	bool console_on = false;

	std::list<Module*> list_modules;

	float GetGameSpeed()const { return game_time_modifier; };
	void SetGameSpeed(float game_speed);

	bool IsAppRunning() const
	{
		return running;
	}
	
	bool IsAppPaused() const
	{
		return paused;
	}

	bool PlayApp()
	{
		running = true;
		scene->wants_to_save = true;
		game_time_modifier = last_game_time_modifier;
		return true;
	}

	bool StopApp()
	{
		running = false;
		paused = false;
		last_game_time_modifier = 1;
		game_time_modifier = 0;
		scene->wants_to_load = true;
		return true;
	}

	void PauseApp()
	{
		paused = true;
		last_game_time_modifier = game_time_modifier;
		game_time_modifier = 0;
	}
	
	void UnPauseApp()
	{
		paused = false;
		game_time_modifier = last_game_time_modifier;
	}


private:

	Timer game_clock;

	Timer	ms_timer;
	Timer   second_timer;
	int		frame_count = 0;
	int last_second_frames;
	float last_frame_miliseconds;

	int fps_cap = 0;
	int ms_cap = 0;

	float real_dt;
	float game_dt;

	float game_time_modifier = 0.0f;
	float last_game_time_modifier = 1.0f;

	bool running = false;
	bool paused = false;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	

	void GetFrames(int& frames, float& miliseconds);
	void SetMaxFrames(int max_frames)
	{
		fps_cap = max_frames;
		ms_cap = (max_frames > 0) ? 1000 / max_frames : 0;
	}

	HardwareSpecs system_specs;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	bool SaveConfig();

		
};


extern Application* App;