#ifndef __J1GUICONSOLE__
#define __J1GUICONSOLE__

#include "j1Module.h"
#include"j1GUIelement.h"

class j1GUIconsole : public j1GUIelement
{
public:

	j1GUIconsole();
	~j1GUIconsole();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };

	void PrintText(const char* txt);
public:

	p2SString		consoleString;
	j1GUIelement* inputbox = nullptr;
	j1GUIelement* label = nullptr;
	j1GUIelement* image = nullptr;

};






#endif // !__J1GUIBUTTON__
