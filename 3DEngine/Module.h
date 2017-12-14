#pragma once


#include "parson.h"



class Application;
struct PhysBody3D;

class Module
{
private :
	bool enabled;
	
protected:
	std::string name;

public:
	

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

	virtual update_status PreUpdate(float real_dt, float game_dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float real_dt, float game_dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float real_dt, float game_dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool SaveConfig(JSON_Object* config_data)
	{
		return true;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	virtual void OnPlay() {};
	virtual void OnPause() {};
	virtual void OnStop() {};


	std::string GetModuleName() 
	{
		return name;
	}
};