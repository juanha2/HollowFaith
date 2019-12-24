#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1IntroScene.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1GUI.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Map.h"

j1IntroScene::j1IntroScene() : j1Module()
{
	name.create("intro");
}

// Destructor
j1IntroScene::~j1IntroScene()
{}

// Called before render is available
bool j1IntroScene::Awake()
{
	LOG("Loading Intro Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1IntroScene::Start()
{
	App->win->scale = 1;
	App->audio->PlayMusic("audio/music/intro.ogg", 1.0f);
	texture = App->tex->Load("Assets/Sprites/background.png");
	App->render->camera = App->render->camera_init;

	play_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 50,50 }, { 0,0 }, true, true, { 4,69,130,37 }, "PLAY", this);
	continue_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 50,100 }, { 0,0 }, true, true, { 4,69,130,37 }, "CONTINUE", this);
	exit_button = App->gui-> AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 50,150 }, { 0,0 }, true, true, { 4,69,130,37 }, "EXIT", this);
	   
	// Settings menu UI elements
	settings_menu.image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 200, 50 }, { 0,0 }, true, false, { 20, 324, 251, 270 }, nullptr, this);
	settings_menu.button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 50,200 }, { 0,0 }, true, true, { 4,69,130,37 }, "SETTINGS", this);
	settings_menu.exit = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 410,60 }, { 0,0 }, true, false, { 513,53,14,14 }, nullptr, this);
	settings_menu.scroll = App->gui->AddGUIelement(GUItype::GUI_SCROLLBAR, nullptr, { 210, 80 }, { 0,0 }, false, false, { 0, 6, 183, 7 }, nullptr,this);

	// Credits menu UI elements
	credits_menu.image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 50, 300 }, { 0,0 }, true, false, { 20, 324, 251, 270 }, nullptr, this);
	credits_menu.button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 50,250 }, { 0,0 }, true, true, { 4,69,130,37 }, "CREDITS", this);
	credits_menu.exit = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 260,310 }, { 0,0 }, true, false, { 513,53,14,14 }, nullptr, this);	

	// Continue Button Logic (if there is not a save_game file, it wont be enable)
	pugi::xml_document data;
	pugi::xml_parse_result result = data.load_file(App->GetLoadString().GetString());

	if (result == NULL)
		continue_button->interactable = false;
	else
		continue_button->interactable = true;
	//-------------------------------------------------------

	return true;
}

// Called each loop iteration
bool j1IntroScene::PreUpdate() {

	return true;
}

// Called each loop iteration
bool j1IntroScene::Update(float dt)
{
	bool ret = true;

	if (want_exit)
		ret = false;

	SDL_Rect rect = { 0,0, App->win->screen_surface->w, App->win->screen_surface->h };
	App->render->Blit(texture, 0, 10, &rect);

	return ret;
}

// Called each loop iteration
bool j1IntroScene::PostUpdate()
{
	bool ret = true;	

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1IntroScene::CleanUp()
{
	LOG("Freeing intro scene");
	play_button = nullptr;
	continue_button = nullptr;
	exit_button = nullptr;

	//Cleaning Setting menu UI
	settings_menu.image = nullptr;
	settings_menu.exit = nullptr;
	settings_menu.button = nullptr;
	settings_menu.scroll = nullptr;

	//Cleaning Credits menu UI
	credits_menu.image = nullptr;
	credits_menu.exit = nullptr;
	credits_menu.button = nullptr;

	App->gui->CleanUp();
	App->tex->UnLoad(texture);
	App->audio->UnLoad();
	return true;
}

void j1IntroScene::GuiObserver(GUI_Event type, j1GUIelement* element) 
{

	switch (type) 
	{

		case GUI_Event::EVENT_ONCLICK: 
		{

			if (element == play_button) 
				App->fade_to_black->FadeToBlack(App->scene, this);
			

			if (element == continue_button) {
				want_continue = true;
				App->fade_to_black->FadeToBlack(App->scene, this);
			}

			if (element == settings_menu.button) {
				settings_menu.image->enabled = true;
				settings_menu.exit->enabled = true;	
				settings_menu.scroll->enabled = true;
			}

			if (element == settings_menu.exit) {
				settings_menu.image->enabled = false;
				settings_menu.exit->enabled = false;
				settings_menu.scroll->enabled = false;
			}

			if (element == credits_menu.button) {
				credits_menu.image->enabled = true;
				credits_menu.exit->enabled = true;
			}

			if (element == credits_menu.exit) {
				credits_menu.image->enabled = false;
				credits_menu.exit->enabled = false;
			}

			if (element == exit_button)
				want_exit = true;

		}
	}

}
