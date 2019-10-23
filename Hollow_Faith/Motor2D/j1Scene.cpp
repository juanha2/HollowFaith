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
        currentmap = 1;
        first = false;
    }

    App->audio->PlayMusic(App->map->data.properties.start->data->value.GetString(), 1.0f);    //Plays current map music
    graphics = App->tex->Load("Assets/Sprites/halo.png");


	sound_repeat = false; 
    App->render->camera = App->render->camera_init; //Sets camera on inicial position.
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
	Win_Lose_Condition();	

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
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		currentmap = 1;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap-1]->name.GetString(), 1.0f);
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		currentmap = 2;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap-1]->name.GetString(), 1.0f);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {		
		App->player->ignoreColl = true;		
		App->player->playerPosition = App->player->startPosLevel1; 
		App->render->camera.x = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		App->SaveGame();
	}
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		App->LoadGame();
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

bool j1Scene::Save(pugi::xml_node& save) const
{
	App->player->savedPosition.x = App->player->playerPosition.x;
	App->player->savedPosition.y = App->player->playerPosition.y;

	pugi::xml_node current_map = save.append_child("currentmap");

	current_map.append_attribute("value").set_value(currentmap);

	return true;
}

bool j1Scene::Load(pugi::xml_node& save)
{
	savedcurrentmap = save.child("currentmap").attribute("value").as_int();

	if (savedcurrentmap != currentmap) {
		currentmap = savedcurrentmap;
		different_map = true;
		App->fade_to_black->FadeToBlack(App->map->data.levels[savedcurrentmap - 1]->name.GetString(), 1.0f);
	}



	return true;
}

void j1Scene::Win_Lose_Condition() {

	// Winning at map 1--------------------------
	if (App->player->win && currentmap == 1) {

		if (!sound_repeat && currentmap == 1) {
			App->audio->PlayFx(3, 0, App->audio->FXvolume);
			sound_repeat = true;
		}

		currentmap = 2;

		for (int i = 1; i <= App->map->data.numLevels; i++) {
			if (currentmap == i)
				App->fade_to_black->FadeToBlack(App->map->data.levels[i - 1]->name.GetString(), 2.0f);
		}
	}

	// Winning at map 2--------------------------
	else if (App->player->win && currentmap == 2) {

		if (!sound_repeat && currentmap == 2) {
			App->audio->PlayFx(4, 0, App->audio->FXvolume);
			sound_repeat = true;
		}

		currentmap = 1;

		for (int i = 1; i <= App->map->data.numLevels; i++) {
			if (currentmap == i)
				App->fade_to_black->FadeToBlack(App->map->data.levels[i - 1]->name.GetString(), 3.0f);
		}
	}

	// Dying -----------------------------------------
	if (App->player->dead) {

		if (!sound_repeat) {
			App->audio->PlayFx(2, 0, App->audio->FXvolume);
			sound_repeat = true;
		}

		for (int i = 1; i <= App->map->data.numLevels; i++) {
			if (App->scene->currentmap == i)
				App->fade_to_black->FadeToBlack(App->map->data.levels[i - 1]->name.GetString(), 2.0f);
		}
	}
}