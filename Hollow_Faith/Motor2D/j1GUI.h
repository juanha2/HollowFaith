#ifndef __j1GUI_H__
#define __j1GUI_H__


#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"

#include "j1GUIelement.h"


class j1Entity;

class j1GUI : public j1Module
{

public:

	j1GUI();
	~j1GUI();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);


	j1GUIelement* AddGUIelement(GUItype type, j1GUIelement* parent, iPoint globalPosition, iPoint localPosition, bool interactable, bool enabled,  SDL_Rect section, char* text=nullptr);
	SDL_Texture* j1GUI::GetAtlasTexture() const;

	p2List<j1GUIelement*>	GUIelementList;
private:

	SDL_Texture* atlasTexture;
	p2SString atlasFile;
};



#endif //  __j1GUI_H__
