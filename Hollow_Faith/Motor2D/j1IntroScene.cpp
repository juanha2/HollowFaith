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
bool j1IntroScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Intro Scene");
	bool ret = true;	
	pugi::xml_node fxIterator = config.child("fx");
	click_Fx1 = fxIterator.child("clickFx1").attribute("path").as_string();
	click_Fx2 = fxIterator.child("clickFx2").attribute("path").as_string();

	pugi::xml_node animIterator = config.child("animation");
	fire.load_animation(animIterator, "fire");
	return ret;
}

// Called before the first frame
bool j1IntroScene::Start()
{
	App->win->scale = 2;
	App->audio->PlayMusic("audio/music/intro.ogg", 1.0f);
		
	App->audio->LoadFx(click_Fx1.GetString());
	App->audio->LoadFx(click_Fx2.GetString());

	texture = App->tex->Load("Assets/Sprites/background.png");
	title_texture= App->tex->Load("Assets/Sprites/title.png");
	fire_texture = App->tex->Load("Assets/Sprites/fire.png");
	App->render->camera = App->render->camera_init;	

	AddUIElements();

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
	
	SDL_Rect rect1 = {0,0, 800,200 };
	SDL_Rect rect2 = { 0,0,67,63 };
	App->render->Blit(texture, 0, 0);
	App->render->Blit(title_texture, 40, 40);

	App->render->Blit(fire_texture, 350, 250, &fire.GetCurrentFrame(dt));

	return ret;
}

// Called each loop iteration
bool j1IntroScene::PostUpdate()
{
	bool ret = true;	

	static char title[256];
	sprintf_s(title, 256, "%s || MAIN MENU", App->GetTitle());

	App->win->SetTitle(title);

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
	settings_menu.title = nullptr;
	settings_menu.label1 = nullptr;
	settings_menu.label2 = nullptr;

	//Cleaning Credits menu UI
	credits_menu.image = nullptr;
	credits_menu.exit = nullptr;
	credits_menu.button = nullptr;
	credits_menu.title = nullptr;
	credits_menu.link = nullptr;
	credits_menu.credits = nullptr;

	App->gui->CleanUp();
	App->tex->UnLoad(texture);
	App->tex->UnLoad(title_texture);
	App->tex->UnLoad(fire_texture);
	App->audio->UnLoad();
	return true;
}

void j1IntroScene::GuiObserver(GUI_Event type, j1GUIelement* element) 
{

	switch (type) 
	{
		case GUI_Event::EVENT_ONCLICK: 
		{		

			if (element == play_button) {
				App->audio->PlayFx(1, 0, 128);
				App->fade_to_black->FadeToBlack(App->scene, this);
			}				

			if (element == continue_button) {
				want_continue = true;
				App->audio->PlayFx(1, 0, 128);
				App->fade_to_black->FadeToBlack(App->scene, this);
			}

			if (element == settings_menu.button) {
				App->audio->PlayFx(2, 0, 128);
				settings_menu.image->enabled = !settings_menu.image->enabled;
				settings_menu.exit->enabled = !settings_menu.exit->enabled;
				settings_menu.scroll1->enabled = !settings_menu.scroll1->enabled;
				settings_menu.scroll2->enabled = !settings_menu.scroll2->enabled;
				settings_menu.title->enabled = !settings_menu.title->enabled;
				settings_menu.label1->enabled = !settings_menu.label1->enabled;
				settings_menu.label2->enabled = !settings_menu.label2->enabled;
				credits_menu.image->enabled = false;
				credits_menu.exit->enabled = false;
				credits_menu.title->enabled = false;
				credits_menu.link->enabled = false;
				credits_menu.credits->enabled = false;
				
			}
			
			if (element == settings_menu.exit) {
				App->audio->PlayFx(2, 0, 128);
				settings_menu.image->enabled = false;
				settings_menu.exit->enabled = false;
				settings_menu.scroll1->enabled = false;
				settings_menu.scroll2->enabled = false;
				settings_menu.title->enabled = false;
				settings_menu.label1->enabled = false;
				settings_menu.label2->enabled = false;
			}

			if (element == credits_menu.button) {
				App->audio->PlayFx(2, 0, 128);
				credits_menu.image->enabled = !credits_menu.image->enabled;
				credits_menu.exit->enabled = !credits_menu.exit->enabled;
				credits_menu.title->enabled = !credits_menu.title->enabled;
				credits_menu.link->enabled = !credits_menu.link->enabled;
				credits_menu.credits->enabled = !credits_menu.credits->enabled;
				settings_menu.image->enabled = false;
				settings_menu.exit->enabled = false;
				settings_menu.scroll1->enabled = false;
				settings_menu.scroll2->enabled = false;
				settings_menu.title->enabled = false;
				settings_menu.label1->enabled = false;
				settings_menu.label2->enabled = false;			
			}

			if (element == credits_menu.exit) {
				App->audio->PlayFx(2, 0, 128);
				credits_menu.image->enabled = false;
				credits_menu.exit->enabled = false;
				credits_menu.title->enabled = false;
				credits_menu.link->enabled = false;
				credits_menu.credits->enabled = false;
			}

			if (element == credits_menu.link) {
				App->audio->PlayFx(2, 0, 128);
				ShellExecuteA(NULL, "open", "https://github.com/juanha2/HollowFaith", NULL, NULL, SW_SHOWNORMAL);
			}				

			if (element == exit_button)
				want_exit = true;

		}

		case GUI_Event::EVENT_HOVER:
		{
								
		}		
	}

}

void j1IntroScene::AddUIElements() 
{
	int x_pos = App->win->screen_surface->w / 8 - 70;

	// Main menu UI elements
	play_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { x_pos,180 }, { 0,0 }, true, true, { 281,6,127,36 }, "PLAY", this);
	continue_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { x_pos,240 }, { 0,0 }, true, true, { 281,6,127,36 }, "CONTINUE", this);
	exit_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { x_pos,300 }, { 0,0 }, true, true, { 281,6,127,36 }, "EXIT", this);

	// Settings menu UI elements
	settings_menu.image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 210, 180 }, { 0,0 }, true, false, { 4,3, 271, 156 }, nullptr, this);
	settings_menu.button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 400,20 }, { 0,0 }, true, true, { 84,164,37,31 }, nullptr, this);
	settings_menu.exit = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 440,190 }, { 0,0 }, true, false, { 459,16,26,26 }, nullptr, this);
	settings_menu.scroll1 = App->gui->AddGUIelement(GUItype::GUI_SCROLLBAR, nullptr, { 300, 250 }, { 0,0 }, false, false, { 284, 62, 167, 4 }, nullptr, this);
	settings_menu.scroll2 = App->gui->AddGUIelement(GUItype::GUI_SCROLLBAR, nullptr, { 300, 300 }, { 0,0 }, false, false, { 284, 62, 167, 4 }, nullptr, this);
	settings_menu.title = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 250,170 }, { 0,-3 }, false, false, { 166,167,109,27 }, "SETTINGS", this);
	settings_menu.label1 = App->gui->AddGUIelement(GUItype::GUI_LABEL, nullptr, { 230, 250 }, { 0,-3 }, false, false, { 166,167,109,27 }, "MUSIC", this);
	settings_menu.label2 = App->gui->AddGUIelement(GUItype::GUI_LABEL, nullptr, { 230, 300 }, { 0,-3 }, false, false, { 166,167,109,27 }, "FX'S", this);

	// Credits menu UI elements
	credits_menu.image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 210, 180 }, { 0,0 }, true, false, { 4,3, 271, 156 }, nullptr, this);
	credits_menu.button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 450,20 }, { 0,0 }, true, true, { 124,165,37,31 }, nullptr, this);
	credits_menu.exit = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 440,190 }, { 0,0 }, true, false, { 459,16,26,26 }, nullptr, this);
	credits_menu.title = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 250,170 }, { 0,-3 }, false, false, { 166,167,109,27 }, "CREDITS", this);
	credits_menu.link = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 325, 120 }, { -5,0 }, true, false, { 283, 75, 159, 31 }, "LINK TO PROJECT!", this);
	credits_menu.credits = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 215, 200 }, { 0,0 }, true, false, { 9,215, 257, 130 }, nullptr, this);
}
