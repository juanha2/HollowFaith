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
	App->objects->DeleteEntities();
	App->objects->AddEntity(j1Entity::entityType::PLAYER, { 0,0 });	
	App->objects->AddEntity(j1Entity::entityType::STONE, { 0,0 });

	//Load first level at start
	if (first) 
	{
		p2List_item<Levels*>* levelData = App->map->data.levels.start;
        App->map->Load(levelData->data->name.GetString());
        currentmap = 1;
        first = false;				

		int w, h;
		uchar* data = nullptr;
		if (App->map->CreateWalkabilityMap(w, h, &data))
		{
			App->pathfinding->SetMap(w, h, data);
			RELEASE_ARRAY(data);
		}		
	}

	SpawnEnemies();
	
    App->audio->PlayMusic(App->map->data.music.GetString(), 1.0f);    //Plays current map music
    graphics = App->tex->Load("Assets/Sprites/halo.png");
	debug_tex = App->tex->Load("Assets/Sprites/path2.png");

	ready_to_load = false;
	sound_repeat = false; 
    App->render->camera = App->render->camera_init; //Sets camera on inicial position.
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
		checkpoint = false;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap-1]->name.GetString(), 1.0f);
	}


	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) 
	{ // Start at the level 2 begin
		checkpoint = false;
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
	App->render->Blit(graphics, -App->render->camera.x, -App->render->camera.y, &rect, App->win->GetScale(), App->win->GetScale()); // Printing player texture

	static char title[256];
	sprintf_s(title, 256, "%s || Actual FPS: %i | Av.FPS: %.2f Last Frame Ms: %02u Time since startup: %.3f Frame Count: %lu ",
		App->GetTitle(), App->frames_on_last_update , App->avg_fps, App->last_frame_ms, App->seconds_since_startup, App->frame_count);

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

	if (savedcurrentmap != currentmap) 
	{
		currentmap = savedcurrentmap;
		different_map = true;

		App->fade_to_black->FadeToBlack(App->map->data.levels[savedcurrentmap - 1]->name.GetString(), 1.0f);
	
	}	

	return true;
}

void j1Scene::SpawnEnemies() {

	BROFILER_CATEGORY("SpawningEnemies", Profiler::Color::PapayaWhip);

	for (p2List_item<ObjectsGroup*>* object = App->map->data.objectgroups.start; object; object = object->next)
	{
		if (object->data->name == ("Enemies"))
		{
			for (p2List_item<ObjectsData*>* object_data = object->data->objects.start; object_data; object_data = object_data->next)
			{
				if (object_data->data->name == 6)
				{
					App->objects->AddEntity(j1Entity::entityType::ENEMY_FLY, { object_data->data->x,object_data->data->y });
				}
				if (object_data->data->name == 7)
				{
					App->objects->AddEntity(j1Entity::entityType::ENEMY_LAND, { object_data->data->x,object_data->data->y });
				}
				if (object_data->data->name == 8)
				{
					App->objects->AddEntity(j1Entity::entityType::BONFIRE, { object_data->data->x,object_data->data->y });
				}

			}
		}
	}

}
