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
#include <stdio.h>//for the sprintf_s function
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
	coin_Fx = fxIterator.child("coinFx").attribute("path").as_string();
	life_Fx = fxIterator.child("lifeUpFx").attribute("path").as_string();
	score_Fx = fxIterator.child("scoreUp").attribute("path").as_string();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//Setting initial values
	timer = 0;
	lifes = 3;
	score = 0;
	num_coins = 0;
	App->win->scale = 2;
	currentmap = 1;
	ready_to_load = false;
	sound_repeat = false;

	if (App->intro->want_continue)
		App->LoadGame();
	else
		LoadMap(currentmap);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate() {

	//At reaching 10 coins, we add a life to Player

	if (num_coins >= 10) {
		App->audio->PlayFx(13, 0, 128);
		lifes++;
		//Add +1 particle 
		App->objects->particle->AddParticle(App->objects->particle->lifeUp, App->objects->player->position.x,
			App->objects->player->position.y + App->objects->player->entity_collider.h, SDL_FLIP_NONE, COLLIDER_NONE, 2);
		num_coins = 0;
	}

	Mix_VolumeMusic(App->audio->MUSICvolume);
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	bool ret = true;
	BROFILER_CATEGORY("Scene_Update", Profiler::Color::Olive);

	if (want_exit)
		ret = false;

	//Updating all UI Texts
	timer += dt;
	sprintf_s(timerText, "%.2f", timer);
	App->tex->UnLoad(timer_label->texture);
	timer_label->text = timerText;

	sprintf_s(lifesText, "%d", lifes);
	App->tex->UnLoad(lifes_label->texture);
	lifes_label->text = lifesText;

	sprintf_s(coinsText, "%d", num_coins);
	App->tex->UnLoad(coins_label->texture);
	coins_label->text = coinsText;

	sprintf_s(scoreText, "%0.5d", score);
	App->tex->UnLoad(score_label->texture);
	score_label->text = scoreText;

	//------------------------------------

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{ // Start at the level 1 begin
		currentmap = 1;
		App->checkpoint->checkpoint = false;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap - 1]->name.GetString(), 1.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{ // Start at the level 2 begin
		App->checkpoint->checkpoint = false;
		currentmap = 2;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap - 1]->name.GetString(), 1.0f);
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
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && !console.image->enabled)
			EnableDisableMenu();

	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN && !menu.image->enabled)
		EnableDisableConsole();	

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		EnableDisableVictoryMenu();

	if (console.input_box->focus)
		ret = ConsoleLogic();


	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);

	App->map->Draw();

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
		App->GetTitle(), App->frames_on_last_update, App->avg_fps, App->last_frame_ms, App->render->vsync, capped);

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
	App->audio->UnLoad();

	console.input_box = nullptr;
	console.image = nullptr;
	menu.menu_button = nullptr;
	menu.resume_button = nullptr;
	menu.return_button = nullptr;
	menu.music_scroll = nullptr;
	menu.image = nullptr;
	menu.title = nullptr;
	menu.exit_button = nullptr;
	menu.volume_scroll = nullptr;
	menu.save = nullptr;
	menu.load = nullptr;
	menu.label1 = nullptr;
	menu.label2 = nullptr;
	lifes_icon = nullptr;
	coins_icon = nullptr;
	coins_label = nullptr;
	lifes_label = nullptr;
	timer_label = nullptr;
	timer_icon = nullptr;
	score_label = nullptr;
	score_icon = nullptr;
	complete.image = nullptr;
	complete.exit_button = nullptr;
	complete.title = nullptr;
	complete.timer = nullptr;
	complete.score = nullptr;
	return true;
}

// ----------------------------------------------------------------------------------
// Save Game State
// ----------------------------------------------------------------------------------
bool j1Scene::Save(pugi::xml_node& save) const
{
	pugi::xml_node current_map = save.append_child("currentmap");
	pugi::xml_node lifes_node = save.append_child("lifes");
	pugi::xml_node coins_node = save.append_child("coins");
	pugi::xml_node time_node = save.append_child("time");
	current_map.append_attribute("value").set_value(currentmap);
	lifes_node.append_attribute("value").set_value(lifes);
	time_node.append_attribute("value").set_value(timer);

	if (!App->checkpoint->save_checkpoints)
		coins_node.append_attribute("value").set_value(num_coins);

	else
		coins_node.append_attribute("value").set_value(App->checkpoint->checkpointcoins);

	return true;
}


// ----------------------------------------------------------------------------------
// Load Game State
// ----------------------------------------------------------------------------------
bool j1Scene::Load(pugi::xml_node& save)
{
	savedcurrentmap = save.child("currentmap").attribute("value").as_int();
	lifes = save.child("lifes").attribute("value").as_int();
	timer = save.child("time").attribute("value").as_float();
	num_coins = save.child("coins").attribute("value").as_int();

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
			App->checkpoint->checkpoint = false;

			if (currentmap == 1)
			{
				if (!sound_repeat) {
					App->audio->PlayFx(3, 0, App->audio->FXvolume);
					sound_repeat = true;
				}
				currentmap = 2;
				for (int i = 1; i <= App->map->data.numLevels; i++) {
					if (currentmap == i)
						App->fade_to_black->FadeToBlack(App->map->data.levels[i - 1]->name.GetString(), 2.0f);
				}
			}
			else if (currentmap == 2)
			{
				if (!sound_repeat) {
					App->audio->PlayFx(4, 0, App->audio->FXvolume);
					sound_repeat = true;
					EnableDisableVictoryMenu();
				}
				//currentmap = 1;
			}				

		}
		// Losing -------------------------

		if (App->objects->player->dead) {

			if (!sound_repeat) {
				App->audio->PlayFx(2, 0, App->audio->FXvolume);
				sound_repeat = true;
				lifes--;
			}

			num_coins = 0;
			score = 0;

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
	App->render->camera = App->render->camera_init;

	//Adding every UI element
	AddUIElements();

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
	App->audio->LoadFx(coin_Fx.GetString());		//12
	App->audio->LoadFx(life_Fx.GetString());		//13
	App->audio->LoadFx(score_Fx.GetString());		//14

	App->audio->LoadFx("audio/fx/button_click.wav");
	debug_tex = App->tex->Load("Assets/Sprites/path2.png");
}


void j1Scene::GuiObserver(GUI_Event type, j1GUIelement* element)
{
	switch (type)
	{

	case GUI_Event::EVENT_ONCLICK:
	{
		App->audio->PlayFx(15, 0, App->audio->FXvolume);

		if (element == menu.return_button) {
			App->pause = !App->pause;
			App->fade_to_black->FadeToBlack(App->intro, this);
		}

		if (element == menu.exit_button) {
			App->pause = !App->pause;
			want_exit = true;
		}

		if (element == menu.save)
			App->SaveGame();

		if (element == menu.load) {
			EnableDisableMenu();
			App->LoadGame();
		}

		if (element == menu.resume_button) 
			EnableDisableMenu();
		
		if (element == menu.menu_button && !console.image->enabled) 
			EnableDisableMenu();
		
		if (element == complete.exit_button) {
			EnableDisableVictoryMenu();
			App->fade_to_black->FadeToBlack(App->intro, this);
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
	if (App->input->GetText() == "FPS")
	{
			
	}
	
	if (App->input->final_text == "map1") {
		currentmap = 1;
		App->checkpoint->checkpoint = false;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap - 1]->name.GetString(), 1.0f);
	}

	if (App->input->final_text == "map2") {
		App->checkpoint->checkpoint = false;
		currentmap = 2;
		App->fade_to_black->FadeToBlack(App->map->data.levels[currentmap - 1]->name.GetString(), 1.0f);
	}
	if (App->input->final_text == "quit") {
		return false;
	}

	App->input->final_text.Clear();

	return ret;
}

void j1Scene::AddUIElements()
{
	timer_label = App->gui->AddGUIelement(GUItype::GUI_LABEL, nullptr, { 220,22 }, { 0,0 }, false, true, { 0,0,0,0 }, timerText, this, false, false);
	timer_icon = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 180, 13 }, { 0,0 }, false, true, { 460, 111, 27, 31 }, nullptr, this);
	lifes_label = App->gui->AddGUIelement(GUItype::GUI_LABEL, nullptr, { 146,22 }, { 0,0 }, false, true, { 0,0,0,0 }, "3");
	lifes_icon = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 100, 13 }, { 0,0 }, false, true, { 458, 78, 41, 31 }, nullptr, this);
	coins_label = App->gui->AddGUIelement(GUItype::GUI_LABEL, nullptr, { 55,22 }, { 0,0 }, false, true, { 0,0,0,0 }, "0");
	coins_icon = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 10, 15 }, { 0,0 }, false, true, { 458, 43, 40, 27 }, nullptr, this);
	score_label = App->gui->AddGUIelement(GUItype::GUI_LABEL, nullptr, { 325,22 }, { 0,0 }, true, true, { 0,0,0,0 }, nullptr, this, false, false);
	score_icon = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 273, 11 }, { 0,0 }, false, true, { 169, 162, 49, 34 }, nullptr, this);

	menu.image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 160, 60 }, { 0,0 }, false, false, { 288, 144, 198, 282 }, nullptr, this);
	menu.menu_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 450,13 }, { 0,0 }, true, true, { 84,164,37,31 }, nullptr, this);
	menu.return_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 210,130 }, { -3,-5 }, true, false, { 283,109,100,22 }, "MAIN MENU", this);
	menu.title = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 195,50 }, { 28,0 }, false, false, { 533,78,129,32 }, "MENU", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	menu.resume_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 210,100 }, { 10,-5 }, true, false, { 283,109,100,22 }, "RESUME", this);
	menu.exit_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 210,220 }, { 20,-5 }, true, false, { 283,109,100,22 }, "EXIT", this);
	menu.save = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 210,160 }, { 20,-5 }, true, false, { 283,109,100,22 }, "SAVE", this);
	menu.load = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 210,190 }, { 20,-5 }, true, false, { 283,109,100,22 }, "LOAD", this);
	menu.volume_scroll = App->gui->AddGUIelement(GUItype::GUI_SCROLLBAR, nullptr, { 220, 270 }, { 0,0 }, false, false, { 284, 62, 120, 4 }, nullptr, this, true, false, SCROLL_TYPE::SCROLL_MUSIC);
	menu.music_scroll = App->gui->AddGUIelement(GUItype::GUI_SCROLLBAR, nullptr, { 220, 310 }, { 0,0 }, false, false, { 284, 62, 120, 4 }, nullptr, this, true, false, SCROLL_TYPE::SCROLL_FX);
	console.image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 160, 60 }, { 0,0 }, false, false, { 288, 144, 198, 200 }, nullptr, this);
	menu.label1 = App->gui->AddGUIelement(GUItype::GUI_LABEL, nullptr, { 170, 270 }, { 0,-3 }, false, false, { 166,167,109,27 }, "MUSIC", this);
	menu.label2 = App->gui->AddGUIelement(GUItype::GUI_LABEL, nullptr, { 170, 310 }, { 0,-3 }, false, false, { 166,167,109,27 }, "FX'S", this);
	console.input_box = App->gui->AddGUIelement(GUItype::GUI_INPUTBOX, nullptr, { 168,220 }, { 0,0 }, true, false, { 11,359,182,26 }, nullptr, this);
	
	complete.image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, nullptr, { 145, 60 }, { 0,0 }, false, false, { 492, 140, 240, 288 }, nullptr, this);
	complete.exit_button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 205,290 }, { 23,0 }, true, false, { 281,6,127,36 }, "CLOSE", this);
	complete.title = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 200,75 }, { 12,0 }, false, false, { 533,78,129,32 }, "VICTORY!!", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	complete.timer = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 205,120 }, { 40,18 }, false, false, { 507,5,104,68 }, timerText, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, { 20,252,13 });
	complete.score = App->gui->AddGUIelement(GUItype::GUI_BUTTON, nullptr, { 205,200 }, { 30,18 }, false, false, { 614,3,102,69 }, scoreText, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, { 20,252,13 });
}

void j1Scene::EnableDisableMenu() {

	menu.image->enabled = !menu.image->enabled;
	menu.resume_button->enabled = !menu.resume_button->enabled;
	menu.return_button->enabled = !menu.return_button->enabled;
	menu.title->enabled = !menu.title->enabled;
	menu.volume_scroll->enabled = !menu.volume_scroll->enabled;
	menu.music_scroll->enabled = !menu.music_scroll->enabled;
	menu.exit_button->enabled = !menu.exit_button->enabled;
	menu.load->enabled = !menu.load->enabled;
	menu.save->enabled = !menu.save->enabled;
	menu.label1->enabled = !menu.label1->enabled;
	menu.label2->enabled = !menu.label2->enabled;
	App->pause = !App->pause;
}

void j1Scene::EnableDisableConsole()
{
	App->input->text.Clear();
	console.input_box->focus = !console.input_box->focus;
	console.input_box->enabled = !console.input_box->enabled;
	console.image->enabled = !console.image->enabled;
	App->pause = !App->pause;
}

void j1Scene::EnableDisableVictoryMenu() {

	App->pause = !App->pause;
	complete.image->enabled = !complete.image->enabled;
	complete.exit_button->enabled = !complete.exit_button->enabled;
	complete.title->enabled = !complete.title->enabled;
	complete.timer->enabled = !complete.timer->enabled;
	complete.score->enabled = !complete.score->enabled;	
	
	//BEST SCORES AND TIME LOGIC--------------------------------
	if(score > App->intro->final_score)
		App->intro->final_score = score;
	if(App->intro->final_time==0 || timer < App->intro->final_time)
		App->intro->final_time = timer;	
}