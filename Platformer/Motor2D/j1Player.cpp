#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Particles.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1FadeToBlack.h"



j1Player::j1Player() : j1Module()
{
	name.create("player");
}

j1Player::~j1Player() {};

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");
	bool ret = true;	
	

	pugi::xml_node animIterator = config.child("animations").child("animation");

	idle.load_animation(animIterator, "idle");
	walk.load_animation(animIterator, "walk");
	jump.load_animation(animIterator, "jump");
	death.load_animation(animIterator, "death");
	climb.load_animation(animIterator, "climb");


	jump_fx = config.child("jumpFx").attribute("path").as_string();
	death_fx = config.child("deathFx").attribute("path").as_string();
	win1_Fx = config.child("win1Fx").attribute("path").as_string();
	win2_Fx = config.child("win2Fx").attribute("path").as_string();

	graphics_path = config.child("graphics").attribute("path").as_string();
	return ret;
}



// Called before the first frame
bool j1Player::Start(){

	win = false;
	dead = false;
	death.Reset();
	
	current_state = ST_AT_AIR;
	current_animation = &idle;
	playerPosition = startPosLevel1;
	graphics = App->tex->Load(graphics_path.GetString());
		
	App->audio->LoadFx(jump_fx.GetString());
	App->audio->LoadFx(death_fx.GetString());
	App->audio->LoadFx(win1_Fx.GetString());
	App->audio->LoadFx(win2_Fx.GetString());
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	LOG("CLEANUP PLAYER");
	App->tex->UnLoad(graphics);

	for (int i = 0; i < MAXNUMOFCOLLIDERS; i++)//deletes all the hitboxes at the start of the frame
	{
		if (colisionadores[i] != nullptr) {
			colisionadores[i]->to_delete = true;
			colisionadores[i] = nullptr;
		}
	}

	App->tex->UnLoad(graphics);
	App->audio->UnLoad();

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

	if (current_state == ST_CLIMB_IDLE)
		climb.speed = 0;
	else
		climb.speed = 0.15f;

	if (!dead) {
		
		//		- - - - - - PLAYER INPUTS - - - - - - 

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && can_climb) { // Pressing W (Climbing)
			playerPosition.y -= playerClimbSpeed;
			inputs.add(IN_CLIMB);
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP) { // Releasing W
			inputs.add(IN_UPWARDS_UP);
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && (current_state == ST_CLIMB || current_state == ST_CLIMB_IDLE)) { // Pressing S (Climbing)
			playerPosition.y += playerClimbSpeed;
			inputs.add(IN_CLIMB);	
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP && (current_state == ST_CLIMB || current_state == ST_CLIMB_IDLE)) { // Releasing S
			playerPosition.y += 1;
			inputs.add(IN_UPWARDS_UP);
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && current_state != ST_CLIMB) { // Pressing A (Running)
			playerSpeed.x += movementForce.x;
			playerFlip = SDL_FLIP_HORIZONTAL;
			inputs.add(IN_WALK_LEFT);
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) { // Releasing A 
			inputs.add(IN_LEFT_UP);
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { // Pressing D (Running)
			playerSpeed.x -= movementForce.x;
			playerFlip = SDL_FLIP_NONE;
			inputs.add(IN_WALK_RIGHT);
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) { // Releasing D 
			inputs.add(IN_RIGHT_UP);
		}

		//  - - - - - ONLY ON GOD MODE - - - - - - -
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godMode) { // Pressing W in God mode (Flying)
			playerSpeed.y += movementForce.x;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godMode) { // Pressing S in God mode (Flying)
			playerSpeed.y -= movementForce.x;
		}
		// - - - - - - - - - - - - - - - - - - - - - 

		else // If none key is pressed, the player and the camera will start braking.
		{
			braking(); // Smoothy braking when player stops running
			cameraBraking(); // Smoothy camera braking when player is not running
		}


		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && current_state != ST_AT_AIR && // Pressing Space (Jumping)
			current_state != ST_CLIMB && current_state != ST_CLIMB_IDLE) 
		{
			if (canJump == true) {
				App->audio->PlayFx(1, 0, App->audio->FXvolume);
				App->particles->AddParticle(App->particles->dustJumping, playerPosition.x, playerPosition.y + playerTexture.h, playerFlip, COLLIDER_NONE);
			}
					
			playerSpeed.y = movementForce.y;
			inputs.add(IN_JUMPING);
			canJump = false;
		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && current_state == ST_AT_AIR) { // Stay pressing Space (Hovering)

			if (playerAcceleration > hoverAcceleration && playerSpeed.y < hoverSpeedActivation)
				playerAcceleration += (movementForce.y / hoverFallSmooth);

		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) { // Releasing Space
			canJump = true;
		}
	}	

	//Refresh the player state
	player_states state = process_fsm(inputs);
	current_state = state;

	//Get time from frames and it's corrected
	previousTime = frameToSecondValue;
	frameToSecondValue = App->DeltaTime();
	if (frameToSecondValue > maxFrameToSecondValue)
		frameToSecondValue = maxFrameToSecondValue;


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
	//current_animation = &idle;
	switch (current_state)
	{
	case ST_IDLE:		
		
		playerAcceleration = 0;
		playerSpeed.y = 0; // When is at the floor we don't apply any gravity force
		current_animation = &idle;		
		break;


	case ST_AT_AIR:
		current_animation = &jump;
		
		if(!godMode)
			playerSpeed.y += gravityForce; // While it's in the air we apply gravity to get down the player		
		break;

	case ST_WALK_RIGHT:
		current_animation = &walk;
		break;

	case ST_WALK_LEFT:
		current_animation = &walk;
		break;

	case ST_CLIMB:	
		playerAcceleration = 0;
		playerSpeed.x = 0;
		playerSpeed.y = 0;
		current_animation = &climb;
		break;

	case ST_CLIMB_IDLE:
		playerAcceleration = 0;
		playerSpeed.x = 0;
		playerSpeed.y = 0;
		current_animation = &climb;
		break;

	case ST_DEAD:	
		
		if (!dead) {
			inputs.add(IN_WALK_RIGHT); //Returns to idle when restarts player
		}
		
		playerAcceleration = 0;
		playerSpeed.x = 0;
		playerSpeed.y = 0;
		current_animation = &death;
		break;
	}

	if (ignoreColl == true && godMode == false)
		ignoreColl = false;

	for (int i = 0; i < MAXNUMOFCOLLIDERS; i++)
	{
		colisionadores[i] = App->coll->AddCollider({ (int)playerPosition.x, (int)playerPosition.y, playerTexture.w, playerTexture.h }, 
			COLLIDER_PLAYER, 0, 0, 0, 0, this); // Creating player colliders
	}

	return true;
}

bool j1Player::PostUpdate()
{
	
	App->render->Blit(graphics, playerPosition.x, playerPosition.y, 
		&current_animation->GetCurrentFrame(), 1.0, 1.0, playerFlip, NULL, playerTexture.w / 2); // Printing all other player textures

	return true;
}

void j1Player::PlayerPositionUpdate(float dt)
{

	// X AXIS POS
	playerPosition.x = playerPosition.x + playerSpeed.x * dt;

	// Y AXIS POS
	playerPosition.y = playerPosition.y + playerSpeed.y * dt;

	if(!godMode)
		playerSpeed.y = playerSpeed.y + playerAcceleration * dt;
	
}

void j1Player::CameraPositionUpdate(float dt) {

	// X AXIS POS
	if (playerPosition.x > App->win->width / (App->win->scale * 2) - playerTexture.w && playerPosition.x < startCameraFollowingPoint)
		App->render->camera.x = (-playerPosition.x + App->win->width / (App->win->scale * 2) - playerTexture.w ) * App->win->scale;


	// Y AXIS POS
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
	detectCollDir[DIR_RIGHT] = (playerPosition.x + playerTexture.w) - c2->rect.x;
	detectCollDir[DIR_LEFT] = (c2->rect.x + c2->rect.w) - playerPosition.x;
	detectCollDir[DIR_UP] = (c2->rect.y + c2->rect.h) - playerPosition.y;
	detectCollDir[DIR_DOWN] = (playerPosition.y + playerTexture.h) - c2->rect.y;

	bool collDir[DIR_MAX];
	collDir[DIR_RIGHT] = !(detectCollDir[DIR_RIGHT] > 0 && playerSpeed.x < 0);
	collDir[DIR_LEFT] = !(detectCollDir[DIR_LEFT] > 0 && playerSpeed.x > 0);
	collDir[DIR_UP] = !(detectCollDir[DIR_UP] > 0 && playerSpeed.y < 0);
	collDir[DIR_DOWN] = !(detectCollDir[DIR_DOWN] > 0 && playerSpeed.y > 0);

	for (int i = 0; i < MAXNUMOFCOLLIDERS; i++)
	{
		bool alredycollided = false;

		// - - - - - - - COLLISIONS LOGIC - - - - - - - 

		int dirCheck = DIR_UNKNOWN;

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
		if (ignoreColl == false) {

			if ((c2->type == COLLIDER_DEATH))
			{
			
				inputs.add(IN_DEAD);
				dead = true;
			}

			if ((c2->type == COLLIDER_WIN))
			{
				inputs.add(IN_WALK_RIGHT);
				win = true;
			}
			
			if ((c2->type == COLLIDER_CLIMB))
			{				
				can_climb = true;
				
			}

			if ((c2->type == COLLIDER_FLOOR))
			{
			
				switch (dirCheck) {

				case DIR_UP:

					playerPosition.y = c2->rect.y + c2->rect.h + 1;
					playerSpeed.y = 0;
				break;

				case DIR_DOWN:
			
					playerPosition.y = c2->rect.y - playerTexture.h;
					playerAcceleration = 0;
					playerSpeed.y = 0;							
					jump.Reset();
					inputs.add(IN_JUMP_FINISH);
					checkingFall = false;
					break;

				case DIR_LEFT:

					playerPosition.x = c2->rect.x + c2->rect.w + 1;
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

			if(playerSpeed.y >= 0){

				if ((c2->type == COLLIDER_PLATFORM))
				{
					switch (dirCheck) {

					case DIR_UP:
						break;

					case DIR_DOWN:
						playerPosition.y = c2->rect.y - playerTexture.h;
						playerSpeed.y = 0;
						playerAcceleration = 0;
						checkingFall = false;
						inputs.add(IN_JUMP_FINISH);
						break;

					case DIR_LEFT:
						break;

					case DIR_RIGHT:
						break;
					case -1:
						break;
					}
				}
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
	if(playerSpeed.x < 0)
		playerSpeed.x /= slowlingValue; // Smoothy braking when player stops running (We need to improve it)
	else
		playerSpeed.x /= slowlingValue;

	if (godMode) {
		if (playerSpeed.y < 0)
			playerSpeed.y /= slowlingValue; // Smoothy braking when player stops running (We need to improve it)
		else
			playerSpeed.y /= slowlingValue;
	}
}


player_states j1Player::process_fsm(p2List<player_inputs>& inputs)
{
	can_climb = false;

	static player_states state = ST_IDLE;
	player_inputs  last_input;

	while (inputs.Pop(last_input))
	{
		switch (state)
		{
		case ST_IDLE:
		{
			switch (last_input)
			{
			case IN_JUMPING: state = ST_AT_AIR;
				break;

			case IN_FALLING: state = ST_AT_AIR;
				break;
			case IN_WALK_RIGHT: state = ST_WALK_RIGHT;
				break;
			case IN_WALK_LEFT: state = ST_WALK_LEFT;
				break;
			case IN_CLIMB: state = ST_CLIMB; break;
			}
		}
		break;

		case ST_AT_AIR:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;
			case IN_CLIMB: state = ST_CLIMB; break;
			case IN_DEAD: state = ST_DEAD;	break;
			}
		}
		break;
		
		case ST_WALK_RIGHT:
			switch (last_input)
			{
			case IN_CLIMB: state = ST_CLIMB; break;
			case IN_RIGHT_UP: state = ST_IDLE; break;
			case IN_JUMPING: state = ST_AT_AIR;
				break;
			case IN_FALLING: state = ST_AT_AIR;
				break;
			case IN_WALK_LEFT: state = ST_IDLE;
				break;

			}
			break;

		case ST_WALK_LEFT:
			switch (last_input)
			{
			case IN_CLIMB: state = ST_CLIMB; break;
			case IN_LEFT_UP: state = ST_IDLE; break;
			case IN_JUMPING: state = ST_AT_AIR;
				break;
			case IN_FALLING: state = ST_AT_AIR;
				break;
			case IN_WALK_RIGHT: state = ST_IDLE;
				break;

			}
			break;

		case ST_CLIMB:
			switch (last_input)
			{
			case IN_UPWARDS_UP: state = ST_CLIMB_IDLE; break;				
			case IN_WALK_RIGHT: state = ST_WALK_RIGHT;
				break;
			case IN_WALK_LEFT: state = ST_WALK_LEFT;
				break;

			}
			break;

		case ST_CLIMB_IDLE:
		{
			switch (last_input)
			{
			
			case IN_WALK_RIGHT: state = ST_WALK_RIGHT;
				break;
			case IN_WALK_LEFT: state = ST_WALK_LEFT;
				break;
			case IN_CLIMB: state = ST_CLIMB; break;
			}
		}
		break;

		case ST_DEAD:
		{
			switch (last_input)
			{

			case IN_WALK_RIGHT: state = ST_WALK_RIGHT;
				break;		
			}
		}
		break;
		}
	}
	return state;
}