#include <stdlib.h>
#include "Application.h"
#include "Globals.h"





#include "SDL/include/SDL.h"

#pragma comment( lib, "3DEngine/SDL/libx86/SDL2.lib" )
#pragma comment( lib, "3DEngine/SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "Dll_s/Brofiler-1.1.2/ProfilerCore32.lib" )

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", "ASIERGINE");

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	

	while (state != MAIN_EXIT)
	{
		BROFILER_FRAME("Asiergine");
	
		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	LOG("Exiting game '%s'...\n", App->window->title.c_str());

	delete App;
	return main_return;
}