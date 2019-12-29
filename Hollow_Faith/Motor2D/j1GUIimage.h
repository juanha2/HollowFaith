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

	bool PostUpdate();
	bool CleanUp();
};


#endif // !__J1GUIIMAGE__

