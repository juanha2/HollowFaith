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
	name.create("player");
	idle.PushBack({ 7,164,17,28 });
	idle.PushBack({ 39,164,17,28 });
	idle.PushBack({ 70,163,18,29 });
	idle.PushBack({ 7,164,17,28 });
	idle.loop = true;
	idle.speed = 0.06f;

	walk.PushBack({ 7,37,17,27 });
	walk.PushBack({ 39,36,17,28 });
	walk.PushBack({ 71,36,17,28 });
	walk.PushBack({ 102,37,18,27 });
	walk.PushBack({ 135,36,17,28 });
	walk.PushBack({ 167,36,17,28 });
	walk.loop = true;
	walk.speed = 0.08f;

	jump.PushBack({ 231,36,17,28 });
	jump.PushBack({ 261,38,20,26 });
	jump.PushBack({ 292,38,22,26 });
	jump.PushBack({ 327,34,17,30 });
	jump.PushBack({ 358,32,18,29 });
	jump.PushBack({ 389,34,20,29 });
	jump.PushBack({ 420,37,22,27 });
	jump.PushBack({ 454,38,18,26 });
	jump.loop = true;
	jump.firstLoopFrame = 5;
	jump.speed = 0.15f;
}
j1Player::~j1Player() {

};

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");
	bool ret = true;	
	
	jump_fx = config.child("jumpFx").attribute("path").as_string();
	
	return ret;
}



// Called before the first frame
bool j1Player::Start(){

	graphics = App->tex->Load("Assets/Sprites/Monster.png");
	App->audio->LoadFx(jump_fx.GetString());	

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

	//		- - - - - - CAMERA FOLLOW - - - - - - 	

	if (playerPosition.x > cameraFollowingPoint) // Starts to follow the player faster in X position.
	{

		if(playerPosition.x)
			cameraSpeed.x -= playerSpeed.x;
	}
	else 
		cameraSpeed.x -= playerSpeed.x / 40;	
		
	
	//		- - - - - - PLAYER INPUTS - - - - - - 

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		playerSpeed.x += movementForce.x;
		playerFlip = SDL_FLIP_HORIZONTAL;
		inputs.add(IN_WALK_LEFT);
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {	
		inputs.add(IN_LEFT_UP);
	}		
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		playerSpeed.x -= movementForce.x;
		playerFlip = SDL_FLIP_NONE;
		inputs.add(IN_WALK_RIGHT);
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {		
		inputs.add(IN_RIGHT_UP);
	}
	//  - - - - - ONLY ON GOD MODE - - - - - - -
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godMode) {
		playerSpeed.y += movementForce.x;
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godMode) {
		playerSpeed.y -= movementForce.x;
	}
	// - - - - - - - - - - - - - - - - - - - - - 
	else 
	{
		braking(); // Smoothy braking when player stops running
		cameraBraking(); // Smoothy camera braking when player is not running
	}
	

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && current_state != ST_AT_AIR) // Jumping
	{
		App->audio->PlayFx(1,0,20);
		playerSpeed.y = movementForce.y; 
		inputs.add(IN_JUMPING);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !godMode)
		playerAcceleration += -movementForce.y; // Get down while you're in the air faster




	//Refresh the player state
	player_states state = process_fsm(inputs);
	current_state = state;

	//Get time from frames and it's corrected
	previousTime = frameToSecondValue;
	frameToSecondValue = App->DeltaTime();
	if (frameToSecondValue > 0.16)
		frameToSecondValue = 0.16f;


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

	}

	if (ignoreColl == true && godMode == false)
		ignoreColl = false;

	for (int i = 0; i < MAXNUMOFCOLLIDERS; i++)
	{
		colisionadores[i] = App->coll->AddCollider({ (int)playerPosition.x, (int)playerPosition.y, playerTexture.w, playerTexture.h }, COLLIDER_PLAYER, 0, 0, 0, 0, this); // Creating player colliders
	}

	return true;
}


bool j1Player::PostUpdate()
{
	App->render->Blit(graphics, playerPosition.x, playerPosition.y, &current_animation->GetCurrentFrame(), 1.0, 1.0, playerFlip , NULL , playerTexture.w / 2); // Printing player texture
	
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
	if (playerPosition.x > 1024 / 4 - playerTexture.w)
		App->render->camera.x = (-playerPosition.x + 1024 / 4 - playerTexture.w ) * 2;

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

		if ((c2->type == COLLIDER_FLOOR))
		{
			switch (dirCheck) {

			case DIR_UP:
				//if(playerSpeed.y < 0)
				playerPosition.y = c2->rect.y + c2->rect.h + 1;
				playerSpeed.y = 0;
				break;

			case DIR_DOWN:
				playerPosition.y = c2->rect.y - playerTexture.h;
				playerSpeed.y = 0;
				playerAcceleration = 0;
				checkingFall = false;
				jump.Reset();
				inputs.add(IN_JUMP_FINISH);
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

		if(playerSpeed.y >= 0)
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
			}
		}
		break;

		case ST_AT_AIR:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE;
				break;

			case IN_RIGHT_UP: state = ST_IDLE; break;
			}
		}
		break;
		
		case ST_WALK_RIGHT:
			switch (last_input)
			{
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
			case IN_LEFT_UP: state = ST_IDLE; break;
			case IN_JUMPING: state = ST_AT_AIR;
				break;
			case IN_FALLING: state = ST_AT_AIR;
				break;
			case IN_WALK_RIGHT: state = ST_IDLE;
				break;

			}
			break;
		}
	}
	return state;
}