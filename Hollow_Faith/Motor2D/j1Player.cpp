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
#include "j1Objects.h"




j1Player::j1Player() : j1Entity(entityType::PLAYER)
{
	if (App->objects->player == nullptr)
		App->objects->player = this;
}

j1Player::~j1Player() {};

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");
	bool ret = true;	

	texture_path = config.child("graphics_player").attribute("path").as_string();

	// Loading all Animations
	pugi::xml_node animIterator = config.child("animations").child("animation");

	idle.load_animation(animIterator, "idle");
	walk.load_animation(animIterator, "walk");
	jump.load_animation(animIterator, "jump");
	death.load_animation(animIterator, "death");
	climb.load_animation(animIterator, "climb");

	// Loading all Player FX

	pugi::xml_node fxIterator = config.child("fx");
	jump_fx = fxIterator.child("jumpFx").attribute("path").as_string();
	death_fx = fxIterator.child("deathFx").attribute("path").as_string();
	win1_Fx = fxIterator.child("win1Fx").attribute("path").as_string();
	win2_Fx = fxIterator.child("win2Fx").attribute("path").as_string();
	landing_Fx = fxIterator.child("landingFx").attribute("path").as_string();
	hover_Fx = fxIterator.child("hoverFx").attribute("path").as_string();
	stone_Fx = fxIterator.child("stoneFx").attribute("path").as_string();	

	// Player data

	pugi::xml_node dataIterator = config.child("data");
	
	startPosLevel1.x = dataIterator.child("StartPosLevel1").attribute("x").as_float();
	startPosLevel1.y = dataIterator.child("StartPosLevel1").attribute("y").as_float();
	
	startPosLevel2.x = dataIterator.child("StartPosLevel2").attribute("x").as_float();
	startPosLevel2.y = dataIterator.child("StartPosLevel2").attribute("y").as_float();
	
	playerClimbSpeed = dataIterator.child("climbspeed").attribute("value").as_float();

	speed.x = dataIterator.child("speed").attribute("x").as_float();
	speed.x = dataIterator.child("speed").attribute("y").as_float();

	Acceleration = dataIterator.child("acceleration").attribute("value").as_float();

	movementForce.x = dataIterator.child("MovementForce").attribute("x").as_float();
	movementForce.y = dataIterator.child("MovementForce").attribute("y").as_float();

	hoverAcceleration = dataIterator.child("hover_acceleration").attribute("value").as_float();
	hoverSpeedActivation = dataIterator.child("hover_speed_activation").attribute("value").as_float();
	hoverFallSmooth = dataIterator.child("hover_fall_smooth").attribute("value").as_float();

	speedLimit.x = dataIterator.child("speed_limit").attribute("x").as_float();
	speedLimit.y = dataIterator.child("speed_limit").attribute("y").as_float();
	
	gravityForce = dataIterator.child("gravity").attribute("value").as_float();
	slowingValue = dataIterator.child("slowing_value").attribute("value").as_float();
	slowlingLimitValue = dataIterator.child("slowing_limit").attribute("value").as_int();

	entity_collider = { 0, 0, 17, 27 };
	collider = new Collider(entity_collider, COLLIDER_PLAYER, this);	
	
	return ret;
}

// Called before the first frame
bool j1Player::Start()
{	
	texture = App->tex->Load(texture_path.GetString());
	App->coll->AddColliderEntity(collider);
	win = false;
	dead = false;
	
	current_state = ST_AT_AIR;
	current_animation = &idle;

	if (App->scene->different_map) { //We make sure to load all positions when F6 is pressed and we are changing scene
	
		App->LoadGame();
		App->scene->different_map = false;
	}
	else
		position = startPosLevel1;


	App->audio->LoadFx(jump_fx.GetString());
	App->audio->LoadFx(death_fx.GetString());
	App->audio->LoadFx(win1_Fx.GetString());
	App->audio->LoadFx(win2_Fx.GetString());
	App->audio->LoadFx(landing_Fx.GetString());
	App->audio->LoadFx(hover_Fx.GetString());
	App->audio->LoadFx(stone_Fx.GetString());

	

	return true;
}

// Called before quitting
void j1Player::CleanUp()
{	
	LOG("CLEANUP PLAYER");
	App->audio->UnLoad();
	death.Reset();

}
// Called each loop iteration
bool j1Player::PreUpdate()
{	
	
	if (!dead) {
		
		//		- - - - - - PLAYER INPUTS - - - - - - 

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && can_climb) { // Pressing W (Climbing)
			position.y -= playerClimbSpeed * (App->dt * 50);
			inputs.add(IN_CLIMB);
		} 
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP) { // Releasing W		
			inputs.add(IN_UPWARDS_UP);
		}
	
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && can_climb) { // Pressing S (Climbing)
			position.y += playerClimbSpeed * (App->dt * 50);
			inputs.add(IN_CLIMB);	
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) { // Releasing S
			//position.y += 1;
			inputs.add(IN_UPWARDS_UP);
		}

		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && can_climb && (current_state == ST_CLIMB || current_state == ST_CLIMB_IDLE)) { // Pressing A (Running)
			position.x -= playerClimbSpeed * (App->dt * 50);
			inputs.add(IN_CLIMB);
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && can_climb && (current_state == ST_CLIMB || current_state == ST_CLIMB_IDLE)) { // Pressing A (Running)
			position.x += playerClimbSpeed * (App->dt * 50);
			inputs.add(IN_CLIMB);
		}

		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { // Pressing A (Running)
			speed.x += movementForce.x;
			flip = SDL_FLIP_HORIZONTAL;
			inputs.add(IN_WALK_LEFT);
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) { // Releasing A 
			inputs.add(IN_LEFT_UP);
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { // Pressing D (Running)
			speed.x -= movementForce.x;
			flip = SDL_FLIP_NONE;
			inputs.add(IN_WALK_RIGHT);
		}		
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP ) { // Releasing D 
			inputs.add(IN_RIGHT_UP);
		}

		//  - - - - - ONLY ON GOD MODE - - - - - - -
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godMode) { // Pressing W in God mode (Flying)
			speed.y += movementForce.x;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godMode) { // Pressing S in God mode (Flying)
			speed.y -= movementForce.x;
		}
		// - - - - - - - - - - - - - - - - - - - - - 

		else // If none key is pressed, the player and the camera will start braking.
		{
			braking(); // Smoothy braking when player stops running
			cameraBraking(); // Smoothy camera braking when player is not running
		}


		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && current_state != ST_AT_AIR && // Pressing Space (Jumping)
			current_state != ST_CLIMB && current_state != ST_CLIMB_IDLE) 
		{
			
			App->audio->PlayFx(1, 0, App->audio->FXvolume);
			App->objects->particle->AddParticle(App->objects->particle->dustJumping, position.x, position.y + entity_collider.h, flip, COLLIDER_NONE);
			speed.y = movementForce.y;
			inputs.add(IN_JUMPING);
		
		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && current_state == ST_AT_AIR) { // Stay pressing Space (Hovering)

			if (Acceleration > hoverAcceleration && speed.y < hoverSpeedActivation)
				Acceleration += (movementForce.y / hoverFallSmooth) * (App->dt * 43);

		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP ) { // Releasing Space

		}
		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN && !can_climb) {

			if (flip)
				App->objects->particle->stone.speed.x = -5;
			else
				App->objects->particle->stone.speed.x = 5;

			App->objects->particle->stone.speed.y = -12;

			if (App->objects->particle->elim)
			{
				App->objects->particle->elim = false;
				App->objects->particle->AddParticle(App->objects->particle->stone, position.x, position.y + entity_collider.h / 4, flip, COLLIDER_STONE, "stone");
				App->audio->PlayFx(7, 0, 100);
			}

			
		}
	}	

	//Refresh the player state
	player_states state = process_fsm(inputs);
	current_state = state;

	//Update position related to real time and puts speed limit.
	speedLimitChecker();
	PositionUpdate(App->dt);

	//Update position related to real time and player position.
	cameraSpeedLimitChecker();
	CameraPositionUpdate(frameToSecondValue);

		return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	
	Win_Lose_Condition();
	//current_animation = &idle;
	switch (current_state)
	{
	case ST_IDLE:		
		
		Acceleration = 0;
		speed.y = 0; // When is at the floor we don't apply any gravity force
		current_animation = &idle;		
		break;


	case ST_AT_AIR:
		current_animation = &jump;
		
		if(!godMode)
			speed.y += gravityForce * (dt * 51) ; 
		
		break;

	case ST_WALK_RIGHT:
		current_animation = &walk;
		break;

	case ST_WALK_LEFT:
		current_animation = &walk;
		break;

	case ST_CLIMB:	// When the player is climbing 
		Acceleration = 0;
		speed.x = 0;
		speed.y = 0;
		climb.speed = 0.15f;

		current_animation = &climb;
		break;

	case ST_CLIMB_IDLE: // When the player is stop climbing
		Acceleration = 0;
		speed.x = 0;
		speed.y = 0;
		climb.speed = 0;
			
		current_animation = &climb;
		break;

	case ST_DEAD:	
		
		if (!dead) {
			inputs.add(IN_WALK_RIGHT); //Returns to idle when restarts player
		}
		
		Acceleration = 0;
		speed.x = 0;
		speed.y = 0;
		current_animation = &death;
		break;
	}

	if (App->scene->ready_to_load == true) {
		ignoreColl = true;
	}
	if (ignoreColl == true && godMode == false && App->scene->ready_to_load==false)
		ignoreColl = false;	

	//Checking if there is no collision, then apply gravity

	if (checkingFall)
		inputs.add(IN_FALLING);

	checkingFall = true;	


	

	return true;
}

bool j1Player::PostUpdate() {

	Draw(App->dt);

	return true;
}

void j1Player::CameraPositionUpdate(float dt) { // Camera movement system

	// X AXIS POS	
	
	if (position.x > App->win->width / (App->win->scale * 2) - entity_collider.w && position.x < startCameraFollowingPoint)
		App->render->camera.x = (-position.x + App->win->width / (App->win->scale * 2) - entity_collider.w ) * App->win->scale;


	// Y AXIS POS
	App->render->camera.y = App->render->camera.y + cameraSpeed.y * dt;
}

void j1Player::speedLimitChecker()
{

	//POSITIVE

	if (speed.x > speedLimit.x)
		speed.x = speedLimit.x;

	if (speed.y > speedLimit.y)
	{
		speed.y = speedLimit.y;
		Acceleration = Acceleration;
	}


	//NEGATIVE

	if (speed.x < -speedLimit.x)
		speed.x = -speedLimit.x;

	if (speed.y < -speedLimit.y)
	{
		speed.y = -speedLimit.y;
		Acceleration = Acceleration;
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

	
	// - - - - - - - COLLISIONS LOGIC - - - - - - - 

	int detectCollDir[DIR_MAX];
	detectCollDir[DIR_UP] = (c2->rect.y + c2->rect.h) - position.y;
	detectCollDir[DIR_DOWN] = (position.y + entity_collider.h) - c2->rect.y;
	detectCollDir[DIR_RIGHT] = (position.x + entity_collider.w) - c2->rect.x;
	detectCollDir[DIR_LEFT] = (c2->rect.x + c2->rect.w) - position.x;
	

	bool collDir[DIR_MAX];
	collDir[DIR_UP] = !(detectCollDir[DIR_UP] > 0 && speed.y < 0);
	collDir[DIR_DOWN] = !(detectCollDir[DIR_DOWN] > 0 && speed.y > 0);
	collDir[DIR_RIGHT] = !(detectCollDir[DIR_RIGHT] > 0 && speed.x < 0);
	collDir[DIR_LEFT] = !(detectCollDir[DIR_LEFT] > 0 && speed.x > 0);
		
		
	int dirCheck = DIR_UNKNOWN;

	for (int i = 0; i < DIR_MAX; ++i)
	{
		if (dirCheck == DIR_UNKNOWN)
			dirCheck = i;
		else if ((detectCollDir[i] < detectCollDir[dirCheck]))
			dirCheck = i;
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
				if ((position.x + 10 > c2->rect.x && position.x < c2->rect.x + c2->rect.w)
					&& (position.y + 15 > c2->rect.y && position.y < c2->rect.y + c2->rect.h)
					&& (position.y + entity_collider.h - 1 < c2->rect.y + c2->rect.h)
					&& (position.x + 5 < c2->rect.x + c2->rect.w))
					can_climb = true;
				else if (position.y +15< c2->rect.y) {
					position.y = c2->rect.y - 14;
					can_climb = true;
				}
				
				else{
					
					can_climb = false;
					current_state = ST_AT_AIR;
				}
			}

			if ((c2->type == COLLIDER_FLOOR))
			{
			
				switch (dirCheck) {

				case DIR_UP:

					position.y = c2->rect.y + c2->rect.h + 1;
					speed.y = 0;
				break;

				case DIR_DOWN:
			
					position.y = c2->rect.y - entity_collider.h;
					Acceleration = 0;
					speed.y = 0;							
					jump.Reset();
					inputs.add(IN_JUMP_FINISH);
					checkingFall = false;
					break;

				case DIR_LEFT:

					position.x = c2->rect.x + c2->rect.w + 1;
					speed.x = 0;
					break;

				case DIR_RIGHT:

					position.x = c2->rect.x - entity_collider.w;
					speed.x = 0;
					break;

				case -1:
					break;					
				}	
				
			}

			if ((c2->type == COLLIDER_ENEMY))
			{
				switch (dirCheck) {

				case DIR_UP:
					inputs.add(IN_DEAD);
					dead = true;
					break;

				case DIR_DOWN:
					speed.y = movementForce.y / 2;
					c2->callback->elim = true;
					inputs.add(IN_JUMPING);
					break;

				case DIR_LEFT:
					inputs.add(IN_DEAD);
					dead = true;
					break;

				case DIR_RIGHT:
					inputs.add(IN_DEAD);
					dead = true;
					break;
				case -1:
					break;
				}
			}
			
			if(speed.y >= 0){

				if ((c2->type == COLLIDER_PLATFORM))
				{
					switch (dirCheck) {

					case DIR_UP:
						break;

					case DIR_DOWN:
						position.y = c2->rect.y - entity_collider.h;
						speed.y = 0;
						Acceleration = 0;
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

		CollisionPosUpdate();
}


void j1Player::cameraBraking()
{
	if (cameraSpeed.x > 0)
		cameraSpeed.x /= slowingValue; // Smoothy braking when player stops running (We need to improve it)
	else
		cameraSpeed.x /= -slowingValue;
}

void j1Player::braking()
{
	if(speed.x < 0)
		speed.x /= slowingValue; // Smoothy braking when player stops running (We need to improve it)
	else
		speed.x /= slowingValue;

	if (godMode) {
		if (speed.y < 0)
			speed.y /= slowingValue; // Smoothy braking when player stops running (We need to improve it)
		else
			speed.y /= slowingValue;
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
			case IN_CLIMB: state = ST_CLIMB; break;
			case IN_DEAD: state = ST_DEAD;	break;
			}
		}
		break;

		case ST_AT_AIR:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; 
				App->objects->particle->AddParticle(App->objects->particle->dustJumping, position.x, position.y + entity_collider.h, flip, COLLIDER_NONE);
				App->objects->particle->AddParticle(App->objects->particle->dustRunning, position.x, position.y + entity_collider.h - particlePosMargin, flip, COLLIDER_NONE);
				App->audio->PlayFx(5, 0, App->audio->FXvolume);
				break;
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
			case IN_DEAD: state = ST_DEAD;	break;

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
			case IN_DEAD: state = ST_DEAD;	break;

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
			case IN_DEAD: state = ST_DEAD;	break;
				
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
			case IN_DEAD: state = ST_DEAD;	break;
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

// Load Game State
void j1Player::Load(pugi::xml_node& data)
{	
	
	if (App->scene->different_map) {

		App->scene->ready_to_load = true;
		ignoreColl = true;	
		savedPosition.x = data.child("position").attribute("x").as_int();
		savedPosition.y= data.child("position").attribute("y").as_int();
	}	

	else if (!App->scene->different_map) {
		
		position.x = data.child("position").attribute("x").as_int();
		position.y = data.child("position").attribute("y").as_int();		
	}
	
}

// Save Game State
void j1Player::Save(pugi::xml_node& data) const
{	
	pugi::xml_node pos = data.append_child("player").append_child("position");

	pos.append_attribute("x").set_value(position.x);
	pos.append_attribute("y").set_value(position.y);

}


void j1Player::Win_Lose_Condition() {

	// Winning at map 1--------------------------
	if (win && App->scene->currentmap == 1) {
		
		if (!App->scene->sound_repeat && App->scene->currentmap == 1) {
			App->audio->PlayFx(3, 0, App->audio->FXvolume);
			App->scene->sound_repeat = true;
		}

		App->scene->currentmap = 2;

		for (int i = 1; i <= App->map->data.numLevels; i++) {
			if (App->scene->currentmap == i)
				App->fade_to_black->FadeToBlack(App->map->data.levels[i - 1]->name.GetString(), 2.0f);
		}
	}
	
	// Winning at map 2--------------------------
	else if (win && App->scene->currentmap == 2) {
		
		if (!App->scene->sound_repeat && App->scene->currentmap == 2) {
			App->audio->PlayFx(4, 0, App->audio->FXvolume);
			App->scene->sound_repeat = true;
		}

		App->scene->currentmap = 1;

		for (int i = 1; i <= App->map->data.numLevels; i++) {
			if (App->scene->currentmap == i)
				App->fade_to_black->FadeToBlack(App->map->data.levels[i - 1]->name.GetString(), 3.0f);
		}
	}

	// Dying -----------------------------------------
	if (dead) {		

		if (!App->scene->sound_repeat) {
			App->audio->PlayFx(2, 0, App->audio->FXvolume);
			death.Reset();
			App->scene->sound_repeat = true;
		}

		for (int i = 1; i <= App->map->data.numLevels; i++) {
			if (App->scene->currentmap == i)
				App->fade_to_black->FadeToBlack(App->map->data.levels[i - 1]->name.GetString(), 2.0f);
		}
	}
}