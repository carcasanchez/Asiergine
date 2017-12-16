#include "ComponentReverb.h"
#include "CompTransform.h"
#include "GameObject.h"
#include "Application.h"


ComponentReverb::ComponentReverb(GameObject* g) :Component(g)
{
	type = COMPONENT_REVERB;
}


ComponentReverb::~ComponentReverb()
{
	App->audio->SubstracReverbArea(this);
}

void ComponentReverb::Update(float real_dt, float game_dt)
{
	CompTransform* transf = (CompTransform*)game_object->GetComponentByType(COMPONENT_TRANSFORM);
	if (transf)
	{
		float3 pos = transf->GetTranslation();
		Quat rot = transf->GetRotation();
		float3 scale = transf->GetScale();
		box.pos = pos;
		box.axis[0] = rot.Transform(float3(1, 0, 0));
		box.axis[1] = rot.Transform(float3(0, 1, 0));
		box.axis[2] = rot.Transform(float3(0, 0, 1));
		box.r = scale;

		App->renderer3D->SetBoxToDraw(box);
	}
}

void ComponentReverb::OnEditor()
{
	char* bus_name = new char[41];

	std::copy(target_bus.begin(), target_bus.end(), bus_name);
	bus_name[target_bus.length()] = '\0';

	ImGui::InputText("Target bus", bus_name, 40);
	target_bus = bus_name;

	ImGui::DragFloat("Value", &value, 0.1, 0.0, 12.0, "%.1f");

	delete[] bus_name;

}

bool ComponentReverb::CheckCollision(float3 p) const
{
	return box.Contains(p);
}

uint ComponentReverb::PrepareToSave() const
{
	uint size = 0;

	//size of name
	size += sizeof(uint);
	//name
	size += target_bus.size();
	//value
	size += sizeof(float);

	return size;
}

void ComponentReverb::Save(char *& cursor) const
{
	uint size_of_name = target_bus.size();
	
	//copy size of name
	uint size_of = sizeof(uint);
	memcpy(cursor, &size_of_name, size_of);
	cursor += size_of;

	//copy name
	size_of = size_of_name;
	memcpy(cursor, target_bus.c_str(), size_of);
	cursor += size_of;

	//copy value
	size_of = sizeof(float);
	memcpy(cursor, &value, size_of);
	cursor += size_of;
}
