#ifndef __j1Console_H__
#define __j1Console_H__

#include "j1Module.h"

class j1GUIelement;


class j1Command {

public:
	enum commandType
	{
		NO_TYPE,
		LIST,
		GOD_MODE,
		LOAD_MAP,
		FPS_CAP,
		QUIT,		
	};

public:
	j1Command();
	~j1Command();
	bool CleanUp();

public:

	commandType		type;
	const char* name = nullptr;
	int			max_arguments;
	int			min_arguments;
	j1Module* listener = nullptr;
	
};


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
	void ReceiveText(const char* txt);
	bool ConsoleLogic();

	void CreateCommand(j1Command::commandType type, const char* command, j1Module* callback, uint min_arg, uint max_args);

public:
	p2SString		consoleString;
	
	j1GUIelement* console = nullptr;
	bool		want_exit = false;

	p2List<j1Command*>	commands;
};

#endif // __j1Console_H__