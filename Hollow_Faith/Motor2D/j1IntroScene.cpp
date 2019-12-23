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
	
	texture = App->tex->Load("Assets/Sprites/background.png");
	play_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 50,50 }, { 0,0 }, true, true, { 4,69,130,37 }, "PLAY");
	continue_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 50,100 }, { 0,0 }, true, true, { 4,69,130,37 }, "CONTINUE");
	exit_button = App->gui-> AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 50,150 }, { 0,0 }, true, true, { 4,69,130,37 }, "EXIT");
	//settings_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 50,200 }, { 0,0 }, true, true, { 4,69,130,37 }, "SETTINGS");
	settings_ui=App->gui->AddGUIelement(GUItype::GUI_SETTINGS, nullptr, { 50,250 }, { 0,0 }, true, true, { 20,324,251,270 }, "SETTINGS");
	
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

	if (play_button->focus) {		
		App->fade_to_black->FadeToBlack(App->scene, this);
	}

	if (continue_button->focus) {
		want_continue = true;
		App->fade_to_black->FadeToBlack(App->scene, this);	
	}	

	//if (settings_button->focus) {

	//	settings_ui->focus = true;
	//}

	if (exit_button->focus) {
		ret = false;
	}

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
	settings_button = nullptr;
	App->gui->CleanUp();
	App->tex->UnLoad(texture);
	App->audio->UnLoad();
	return true;
}
