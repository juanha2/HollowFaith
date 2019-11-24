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
#include "j1Objects.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Pathfinding.h"
#include "j1Entity.h"
#include "j1Enemy.h"


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
	
	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file);

	App->objects->DeleteEntities();
	App->objects->AddEntity(j1Entity::entityType::PLAYER, { 0,0 });
	App->objects->player->Awake(config.child(App->objects->name.GetString()));
	App->objects->player->Start();

	App->objects->AddEntity(j1Entity::entityType::ENEMY, { 0,0 });
	App->objects->enemy->Awake(config.child(App->objects->name.GetString()));
	App->objects->enemy->Start();
	
	App->objects->AddEntity(j1Entity::entityType::STONE, { 0,0 });
	App->objects->particle->Awake(config.child(App->objects->name.GetString()));
	App->objects->particle->Start();

	//Load first level at start
	if (first) 
	{
		p2List_item<Levels*>* levelData = App->map->data.levels.start;
        App->map->Load(levelData->data->name.GetString());
        currentmap = 1;
        first = false;		
	}

    App->audio->PlayMusic(App->map->data.properties.start->data->value.GetString(), 1.0f);    //Plays current map music
    graphics = App->tex->Load("Assets/Sprites/halo.png");
	debug_tex = App->tex->Load("Assets/Sprites/path2.png");

	ready_to_load = false;
	sound_repeat = false; 
    App->render->camera = App->render->camera_init; //Sets camera on inicial position.
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);


	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) 
	{ // Start at the level 1 begin
		currentmap = 1;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap-1]->name.GetString(), 1.0f);
	}


	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) 
	{ // Start at the level 2 begin
		
		currentmap = 2;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap-1]->name.GetString(), 1.0f);
	}
	

	//if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) 
	//{ // Start at the current level begin
	//	App->player->ignoreColl = true;		
	//	App->player->playerPosition = App->player->startPosLevel1; 
	//	App->render->camera.x = 0;
	//}


	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) 
	{ // Save State
		App->SaveGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) 
	{ // Load State				
		App->LoadGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && App->objects->player->godMode == false)
	{ // Turn On God mode
		App->objects->player->godMode = true;
		App->objects->player->ignoreColl = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && App->objects->player->godMode == true)
	{ // Turn Off God mode
		App->objects->player->godMode = false;
	}	

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
	SDL_Rect rect = { 0,0, App->win->screen_surface->w / App->win->scale, App->win->screen_surface->h / App->win->scale };
	App->render->Blit(graphics, -App->render->camera.x, -App->render->camera.y, &rect, App->win->GetScale(), App->win->GetScale()); // Printing player texture

	static char title[256];
	sprintf_s(title, 256, "%s || Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu ",
		App->GetTitle(), App->avg_fps, App->last_frame_ms, App->frames_on_last_update, App->seconds_since_startup, App->frame_count);
	App->win->SetTitle(title);

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");		
	
	App->objects->DeleteEntities();
	App->tex->UnLoad(graphics);
	App->tex->UnLoad(debug_tex);
	App->tex->CleanUp();
	App->coll->CleanUp();

	App->objects->player = nullptr;
	App->objects->enemy = nullptr;
	App->objects->particle = nullptr;

	return true;
}

bool j1Scene::Save(pugi::xml_node& save) const
{
	/*App->player->savedPosition.x = App->player->playerPosition.x;
	App->player->savedPosition.y = App->player->playerPosition.y;*/

	pugi::xml_node current_map = save.append_child("currentmap");

	current_map.append_attribute("value").set_value(currentmap);

	return true;
}

bool j1Scene::Load(pugi::xml_node& save)
{
	savedcurrentmap = save.child("currentmap").attribute("value").as_int();
	LOG("%i", savedcurrentmap);

	if (savedcurrentmap != currentmap) {
		currentmap = savedcurrentmap;
		different_map = true;
		App->fade_to_black->FadeToBlack(App->map->data.levels[savedcurrentmap - 1]->name.GetString(), 1.0f);
	}
	

	return true;
}
