#pragma once
#include "Component.h"
#include "GameObject.h"
#include "ResourceTexture.h"

class ComponentMaterial :public Component
{
public:
	ComponentMaterial(GameObject*);
	~ComponentMaterial();

	void OnEditor();

	bool IsActive() { return active; }

	uint GetTextureID() const
	{
		return texture ? texture->GetTextureId() : 0;
	}
	std::string name;

	std::string GetTextureName()const
	{
		return texture ? texture->GetTextureName() : "- none -";
	}

	void SetTexture(ResourceTexture* t)
	{
		if (texture)
			texture->DecreaseInstancies();
		texture = t;
		if (texture)
			texture->IncreaseInstancies();
	}


	//Scene serialization------------------------
	uint PrepareToSave() const;
	void Save(char* &cursor) const;



private:
	
	ResourceTexture* texture = nullptr;
	bool want_to_change_texture= false;
	std::string library_textures_path;
};

