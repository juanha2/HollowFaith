#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Map.h"


j1Player::j1Player() : j1Module()
{

}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(graphics);

	for (int i = 0; i < MAXNUMOFCOLLIDERS; i++)//deletes all the hitboxes at the start of the frame
	{
		if (colisionadores[i] != nullptr) {
			colisionadores[i]->to_delete = true;
			colisionadores[i] = nullptr;
		}
	}

	return true;
}


// Called before the first frame
bool j1Player::Start()
{

	graphics = App->tex->Load("Assets/Sprites/playerChar.png");



	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{

	//Deletes all the hitboxes at the start of the frame
	for (int i = 0; i < MAXNUMOFCOLLIDERS; i++)
	{
		if (colisionadores[i] != nullptr) {
			colisionadores[i]->to_delete = true;
			colisionadores[i] = nullptr;
		}
	}

	//Get time from frames and it's corrected
	previousTime = frameToSecondValue;
	frameToSecondValue = App->DeltaTime();
	if (frameToSecondValue > 0.16)
		frameToSecondValue = 0.16f;


	//		- - - - - - PLAYER INPUTS - - - - - - 

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		playerSpeed.x += movementForce.x;
		braking(); // Smoothy braking when player stops running
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{

		playerSpeed.x -= movementForce.x;
		braking(); // Smoothy braking when player stops running

	}
	else
		braking(); // Smoothy braking when player stops running
	

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && current_state != ST_AT_AIR) // Jumping
	{
		playerSpeed.y = movementForce.y; 
		inputs.add(IN_JUMPING);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		playerAcceleration += -movementForce.y; // Get down while you're in the air faster

	//		- - - - - - - - - - - - - - - - - - 

	
	//Refresh the player state
	player_states state = process_fsm(inputs);
	current_state = state;

	//Update position related to real time and puts speed limit.
	speedLimitChecker();
	PlayerPosUpdate(frameToSecondValue);



	//(Provisional floor)
	if (playerPosition.y > 640) {
		playerPosition.y = 640;
		inputs.add(IN_JUMP_FINISH);
	}


	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{

	switch (current_state)
	{
	case ST_AT_FLOOR:

		playerAcceleration = 0;
		playerSpeed.y = 0; // When is at the floor we don't apply any gravity force

		//current_animation = &idle; 
		break;


	case ST_AT_AIR:

		playerAcceleration += gravityForce; // While it's in the air we apply gravity to get down the player
	
		break;
	}

	for (int i = 0; i < MAXNUMOFCOLLIDERS; i++)
	{
		colisionadores[i] = App->coll->AddCollider({ (int)playerPosition.x, (int)playerPosition.y, playerTexture.w, playerTexture.h }, COLLIDER_PLAYER, 0, 0, 0, 0, this); // Creating player colliders
	}

	return true;
}


bool j1Player::PostUpdate()
{

	App->render->Blit(graphics, playerPosition.x, playerPosition.y, &playerTexture, 1.0, SDL_FLIP_HORIZONTAL); // Printing player texture

	return true;
}

void j1Player::PlayerPosUpdate(float dt)
{

	// X AXIS POS
	playerPosition.x = playerPosition.x + playerSpeed.x * dt;

	// Y AXIS POS
	playerPosition.y = playerPosition.y + playerSpeed.y * dt;
	playerSpeed.y = playerSpeed.y + playerAcceleration * dt;

}


void j1Player::speedLimitChecker()
{

	//POSITIVE

	if (playerSpeed.x > speedLimit.x)
		playerSpeed.x = speedLimit.x;

	if (playerSpeed.y > speedLimit.y)
	{
		playerSpeed.y = speedLimit.y;
		playerAcceleration = playerAcceleration;
	}


	//NEGATIVE

	if (playerSpeed.x < -speedLimit.x)
		playerSpeed.x = -speedLimit.x;

	if (playerSpeed.y < -speedLimit.y)
	{
		playerSpeed.y = -speedLimit.y;
		playerAcceleration = playerAcceleration;
	}

}

void j1Player::OnCollision(Collider* c1, Collider* c2) {


	bool alredycollided = false;
	for (int i = 0; i < MAXNUMOFCOLLIDERS; i++)
	{
		if ((c2->type == COLLIDER_FLOOR))
		{

			playerPosition.y = c2->rect.y - playerTexture.h;

		}
	}

}


void j1Player::braking()
{

	playerSpeed.x /= slowlingValue; // Smoothy braking when player stops running (We need to improve it)

}


player_states j1Player::process_fsm(p2List<player_inputs>& inputs)
{

	static player_states state = ST_AT_FLOOR;
	player_inputs  last_input;

	while (inputs.Pop(last_input))
	{
		switch (state)
		{
		case ST_AT_FLOOR:
		{
			switch (last_input)
			{
			case IN_JUMPING: state = ST_AT_AIR; 

				break;
			}
		}
		break;

		case ST_AT_AIR:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_AT_FLOOR; 

				break;

			}
		}
		break;
		}
	}
	return state;
}