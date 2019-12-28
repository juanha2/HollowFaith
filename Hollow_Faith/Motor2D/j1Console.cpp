#include "j1Console.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1GUIelement.h"
#include "j1Scene.h"
#include "j1GUI.h"
#include "j1Input.h"
#include "j1Collision.h"
#include "j1Fonts.h"
#include "j1EntityManager.h"
#include "j1FadeToBlack.h"


j1Console::j1Console() {
	name.create("console");
}

j1Console::~j1Console()
{
}

bool j1Console::Awake(pugi::xml_node& config) {

	return true;
}

bool j1Console::Start() {

	image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 160, 60 }, { 0,0 }, false, false, { 288, 144, 198, 200 }, nullptr, this);
	input_box = App->gui->AddGUIelement(GUItype::GUI_INPUTBOX, nullptr, { 168,220 }, { 0,0 }, true, false, { 11,359,182,26 }, nullptr, this);
	text = App->gui->AddGUIelement(GUItype::GUI_LABEL, nullptr, { 160, 60 }, { 3,3 }, false, false, { 166,167,109,27 }, "Commands:", this);

	return true;
}

bool j1Console::PreUpdate() {
	
	return true;
}

bool j1Console::Update(float dt) {

	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN && App->scene->active)
		EnableDisableConsole();	

	return true;
}

bool j1Console::PostUpdate() {

	bool ret = true;
	if (input_box->focus)
		ret = ConsoleLogic();

	return true;
}

bool j1Console::CleanUp()
{
	
	input_box = nullptr;
	text = nullptr;
	image = nullptr;
	
	return true;
}

void j1Console::EnableDisableConsole()
{	
	App->input->text.Clear();
	consoleString.Clear();
	App->tex->UnLoad(text->texture);
	App->pause = !App->pause;
	input_box->focus = !input_box->focus;
	input_box->enabled = !input_box->enabled;
	image->enabled = !image->enabled;
	text->enabled = !text->enabled;
	
}

void j1Console::ReceiveText(const char* text) {

	command = text;
	LOG(text);
}

void j1Console::PrintText(char* txt) {

	consoleString += "- ";
	consoleString += txt;
	consoleString += ";";
	consoleString += "\n";

	if (text != nullptr) {
		App->tex->UnLoad(text->texture);
		text->text = consoleString.GetString();
		App->fonts->CalcSize(consoleString.GetString(), text->rect.w, text->rect.h);

		if (text->rect.h > image->rect.h - 20)
		{
			consoleString.Cut(consoleString.FindFirst("-"), consoleString.FindFirst(";") + 2);
		}
	}
	
}


bool j1Console::ConsoleLogic()
{
	bool ret = true;

	if (command == "list") {
		
		LOG("1. god_mode");
		LOG("2. map<num>");
		LOG("3. FPS<num>");
		LOG("4. quit");
	}
	if (command == "god_mode") {
		input_box->focus = false;
		App->objects->player->godMode = !App->objects->player->godMode;
	}
	//if (App->input->GetText() == "FPS")
	//{
	//		
	//}
	
	if (command == "map1") {		
		App->pause = !App->pause;
		input_box->focus = false;
		input_box->enabled = false;
		image->enabled = false;
		text->enabled = false;
		App->scene->ChangeMap(1);
	}

	if (command == "map2") {
		App->pause = !App->pause;
		input_box->focus = false;
		input_box->enabled = false;
		image->enabled = false;
		text->enabled = false;

		App->scene->ChangeMap(2);
	}
	if (command == "quit") {
		return false;
	}

	command.Clear();

	return ret;
}