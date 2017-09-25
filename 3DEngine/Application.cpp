#include "Application.h"
#include "parson.h"

#include ".\mmgr\mmgr.h"

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio(true);
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	//physics = new ModulePhysics3D(this);
	editor = new ModuleEditor();


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	//AddModule(physics);
	AddModule(editor);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while (item != list_modules.rend())
	{
		RELEASE((*item));
		item++;
	}

	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	//Console ON
	console_on = true;
	LOG("Opening Console");


	//LoadData from Config
	JSON_Value * config_data = json_parse_file("config.json");
		
	assert(config_data != nullptr);


	JSON_Object * object_data = json_value_get_object(config_data);
	JSON_Object * application_data = json_object_dotget_object(object_data, "App");
	app_name = json_object_dotget_string(application_data, "name");
	org_name = json_object_dotget_string(application_data, "organization");

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init(object_data);
		item++;
	}
		

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		(*item)->start_profiler.StartComprobation();
		ret = (*item)->Start();
		(*item)->start_profiler.EndComprobation();
		item++;
	}

	window->SetTitle(app_name.c_str());
	
	ms_timer.Start();
	second_timer.Start();

	//Get Hardware specs
	SDL_GetVersion(&system_specs.sdl_version);
	system_specs.cpus = SDL_GetCPUCount();
	system_specs.system_ram = SDL_GetSystemRAM()/1000.0f;
	system_specs.cpu_chache = SDL_GetCPUCacheLineSize();

	system_specs.altivec = SDL_HasAltiVec();
	system_specs.rdtsc = SDL_HasRDTSC();
	system_specs.mmx = SDL_HasMMX();
	system_specs.sse = SDL_HasSSE();
	system_specs.sse2 = SDL_HasSSE2();
	system_specs.sse3 = SDL_HasSSE3();
	system_specs.sse41 = SDL_HasSSE41();
	system_specs.sse42 = SDL_HasSSE42();
	system_specs.three_d_now = SDL_Has3DNow();
	system_specs.avx = SDL_HasAVX();
	/**/

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.ReadMS();
	ms_timer.Start();

	frame_count++;
	
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	
	if (fps_cap > 0 && last_frame_miliseconds < (1000/ fps_cap))
	{
		SDL_Delay(((1000/fps_cap) - int(last_frame_miliseconds)));
	}
	
	if (second_timer.ReadMS() > 1000)
	{
		last_second_frames = frame_count;
		second_timer.Start();
		frame_count = 0;
	}

	last_frame_miliseconds = (float)ms_timer.ReadMS();
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		(*item)->pre_update_profiler.StartComprobation();
		ret = (*item)->PreUpdate(dt);
		(*item)->pre_update_profiler.EndComprobation();
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		(*item)->update_profiler.StartComprobation();
		ret = (*item)->Update(dt);
		(*item)->update_profiler.EndComprobation();
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		(*item)->post_update_profiler.StartComprobation();
		ret = (*item)->PostUpdate(dt);
		(*item)->post_update_profiler.EndComprobation();
		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	SaveConfig();

	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	//Shut up console LOG's
	LOG("Closing Console");
	console_on = false;

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}

void Application::GetFrames(int & frames, float & miliseconds)
{
	frames = last_second_frames;
	miliseconds = last_frame_miliseconds;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

//---------------------------------------------

bool Application::SaveConfig()
{
	JSON_Value * config_file = json_parse_file("config.json");

	if (config_file == nullptr)
	{
		LOG("ERROR: COULD NOT SAVE DATA TO CONFIG");
		return false;
	}

	LOG("Saving data to config--------");

	//Save app data
	JSON_Object * object_data = json_value_get_object(config_file);
	JSON_Object* application_data = json_object_dotget_object(object_data, "App");
	
	json_object_dotset_string(application_data, "name", app_name.c_str());
	json_object_dotset_string(application_data, "organization", org_name.c_str());
	
	//Save modules data
	for (std::list<Module*>::iterator it = list_modules.begin(); it!=list_modules.end(); it++)
	{
		JSON_Object* module_data = json_object_dotget_object(application_data, (*it)->GetModuleName().c_str());
		(*it)->SaveConfig(module_data);	
	}

	json_serialize_to_file(config_file, "config.json");

	return true;

}