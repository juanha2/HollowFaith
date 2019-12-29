#ifndef __J1GUICONSOLE__
#define __J1GUICONSOLE__

#include "j1Module.h"
#include"j1GUIelement.h"

class j1GUIconsole : public j1GUIelement
{
public:

	j1GUIconsole();
	~j1GUIconsole();

	bool Start();
	bool PreUpdate();
	bool CleanUp();

	void PrintText(const char* txt);

public:

	p2SString		consoleString;
	j1GUIelement* inputbox = nullptr;
	j1GUIelement* label = nullptr;
	j1GUIelement* image = nullptr;
};






#endif // !__J1GUIBUTTON__
