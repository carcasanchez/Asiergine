#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();

	Uint32 Read();
	double ReadMS();

private:

	bool	running;
	Uint64	started_at;
	Uint64	stopped_at;

	static Uint64 frequency;
};

#endif //__TIMER_H__