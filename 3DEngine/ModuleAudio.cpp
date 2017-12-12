#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "ModuleAudio.h"
#include "ModuleCamera3D.h"



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

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;	
	std::wstring base_path = converter.from_bytes((App->fs->GetAssetDirectory() + "Soundbanks/").c_str());

	bool ret = Wwished::InitWwished(base_path.c_str(), "English(US)");

	Wwished::Utility::LoadBank("Main.bnk");
		
	return ret;
}

bool ModuleAudio::Start()
{
	//Create listener for the module camera
	float3 cam_up = App->camera->frustum.up;
	float3 cam_front = App->camera->frustum.front;
	float3 cam_pos = App->camera->frustum.pos;

	camera_listener = Wwished::Utility::CreateEmitter(0, "Camera_Listener", cam_pos.x, cam_pos.y, cam_pos.z, true);
	camera_listener->SetPosition(cam_pos.x, cam_pos.y, cam_pos.z, cam_front.x, cam_front.y, cam_front.z, cam_up.x, cam_up.y, cam_up.z);

	return true;
}

update_status ModuleAudio::PostUpdate(float real_dt, float game_dt)
{
	//Update camera listener
	//Create listener for the module camera
	float3 cam_up = App->camera->frustum.up;
	float3 cam_front = App->camera->frustum.front;
	float3 cam_pos = App->camera->frustum.pos;
	camera_listener->SetPosition(cam_pos.x, cam_pos.y, cam_pos.z, cam_front.x, cam_front.y, cam_front.z, cam_up.x, cam_up.y, cam_up.z);

	Wwished::ProcessAudio();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Unloading Wwished library");
	delete camera_listener;
	return Wwished::CloseWwished();
}

Wwished::SoundEmitter * ModuleAudio::CreateSoundEmitter(const char * name, float3 position)
{
	Wwished::SoundEmitter* ret = Wwished::Utility::CreateEmitter(last_go_id++, name, position.x, position.y, position.z);
	sound_emitters.push_back(ret);
	return ret;
}

void ModuleAudio::DeleteSoundEmitter(Wwished::SoundEmitter * emit)
{
	sound_emitters.remove(emit);
	delete emit;
}

void ModuleAudio::ChangeState(const char * group, const char * new_state)
{
	Wwished::Utility::ChangeState(group, new_state);
}
