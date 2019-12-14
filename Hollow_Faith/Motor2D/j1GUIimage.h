#ifndef __J1GUIIMAGE__
#define __J1GUIIMAGE__

#include "j1Module.h"
#include"j1GUIelement.h"
#include "j1GUI.h"

struct SDL_Texture;

class j1GUIimage : public j1GUIelement
{
public:

	j1GUIimage();
	~j1GUIimage();

	bool Awake(pugi::xml_node&);

	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };

};


#endif // !__J1GUIIMAGE__

