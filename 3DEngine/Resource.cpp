#include "Resource.h"
#include "Application.h"
#include "ModuleResourceManager.h"



Resource::Resource(uint uid, RESOURCE_TYPE type) : UID(uid), type(type)
{
}


Resource::~Resource()
{
}

void Resource::DecreaseInstancies()
{
	
		if (instancies>0)
			instancies--;
		
		if(instancies<=0)
			App->resource_m->SetToDelete(UID);

	
}
