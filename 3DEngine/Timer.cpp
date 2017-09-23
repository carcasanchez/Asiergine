// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

Uint64 Timer::frequency = 0;

// ---------------------------------------------
Timer::Timer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetPerformanceCounter() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}

double Timer::ReadMS()
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}


