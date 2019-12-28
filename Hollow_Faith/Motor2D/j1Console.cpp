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

bool j1Console::Update(float dt) {

	bool ret = true;
	if (console->enabled)
		ret = ConsoleLogic();		

	return ret;
}

bool j1Console::PostUpdate() {	
		
	return true;
}

bool j1Console::CleanUp() {
	
	p2List_item<j1Command*>* tmp = commands.end;

	while (tmp != nullptr)
	{
		p2List_item<j1Command*>* tmp2 = tmp;
	
		tmp->data->CleanUp();
		RELEASE(tmp->data);
		commands.del(tmp2);		

		tmp = tmp->prev;
	}

	App->gui->DeleteConsole();
	console = nullptr;
	return true;
}

void j1Console::EnableDisableConsole() 
{	
	console->enabled = !console->enabled;
	console->focus = !console->focus;
	
}

void j1Console::PrintText(const char* txt) {

	if(console->enabled)
		console->PrintText(txt);	

}

void j1Console::ReceiveText(const char* txt) {

	//LOG(txt);
	consoleString = txt;
}

bool j1Console::ConsoleLogic() {

	if (consoleString == "list") {
		
		CreateCommand(j1Command::commandType::LIST,"list", this, 1, 1);
	}
	
	else if (consoleString == "god_mode") {
		EnableDisableConsole();
		App->objects->player->godMode = !App->objects->player->godMode;
		App->objects->player->ignoreColl = !App->objects->player->ignoreColl;
	}
	else if (consoleString == "quit") {
		EnableDisableConsole();
		return false;
	}
	else if(consoleString.Length()>0){
		LOG("UNKNOWN COMMAND. TYPE 'LIST' TO READ ALL COMMANDS");
	}

	consoleString.Clear();

	return true;
}

void j1Console::CreateCommand(j1Command::commandType type, const char* command, j1Module* callback, uint min_arg, uint max_args) {

	j1Command* tmp = nullptr;

	switch (type)
	{
	case j1Command::commandType::LIST:
		LOG("1. god_mode");
		LOG("2. FPS<num>");
		LOG("3. map path");
		LOG("4. quit");
		break;

	case j1Command::commandType::GOD_MODE:

		tmp = new j1Command();
		break;

	case j1Command::commandType::LOAD_MAP:

		tmp = new j1Command();
		break;

	case j1Command::commandType::FPS_CAP:
		tmp = new j1Command();
		break;

	case j1Command::commandType::QUIT:
		tmp = new j1Command();
		break;

	}
	if (tmp) {
		tmp->name = command;
		tmp->min_arguments = min_arg;
		tmp->max_arguments = max_args;
		tmp->listener = callback;
	}	
	
	commands.add(tmp);
}

j1Command::j1Command() {

}

j1Command::~j1Command() {}

bool j1Command::CleanUp() 
{ 
	name = nullptr;
	listener = nullptr;
	return true; 
}