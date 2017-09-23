#pragma once

#include "parson.h"
#include "Profiler.h"


class Application;
struct PhysBody3D;

class Module
{
private :
	bool enabled;
	
protected:
	std::string name;

public:

	Profiler start_profiler;
	Profiler pre_update_profiler;
	Profiler update_profiler;
	Profiler post_update_profiler;


	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init(const JSON_Object* config_data)
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool SaveConfig(const JSON_Object* config_data)
	{
		return true;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}



	std::string GetModuleName() 
	{
		return name;
	}
};