#ifndef __J1GUIINPUTBOX__
#define __J1GUIINPUTBOX__

#include "j1Module.h"
#include"j1GUIelement.h"
#include "j1GUI.h"

struct SDL_Texture;

class j1GUIinputBox : public j1GUIelement
{
public:


	j1GUIinputBox(char* text);
	~j1GUIinputBox();

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();	
	void OnClick();

private:

	j1GUIelement* string = nullptr;
};






#endif // !__J1GUIINPUTBOX__

