#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Pathfinding.h"
#include "j1Entity.h"
#include "j1Enemy.h"
#include "j1Checkpoint.h"
#include "j1GUI.h"
#include "j1IntroScene.h"

#include "j1Fonts.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	pugi::xml_node fxIterator = config.child("fx");
	jump_fx = fxIterator.child("jumpFx").attribute("path").as_string();
	death_fx = fxIterator.child("deathFx").attribute("path").as_string();
	win1_Fx = fxIterator.child("win1Fx").attribute("path").as_string();
	win2_Fx = fxIterator.child("win2Fx").attribute("path").as_string();
	landing_Fx = fxIterator.child("landingFx").attribute("path").as_string();
	hover_Fx = fxIterator.child("hoverFx").attribute("path").as_string();
	stone_Fx = fxIterator.child("stoneFx").attribute("path").as_string();
	fire_Fx = fxIterator.child("bonfireFx").attribute("path").as_string();
	fuse_Fx = fxIterator.child("fuseFx").attribute("path").as_string();
	death = fxIterator.child("death_enemyFx").attribute("path").as_string();
	hurt_Fx = fxIterator.child("hurtFx").attribute("path").as_string();
	   

	return ret; 
}

// Called before the first frame
bool j1Scene::Start()
{
	//Setting initial values
	lifes = 3;
	App->win->scale = 2;
	debug_tex = App->tex->Load("Assets/Sprites/path2.png");	
	currentmap = 1;	
	ready_to_load = false;
	sound_repeat = false;	
	
	//We load every Fx here so each entity doesn't have to repeat loading the same one		

	App->audio->LoadFx(jump_fx.GetString());		//1
	App->audio->LoadFx(death_fx.GetString());		//2
	App->audio->LoadFx(win1_Fx.GetString());		//3
	App->audio->LoadFx(win2_Fx.GetString());		//4
	App->audio->LoadFx(landing_Fx.GetString());		//5
	App->audio->LoadFx(hover_Fx.GetString());		//6	
	App->audio->LoadFx(stone_Fx.GetString());		//7
	App->audio->LoadFx(fire_Fx.GetString());		//8
	App->audio->LoadFx(fuse_Fx.GetString());		//9
	App->audio->LoadFx(death.GetString());			//10
	App->audio->LoadFx(hurt_Fx.GetString());		//11

	if (App->intro->want_continue)		
		App->LoadGame();			
	else 
		LoadMap(currentmap);	
	
	
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate() {

	if (lifes == 3)
		lifes_label->text = "3";
	if (lifes == 2)
		lifes_label->text = "2";
	if (lifes == 1)
		lifes_label->text = "1";

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	bool ret = true;
	BROFILER_CATEGORY("Scene_Update", Profiler::Color::Olive);
	
	if(App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN )
		App->pause = !App->pause;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) 
	{ // Start at the level 1 begin
		currentmap = 1;
		App->checkpoint->checkpoint = false;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap-1]->name.GetString(), 1.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) 
	{ // Start at the level 2 begin
		App->checkpoint->checkpoint = false;
		currentmap = 2;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap-1]->name.GetString(), 1.0f);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) 
	{ // Start at the current level begin
		App->checkpoint->checkpoint = false;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap - 1]->name.GetString(), 1.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) 
	{ // Save State
		App->SaveGame();		
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) 
	{ // Load State				
		App->LoadGame();		
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && App->objects->player->godMode == false)
	{ // Turn On God mode
		App->objects->player->godMode = true;
		App->objects->player->ignoreColl = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && App->objects->player->godMode == true)
	{ // Turn Off God mode
		App->objects->player->godMode = false;
	}	

	//Changing frameratecap
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) 
	{
		if (App->frameratecap == App->desiredFrameratecap)
			App->frameratecap = 1000;
		else
			App->frameratecap = App->desiredFrameratecap;
	}		
	
	//Opening in-game menu
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) 
	{
		menu.image->enabled = !menu.image->enabled;		
		menu.resume_button->enabled = !menu.resume_button->enabled;
		menu.return_button->enabled = !menu.return_button->enabled;
		menu.volume_scroll->enabled = !menu.volume_scroll->enabled;
	}

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) 
	{	
		console_on = !console_on;
		console.image->enabled = !console.image->enabled;
		console.input_box->enabled = !console.input_box->enabled;
		console.input_box->focus = true;
	}		
	
	if (console_on)
		ret= ConsoleLogic();
	
	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);

	return ret;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene_PostUpdate", Profiler::Color::DarkOliveGreen);

	bool ret = true;
	SDL_Rect rect = { 0,0, App->win->screen_surface->w / App->win->scale, App->win->screen_surface->h / App->win->scale };

	bool capped;
	
	if (App->frameratecap == App->desiredFrameratecap)
		capped = true;
	else
		capped = false;
	

	static char title[256];
	sprintf_s(title, 256, "%s || Actual FPS: %i | Av.FPS: %.2f | Last Frame MS: %02u | VSYNC: %d | Frames Cap: %d",
		App->GetTitle(), App->frames_on_last_update , App->avg_fps, App->last_frame_ms, App->render->vsync, capped);

	App->win->SetTitle(title);



	sceneswitch();

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");	

	lifes_label = nullptr;
	App->map->CleanUp();	
	App->pathfinding->CleanUp();
	App->objects->CleanUp();
	App->coll->CleanUp();
	App->tex->UnLoad(debug_tex);
	App->gui->CleanUp();

	console.input_box = nullptr;
	console.image = nullptr;
	menu.resume_button = nullptr;
	menu.return_button = nullptr;
	menu.image = nullptr;
	menu.exit_button = nullptr;
	menu.volume_scroll = nullptr;

	return true;
}


// ----------------------------------------------------------------------------------
// Save Game State
// ----------------------------------------------------------------------------------
bool j1Scene::Save(pugi::xml_node& save) const
{
	pugi::xml_node current_map = save.append_child("currentmap");
	pugi::xml_node lifes_node = save.append_child("lifes");
	current_map.append_attribute("value").set_value(currentmap);
	lifes_node.append_attribute("value").set_value(lifes);

	return true;
}


// ----------------------------------------------------------------------------------
// Load Game State
// ----------------------------------------------------------------------------------
bool j1Scene::Load(pugi::xml_node& save)
{
	savedcurrentmap = save.child("currentmap").attribute("value").as_int();	
	lifes= save.child("lifes").attribute("value").as_int();

	if (savedcurrentmap == currentmap && App->intro->want_continue)		
		LoadMap(currentmap);

	// If we want to load from a different map, we change scene.
	else if (savedcurrentmap != currentmap) 
	{
		currentmap = savedcurrentmap;
		different_map = true;
		
		if (!App->intro->want_continue) {
			App->objects->player->ignoreColl = true;
			App->objects->player->speed = { 0,0 };
			App->objects->player->gravityForce = 0.0f;
			App->fade_to_black->FadeToBlack(App->map->data.levels[savedcurrentmap - 1]->name.GetString(), 1.0f);
		}			
		else 
			LoadMap(currentmap);			
	}		
	
	return true;
}


void j1Scene::sceneswitch()
{
	if (App->objects->player != nullptr) {

		// Winning -------------------------
		if (App->objects->player->win) {

			App->objects->player->ignoreColl = true;

			if (currentmap == 1)
			{
				if (!sound_repeat) {
					App->audio->PlayFx(3, 0, App->audio->FXvolume);
					sound_repeat = true;
				}
				currentmap = 2;
			}
			else if (currentmap == 2)
			{
				if (!sound_repeat) {
					App->audio->PlayFx(4, 0, App->audio->FXvolume);
					sound_repeat = true;
				}
				currentmap = 1;
			}

			App->checkpoint->checkpoint = false;

			for (int i = 1; i <= App->map->data.numLevels; i++) {
				if (currentmap == i)
					App->fade_to_black->FadeToBlack(App->map->data.levels[i - 1]->name.GetString(), 2.0f);
			}

		}
		// Losing -------------------------

		if (App->objects->player->dead) {

			if (!sound_repeat) {
				App->audio->PlayFx(2, 0, App->audio->FXvolume);
				sound_repeat = true;
				lifes--;
			}


			if (currentmap == 1)
				currentmap = 1;
			else if (currentmap == 2)
				currentmap = 2;

			if (lifes != 0) {
				for (int i = 1; i <= App->map->data.numLevels; i++) {
					if (App->scene->currentmap == i)
						App->fade_to_black->FadeToBlack(App->map->data.levels[i - 1]->name.GetString(), 2.0f);
				}
			}
			else {
				App->fade_to_black->FadeToBlack(App->intro, this);
			}

		}
	}	

}

void j1Scene::LoadMap(int num_map) {

	CleanUp();
	App->map->Load(App->map->data.levels[num_map - 1]->name.GetString());
	App->scene->ready_to_load = false;
	App->scene->sound_repeat = false;
	lifes_label = App->gui->AddGUIelement(GUItype::GUI_LABEL, nullptr, { 50,50 }, { 0,0 }, true, true, { 0,0,0,0 }, "3");
	App->render->camera = App->render->camera_init;

	//Create Walkability Map
	int w, h;
	uchar* data = nullptr;
	if (App->map->CreateWalkabilityMap(w, h, &data))
	{
		App->pathfinding->SetMap(w, h, data);
		RELEASE_ARRAY(data);
	}

	//Plays current map music
	App->audio->PlayMusic(App->map->data.music.GetString(), 1.0f);

	App->intro->want_continue = false;	

	menu.image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 201, 50 }, { 0,0 }, true, false, { 577, 220, 142, 283 }, nullptr, this);
	menu.return_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 220,150 }, { 0,0 }, true, false, { 733,224,106,30 }, "MAIN MENU", this);
	menu.return_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 220,200 }, { 0,0 }, true, false, { 733,224,106,30 }, "MAIN MENU", this);
	menu.resume_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 220,150 }, { 0,0 }, true, false, { 733,224,106,30 }, "RESUME", this);
	menu.volume_scroll = App->gui->AddGUIelement(GUItype::GUI_SCROLLBAR, nullptr, { 205, 250 }, { 0,0 }, true, false, { 858, 245, 133, 4 }, nullptr, this);
	console.image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 125, 50 }, { 0,0 }, true, false, { 20, 324, 251, 270 }, nullptr, this);
	console.input_box = App->gui->AddGUIelement(GUItype::GUI_INPUTBOX, nullptr, { 150,100 }, { 0,0 }, true, false, { 295,343,199,30 }, "Commands", this);
}


void j1Scene::GuiObserver(GUI_Event type, j1GUIelement* element)
{
	switch (type)
	{

	case GUI_Event::EVENT_ONCLICK:
	{

		if (element == menu.return_button)
			App->fade_to_black->FadeToBlack(App->intro, this);

		if (element == menu.resume_button) {
			menu.image->enabled = !menu.image->enabled;		
			menu.resume_button->enabled = !menu.resume_button->enabled;
			menu.return_button->enabled = !menu.return_button->enabled;
			menu.volume_scroll->enabled = !menu.volume_scroll->enabled;
		}
		if (element == menu.volume_scroll) {
			
		}
	}	
	}
}

bool j1Scene::ConsoleLogic() 
{
	bool ret = true;	

	if (App->input->final_text == "list") {
		LOG("HOLA");
	}
	if (App->input->final_text == "god_mode") {
		console.input_box->focus = false;
		App->objects->player->godMode = !App->objects->player->godMode;
	}
	if (App->input->final_text == "quit") {
		return false;
	}

	App->input->final_text.Clear();

	return ret;
}