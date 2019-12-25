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
#include "p2Animation.h"

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
	title_texture= App->tex->Load("Assets/Sprites/title.png");
	rain_texture = App->tex->Load("Assets/Sprites/rain.png");
	App->render->camera = App->render->camera_init;

	int x_pos = App->win->screen_surface->w / 5 -80;

	play_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { x_pos,325 }, { 0,0 }, true, true, { 558,22,174,49 }, "PLAY", this);
	continue_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { x_pos,400 }, { 0,0 }, true, true, { 558,22,174,49 }, "CONTINUE", this);
	exit_button = App->gui-> AddGUIelement(GUItype::GUI_BUTTON, nullptr, { x_pos,475 }, { 0,0 }, true, true, { 558,22,174,49 }, "EXIT", this);
	   
	// Settings menu UI elements
	settings_menu.image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 375, 325 }, { 0,0 }, true, false, { 8, 8, 542, 349 }, nullptr, this);
	settings_menu.button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { x_pos,550 }, { 0,0 }, true, true, { 558,22,174,49 }, "SETTINGS", this);
	settings_menu.exit = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 825,350 }, { 0,0 }, true, false, { 920,31,51,51 }, nullptr, this);
	settings_menu.scroll1 = App->gui->AddGUIelement(GUItype::GUI_SCROLLBAR, nullptr, { 525, 475 }, { 0,0 }, false, false, {568, 124, 333, 8 }, nullptr,this);
	settings_menu.scroll2 = App->gui->AddGUIelement(GUItype::GUI_SCROLLBAR, nullptr, { 525, 575 }, { 0,0 }, false, false, { 568, 124, 333, 8 }, nullptr, this);

	// Credits menu UI elements
	credits_menu.image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 375, 325 }, { 0,0 }, true, false, { 8, 8, 542, 349 }, nullptr, this);
	credits_menu.button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { x_pos,625 }, { 0,0 }, true, true, { 558,22,174,49 }, "CREDITS", this);
	credits_menu.exit = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 825,350 }, { 0,0 }, true, false, { 920,31,51,51 }, nullptr, this);

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
	SDL_Rect rect1 = {0,0, 800,200 };
	App->render->Blit(texture, 0, 10, &rect);
	App->render->Blit(title_texture, 100, 70);

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
	settings_menu.scroll1 = nullptr;
	settings_menu.scroll2 = nullptr;

	//Cleaning Credits menu UI
	credits_menu.image = nullptr;
	credits_menu.exit = nullptr;
	credits_menu.button = nullptr;

	App->gui->CleanUp();
	App->tex->UnLoad(texture);
	App->tex->UnLoad(title_texture);
	App->tex->UnLoad(rain_texture);
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
				settings_menu.scroll1->enabled = true;
				settings_menu.scroll2->enabled = true;
			}
			
			if (element == settings_menu.exit) {
				settings_menu.image->enabled = false;
				settings_menu.exit->enabled = false;
				settings_menu.scroll1->enabled = false;
				settings_menu.scroll2->enabled = false;
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
