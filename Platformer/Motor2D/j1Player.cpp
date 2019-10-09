#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Map.h"


j1Player::j1Player() : j1Module()
{
	
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(graphics);

	return true;
}


// Called before the first frame
bool j1Player::Start()
{

	graphics = App->tex->Load("Assets/Sprites/playerChar.png");

	// Initial Pos (CREAR VARIABLES EN XML PARA POSICIONES)
	position.x = 30;
	position.y = -40 - App->render->camera.y;

	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	// MEJORAREMOS ESTO EN UNA MÁQUINA DE ESTADOS

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		position.x++;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		position.x--;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		position.y--;

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		position.y++;

	return true;
}

// Called each loop iteration
bool j1Player::Update()
{


	
	return true;
}


bool j1Player::PostUpdate()
{


	SDL_Rect r = { 0, 0, 50, 57 };
	App->render->Blit(graphics, position.x, position.y, &r, 1.0);

	return true;
}