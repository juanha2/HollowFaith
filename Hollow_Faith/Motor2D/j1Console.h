#ifndef __j1Console_H__
#define __j1Console_H__

#include "j1Module.h"

class j1GUIelement;

class j1Console : public j1Module
{

public:

	j1Console();
	~j1Console();

	bool Start();
	bool Awake(pugi::xml_node&);
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void EnableDisableConsole();

	void PrintText(const char* txt);
	void LogText(const char* txt);

public:
	p2SString		consoleString;

	j1GUIelement* console = nullptr;
};

#endif // __j1Console_H__