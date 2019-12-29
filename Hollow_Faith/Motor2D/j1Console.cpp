#include "j1App.h"
#include "j1Console.h"
#include "j1GUI.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1EntityManager.h"

j1Console::j1Console() {

	name.create("console");
}

j1Console::~j1Console() {}

bool j1Console::Awake(pugi::xml_node&) {

	
	return true;
}

bool j1Console::Start()
{
	//We add our GUI element Console + create command
	CreateCommand(j1Command::commandType::LIST, "list", this, 1, 1);
	console = App->gui->AddGUIelement(GUItype::GUI_CONSOLE, nullptr, { 273, 11 }, { 0,0 }, false, false, { 169, 162, 49, 34 }, nullptr, this);
	return true;
}

bool j1Console::PreUpdate() {

	if (App->scene->active) {
		if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN)
			EnableDisableConsole();
	}

	return true;
}

bool j1Console::CleanUp() {
	
	App->gui->DeleteConsole();
	console = nullptr;
	return true;
}

void j1Console::EnableDisableConsole() 
{	
	console->enabled = !console->enabled;
	console->focus = !console->focus;	
}

void j1Console::ReceiveText(const char* txt)
{
	if (console->enabled)
		console->PrintText(txt);
}

bool j1Console::ExecuteCommand(const char* txt) {

	bool ret = true;

	not_found = 0;
	p2SString consoleString = txt;
	
	//If the console string has 2 words, we get the first one
	if(consoleString.FindFirst(" "))
		consoleString.Cut(consoleString.FindFirst(" "));	

	p2List_item<j1Command*>* command = commands.start;

	while (command != nullptr)
	{		
		//if the new text equals the command text, we execute our commands
		if (consoleString == command->data->first_name)
		{
			consoleString = txt;

			switch (command->data->type) {

			case j1Command::commandType::LOAD_MAP:
				consoleString.Cut(0, consoleString.FindFirst(" ") + 1);
				command->data->listener->GuiObserver(GUI_Event::EVENT_CONSOLE, nullptr, consoleString, command->data->first_name);
				break;
			case j1Command::commandType::FPS_CAP:
				consoleString.Cut(0, consoleString.FindFirst(" ") + 1);
				command->data->listener->GuiObserver(GUI_Event::EVENT_CONSOLE, nullptr, consoleString, command->data->first_name);
				break;
			case j1Command::commandType::LIST:
				command->data->listener->GuiObserver(GUI_Event::EVENT_CONSOLE, nullptr, consoleString, command->data->first_name);
				break;
			case j1Command::commandType::QUIT:
				ret = command->data->listener->GuiObserver(GUI_Event::EVENT_CONSOLE, nullptr, consoleString, command->data->first_name);
				break;
			case j1Command::commandType::GOD_MODE:
				command->data->listener->GuiObserver(GUI_Event::EVENT_CONSOLE, nullptr, consoleString, command->data->first_name);
				break;
			}
			break;
		}		
		else
			not_found++;

		command = command->next;
	}
	
	if (not_found == commands.count()) {
		LOG("WRONG COMMAND! TYPE 'list' TO SEE ALL COMMANDS");
	}

	consoleString.Clear();

	return ret;
}

void j1Console::CreateCommand(j1Command::commandType typ, const char* command, j1Module* callback, uint min_arg, uint max_args) {

	j1Command* tmp = nullptr;
	tmp = new j1Command();

	if (tmp) {
		tmp->type = typ;
		tmp->name = command;		
		tmp->listener = callback;
		tmp->Start();
	}	
	
	commands.add(tmp);
}

bool j1Console::GuiObserver(GUI_Event type, j1GUIelement* element, p2SString txt, p2SString name)
{
	switch (type)
	{

	case GUI_Event::EVENT_CONSOLE:
	{
		if (name == "list")
		{
			LOG("1. god_mode");
			LOG("2. map <path>");
			LOG("3. FPS <number>");
			LOG("4. quit");
		}
	}
	break;
	}
	return true;
}

void j1Console::DeleteCommands() {

	p2List_item<j1Command*>* tmp = commands.end;

	while (tmp != nullptr)
	{
		p2List_item<j1Command*>* tmp2 = tmp;

		tmp->data->CleanUp();
		RELEASE(tmp->data);
		commands.del(tmp2);

		tmp = tmp->prev;
	}
}

j1Command::j1Command() {}

j1Command::~j1Command() {}

bool j1Command::Start() {

	//We get the first word to compare later
	first_name = this->name;

	if (first_name.FindFirst(" "))
		first_name.Cut(first_name.FindFirst(" "));
	
	return true;
}

bool j1Command::CleanUp() 
{ 
	name = nullptr;
	listener = nullptr;
	return true; 
}
