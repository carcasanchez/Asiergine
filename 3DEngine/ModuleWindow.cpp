#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	name = "window";
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(const JSON_Object* config_data)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//LoadData from Config
		JSON_Value * config_data = json_parse_file("config.json");

		assert(config_data != nullptr);


		JSON_Object * object_data = json_value_get_object(config_data);
		JSON_Object * application_data = json_object_dotget_object(object_data, "App");
		JSON_Object * window_data = json_object_dotget_object(application_data, "window");
		window_width = json_object_dotget_number(window_data, "width");
		window_height = json_object_dotget_number(window_data, "height");
		window_state = json_object_dotget_string(window_data, "window_state");
		title = json_object_dotget_string(window_data, "title");
		screen_size = json_object_dotget_number(window_data, "screen_size");
		vsync = json_object_dotget_boolean(window_data, "vsync");


		//Create window
		int width = window_width * screen_size;
		int height = window_height * screen_size;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(window_state == "fullscreen" )
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(window_state == "resizable")
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(window_state == "fullscreen_desktop")
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");
	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

vec2 ModuleWindow::GetWindowPixels()
{
	vec2 size;
	size.x = window_width * screen_size;
	size.x = window_height * screen_size;
	return size;
}

bool ModuleWindow::SwitchVSync()
{
	if(SDL_GL_SetSwapInterval(vsync) < 0)
		LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

	return true;
}

bool ModuleWindow::SaveConfig(JSON_Object* config_data)
{
	LOG("Saving data to config--------");

	//Save window data

	json_object_dotset_number(config_data, "width", window_width);
	json_object_dotset_number(config_data, "height", window_height);
	json_object_dotset_string(config_data, "window_state", window_state.c_str());
	json_object_dotset_boolean(config_data, "vsync", vsync);

	return true;

}