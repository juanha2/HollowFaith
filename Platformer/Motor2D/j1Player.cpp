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
#include "j1Window.h"

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

	//		- - - - - - CAMERA FOLLOW - - - - - - 	

	if (playerPosition.x > cameraFollowingPoint) // Starts to follow the player faster in X position.
	{

		if(playerPosition.x)
			cameraSpeed.x -= playerSpeed.x;
	}
	else 
		cameraSpeed.x -= playerSpeed.x / 40;	
		
	
	//		- - - - - - PLAYER INPUTS - - - - - - 

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		playerSpeed.x += movementForce.x;
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		playerSpeed.x -= movementForce.x;
	else 
	{
		braking(); // Smoothy braking when player stops running
		cameraBraking(); // Smoothy camera braking when player is not running
	}
	

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && current_state != ST_AT_AIR) // Jumping
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
	PlayerPositionUpdate(frameToSecondValue);



	//Update position related to real time and player position.
	cameraSpeedLimitChecker();
	CameraPositionUpdate(frameToSecondValue);


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

		playerSpeed.y += gravityForce; // While it's in the air we apply gravity to get down the player
	
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

void j1Player::PlayerPositionUpdate(float dt)
{


	// X AXIS POS
	playerPosition.x = playerPosition.x + playerSpeed.x * dt;

	// Y AXIS POS
	playerPosition.y = playerPosition.y + playerSpeed.y * dt;
	playerSpeed.y = playerSpeed.y + playerAcceleration * dt;
	
}

void j1Player::CameraPositionUpdate(float dt) {

	// X AXIS POS

	if (playerPosition.x > 1024 / 4 - playerTexture.w)
		App->render->camera.x = (-playerPosition.x+ 1024 / 4 - playerTexture.w )*2;
	//if(playerPosition.y< 640-768/2 + playerTexture.h)
		//App->render->camera.y = (-playerPosition.y +150 + 768 / 4 - playerTexture.h) * 2;


	//App->render->camera.x = App->render->camera.x + cameraSpeed.x * dt;

	//Y AXIS POS
	App->render->camera.y = App->render->camera.y + cameraSpeed.y * dt;
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

void j1Player::cameraSpeedLimitChecker() {

	//POSITIVE

	if (cameraSpeed.x > cameraSpeedLimit.x)
		cameraSpeed.x = cameraSpeedLimit.x;

	if (cameraSpeed.y > cameraSpeedLimit.y)
	{
		cameraSpeed.y = cameraSpeedLimit.y;
	}


	//NEGATIVE

	if (cameraSpeed.x < -cameraSpeedLimit.x)
		cameraSpeed.x = -cameraSpeedLimit.x;

	if (cameraSpeed.y < -cameraSpeedLimit.y)
	{
		cameraSpeed.y = -cameraSpeedLimit.y;
	}

}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	int detectCollDir[DIR_MAX];
	detectCollDir[DIR_LEFT] = (c2->rect.x + c2->rect.w) - playerPosition.x;
	detectCollDir[DIR_RIGHT] = (playerPosition.x + playerTexture.w) - c2->rect.x;
	detectCollDir[DIR_UP] = (c2->rect.y + c2->rect.h) - playerPosition.y;
	detectCollDir[DIR_DOWN] = (playerPosition.y + playerTexture.h) - c2->rect.y;

	bool collDir[DIR_MAX];
	collDir[DIR_RIGHT] = !(detectCollDir[DIR_RIGHT] > 0 && playerSpeed.x < 0);
	collDir[DIR_LEFT] = !(detectCollDir[DIR_RIGHT] > 0 && playerSpeed.x > 0);
	collDir[DIR_UP] = !(detectCollDir[DIR_UP] > 0 && playerSpeed.y < 0);
	collDir[DIR_DOWN] = !(detectCollDir[DIR_DOWN] > 0 && playerSpeed.y > 0);

	for (int i = 0; i < MAXNUMOFCOLLIDERS; i++)
	{
		bool alredycollided = false;

		// - - - - - - - COLLISIONS LOGIC - - - - - - - 

		int dirCheck = -1;

		for (int i = 0; i < DIR_MAX; ++i) 
		{
			if (collDir[i] && dirCheck == -1) 
			{
				dirCheck = i;
			}
			else if (detectCollDir[i] < detectCollDir[dirCheck]) 
			{
				dirCheck = i;
			}			
		}
		

		// - - - - - - - CHECK COLLISIONS - - - - - - - 

		if ((c2->type == COLLIDER_FLOOR))
		{

			switch (dirCheck) {

			case DIR_UP:
				playerPosition.y = c2->rect.y + c2->rect.h + 1;
				playerSpeed.y = 0;
				break;

			case DIR_DOWN:
				playerPosition.y = c2->rect.y - playerTexture.h;
				playerSpeed.y = 0;
				playerAcceleration = 0;
				checkingFall = false;
				inputs.add(IN_JUMP_FINISH);
				break;

			case DIR_LEFT:
				playerPosition.x = c2->rect.x + c2->rect.w;
				playerSpeed.x = 0;
				break;

			case DIR_RIGHT:
				playerPosition.x = c2->rect.x - playerTexture.w;
				playerSpeed.x = 0;
				break;
			case -1:
				break;
			}


			
		}

	}

}


void j1Player::cameraBraking()
{
	if (cameraSpeed.x > 0)
		cameraSpeed.x /= slowlingValue; // Smoothy braking when player stops running (We need to improve it)
	else
		cameraSpeed.x /= -slowlingValue;
}

void j1Player::braking()
{
	if(playerSpeed.x > 0)
		playerSpeed.x /= slowlingValue; // Smoothy braking when player stops running (We need to improve it)
	else
		playerSpeed.x /= -slowlingValue;
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

			case IN_FALLING: state = ST_AT_AIR;
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