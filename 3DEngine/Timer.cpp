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
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
Uint64 Timer::ReadTicks()
{
	return SDL_GetPerformanceCounter() - started_at;
}

double Timer::ReadMS()
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}


