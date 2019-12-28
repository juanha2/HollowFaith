#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "j1Entity.h"


class j1Console : public j1Module
{

public:

	j1Console();
	~j1Console();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void EnableDisableConsole();

	void ReceiveText(const char* text);
	void PrintText(char* text);
	bool ConsoleLogic();

public:

	p2SString		consoleString;
	p2SString		command;
	j1GUIelement* input_box = nullptr;
	j1GUIelement* image = nullptr;
	j1GUIelement* text = nullptr;
};



#endif // !

