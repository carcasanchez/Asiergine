#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "ModuleAudio.h"
#include "wwished.h"



ModuleAudio::ModuleAudio( bool start_enabled) : Module(start_enabled)
{
	name = "audio";
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init(const JSON_Object* config_data)
{
	LOG("Loading Wwished library");
	return Wwished::InitWwished(".", "English(US)");
}

update_status ModuleAudio::Update(float real_dt, float game_dt)
{
	Wwished::ProcessAudio();
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Unloading Wwished library");
	return Wwished::CloseWwished();
}
