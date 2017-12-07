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
	LOG("Loading Audio Mixer");
	bool ret = true;

	Wwished::InitWwished();

	return true;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	
	return true;
}
