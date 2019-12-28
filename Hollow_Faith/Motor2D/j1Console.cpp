#include "j1App.h"
#include "j1Console.h"
#include "j1GUI.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Textures.h"

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

	/*if (console->enabled) {
		();
	}*/

	return true;
}

bool j1Console::PostUpdate() {

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

void j1Console::PrintText(const char* txt) {

	if(console->enabled)
		console->PrintText(txt);	
}

void j1Console::LogText(const char* txt) {

	LOG(txt);
}