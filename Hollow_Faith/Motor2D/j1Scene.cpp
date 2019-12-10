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


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	   
	return ret; 
}

// Called before the first frame
bool j1Scene::Start()
{
	debug_tex = App->tex->Load("Assets/Sprites/path2.png");

	//Load first level at start
	App->objects->CleanUp();
	p2List_item<Levels*>* levelData = App->map->data.levels.start;
	App->map->Load(levelData->data->name.GetString());
	currentmap = 1;		

	int w, h;
	uchar* data = nullptr;
	if (App->map->CreateWalkabilityMap(w, h, &data))
	{
		App->pathfinding->SetMap(w, h, data);
		RELEASE_ARRAY(data);
	}
	
	    //Plays current map music
	App->audio->PlayMusic(App->map->data.music.GetString(), 1.0f);
	// We set initial values
	ready_to_load = false;
	sound_repeat = false;	
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate() {

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	BROFILER_CATEGORY("Scene_Update", Profiler::Color::Olive);

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
		

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);

	return true;
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

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	sceneswitch();

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");	

	App->audio->UnLoad();
	App->pathfinding->CleanUp();
	App->objects->CleanUp();
	App->coll->CleanUp();
	App->tex->UnLoad(debug_tex);

	return true;
}


// ----------------------------------------------------------------------------------
// Save Game State
// ----------------------------------------------------------------------------------
bool j1Scene::Save(pugi::xml_node& save) const
{

	pugi::xml_node current_map = save.append_child("currentmap");
	current_map.append_attribute("value").set_value(currentmap);

	return true;
}


// ----------------------------------------------------------------------------------
// Load Game State
// ----------------------------------------------------------------------------------
bool j1Scene::Load(pugi::xml_node& save)
{
	savedcurrentmap = save.child("currentmap").attribute("value").as_int();	

	// If we want to load from a different map, we change scene.
	if (savedcurrentmap != currentmap) 
	{
		currentmap = savedcurrentmap;
		different_map = true;
		App->objects->player->ignoreColl = true;
		App->objects->player->speed = { 0,0 };
		App->objects->player->gravityForce = 0.0f;
		App->fade_to_black->FadeToBlack(App->map->data.levels[savedcurrentmap - 1]->name.GetString(), 1.0f);	
	}	

	return true;
}


void j1Scene::sceneswitch()
{
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
		}

		if (currentmap == 1)
			currentmap = 1;
		else if (currentmap == 2)
			currentmap = 2;

		for (int i = 1; i <= App->map->data.numLevels; i++) {
			if (App->scene->currentmap == i)
				App->fade_to_black->FadeToBlack(App->map->data.levels[i - 1]->name.GetString(), 2.0f);
		}
	}

}