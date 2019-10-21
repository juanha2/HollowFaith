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
#include "j1Player.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"

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
	
	//Load first level at start
	if (first) {
		
		p2List_item<Levels*>* levelData = App->map->data.levels.start;
		App->map->Load(levelData->data->name.GetString());		
		//App->audio->PlayMusic(App->map->data.properties.start->data->value.GetString(), 1.0f);
		first = false;
	}
	if (currentmap == 1) {
		//App->audio->PlayMusic(App->map->data.properties.start->data->value.GetString(), 1.0f);
	}
	if (currentmap == 2) {
	//	App->audio->PlayMusic(App->map->data.properties.start->next->data->value.GetString(), 1.0f);
	}
	App->render->camera = App->render->camera_init; //Sets camera on inicial position.
	graphics = App->tex->Load("Assets/Sprites/halo.png");
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();		

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {	
		if (App->render->camera.y>-App->map->data.height*App->map->data.tile_height)
			App->render->camera.y -= 1;	
	}		

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		if (App->render->camera.y < 0)
			App->render->camera.y += 1;
	}		

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {			
		if(App->render->camera.x>-App->map->data.width*App->map->data.tile_width)
			App->render->camera.x -= 1;
	}		

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		if (App->render->camera.x < 0)
			App->render->camera.x += 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		currentmap = 2;
		App->fade_to_black->FadeToBlack("level02.tmx", 1.0f);
	}
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		currentmap = 1;
		App->fade_to_black->FadeToBlack("level01.tmx", 1.0f);

	}
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		App->player->savedPosition.x = App->player->playerPosition.x;
		App->player->savedPosition.y = App->player->playerPosition.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		App->player->ignoreColl = true;
		App->player->playerPosition.y = App->player->savedPosition.y;
		App->player->playerPosition.x = App->player->savedPosition.x;	
	}
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && App->player->godMode == false) {
		App->player->godMode = true;
		App->player->ignoreColl = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && App->player->godMode == true) {
		App->player->godMode = false;

	}	

	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	iPoint mouseCoord = App->map->WorldToMap((App->input->mouse_x - App->render->camera.x), (App->input->mouse_y - App->render->camera.y));

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Mouse Position X:%d Y:%d Mouse Tilset:%d,%d Camera Position:%d,%d Player Position:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(), App->input->mouse_x - App->render->camera.x,
		App->input->mouse_y - App->render->camera.y,
		mouseCoord.x, mouseCoord.y,
		App->render->camera.x, App->render->camera.y,
		App->player->playerPosition.x, App->player->playerPosition.y);

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
	SDL_Rect rect = { 0,0,1024 / 2,384 };
	App->render->Blit(graphics, -App->render->camera.x, -App->render->camera.y, &rect, App->win->GetScale(), App->win->GetScale()); // Printing player texture

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
		
	
	App->tex->UnLoad(graphics);	
	App->tex->CleanUp();		

	return true;
}
