#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;
class vec2;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(const JSON_Object* config_data);
	bool CleanUp();

	void SetTitle(const char* title);

	float2 GetWindowPixels();


	bool SwitchVSync();
	void SetAspectRatio(float new_aspect);

	float GetAspectRatio()
	{
		return aspect_ratio;
	}

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	//Json configuration
	int window_width;
	int window_height;
	int screen_size;
	bool vsync;

	std::string title;
	std::string window_state;

	bool SaveConfig(JSON_Object* config_data);

private:
	float aspect_ratio = 0.0;
};

#endif // __ModuleWindow_H__