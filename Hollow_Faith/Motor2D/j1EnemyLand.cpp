#include "p2Defs.h"
#include "p2Log.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Particles.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1EnemyLand.h"
#include "j1Scene.h"

j1EnemyLand::j1EnemyLand() : j1Enemy(entityType::ENEMY_LAND) {}

j1EnemyLand::j1EnemyLand(fPoint pos) : j1Enemy(entityType::ENEMY_LAND, pos) {  }

j1EnemyLand::~j1EnemyLand() {};

bool j1EnemyLand::Awake(pugi::xml_node& config)
{	
	bool ret = true;

	pugi::xml_node dataIterator = config.child("data");
	pugi::xml_node enemydata = config.child("enemydata");	

	//Loading enemy Data
	texture_path = config.child("graphics_enemy").attribute("path").as_string();
	speedLimit.y = dataIterator.child("speed_limit").attribute("y").as_float();
	speedLimit.x = dataIterator.child("speed_limit").attribute("x").as_float();
	pathCadency = enemydata.child("pathCadency").attribute("value").as_float();
	pathMinDist = enemydata.child("pathMinDist").attribute("value").as_int();
	agroDistance = enemydata.child("agroDistance").attribute("value").as_int();
	movementForce.x = enemydata.child("movementForce").attribute("x").as_float();
	movementForce.y = enemydata.child("movementForce").attribute("y").as_float();
	jumpDistance = enemydata.child("jumpDistance").attribute("value").as_int();
	extraJumpForce = enemydata.child("extraJumpForce").attribute("value").as_int();	
	gravityForce = config.child("data").child("gravity").attribute("value").as_float();

	// Loading all Animations
	pugi::xml_node animIterator = config.child("animations").child("animation");
	idle.load_animation(animIterator, "idleLand");
	walk.load_animation(animIterator, "walkLand");
	jump.load_animation(animIterator, "jumpLand");

	// Loading collider
	entity_collider.x = dataIterator.child("collider").attribute("x").as_int();
	entity_collider.y = dataIterator.child("collider").attribute("y").as_int();
	entity_collider.w = dataIterator.child("collider").attribute("w").as_int();
	entity_collider.h= dataIterator.child("collider").attribute("h").as_int();

	return ret;
}

bool j1EnemyLand::Start()
{
	bool ret = true;

	// Loading Texture and collider
	texture = App->tex->Load(texture_path.GetString());
	collider = new Collider(entity_collider, COLLIDER_ENEMY, this);
	App->coll->AddColliderEntity(collider);

	

	//Setting initial values
	current_animation = &idle;
	ignoreColl = false;
	canJump = true;
	originalPos.x = position.x;
	originalPos.y = position.y;

	CollisionPosUpdate();

	return ret;
}

bool j1EnemyLand::PreUpdate()
{
	bool ret = true;

	if (elim) // When enemy dies
	{
		App->scene->score += 500;
		App->audio->PlayFx(10, 0, App->audio->SpatialAudio(App->audio->FXvolume, distance));		
		App->objects->DeleteEntity();
	}

	if (hurtedConsec) // Gets the first stone hit
	{
		timeConsec += App->dt;

		if (timeConsec <= 0.10f)
		{
			SDL_SetTextureColorMod(texture, 255, 0, 0);
			SDL_SetTextureColorMod(texture, 255, 0, 0);
			speed.y = movementForce.y / 3;

			if (flip == SDL_FLIP_NONE)
				speed.x = movementForce.y;
			else
				speed.x = -movementForce.y;	
		}
		else 
		{
			SDL_SetTextureColorMod(texture, 255, 255, 255);
			speed.x = 0;
			hurtedConsec = false;
		}		
	}

	
	// We check after update the speed
	speedLimitChecker(); 

	return ret;
}


bool j1EnemyLand::Update(float dt)
{
	BROFILER_CATEGORY("Update_LandEnemy", Profiler::Color::DarkGreen);

	bool ret = true;

	if (canJump) // If the enemy isn't in the air
		GeneratingThePath(pathCadency, dt, agroDistance); // Generates a path with a X cadency, using the time and only when the distance between player and enemy is X
	else 
	{
		pathToPlayer.Clear();
		chase = false;
	}
	

	if (chase)  
	{	
		if (!PathJumping()) // If the enemy is not ready to jump let's follow the path
		{
			if (!FollowingThePath(movementForce.x, dt)) // Follows the path logic generated
			{
				pathToPlayer.Clear();
				speed.x = 0;
				current_animation = &idle;

				chase = false;
			}
		}
		else if (Stop().x != -1) // If there is floor in the next path tile, let's follows the path
		{
			if (!FollowingThePath(movementForce.x, dt)) // Follows the path logic generated
			{
				pathToPlayer.Clear();
				speed.x = 0;
				current_animation = &idle;

				chase = false;
			}
		}
		else // If enemy won't jump or follow the path, let's Stop.
		{
			speed.x = 0;
			current_animation = &idle;
		}
	}
	
	// Update with dt the enemy position all frames
	PositionUpdate(dt);
	
	JumpFallLogic(dt); // gravity and X jump force

	return ret;
}

bool j1EnemyLand::PostUpdate()
{
	bool ret = true;

	if (App->scene->debug) // PathFinding personal debug
	{
		for (uint i = 0; i < pathToPlayer.Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(pathToPlayer[i].x, pathToPlayer[i].y);
			App->render->Blit(App->scene->debug_tex, pos.x, pos.y);
		}
	}
	// Normalized Animations with DeltaTime
	Draw(App->dt);

	return ret;
}

bool j1EnemyLand::CleanUp()
{
	//Unloading data
	App->tex->UnLoad(texture);

	collider->to_delete = true;

	pathToPlayer.Clear();
	

	return true;
}


// ----------------------------------------------------------------------------------------
// We generate the path from our Enemy position to the Player position
// ----------------------------------------------------------------------------------------
bool j1EnemyLand::GeneratingThePath(float auxTimer, float dt, int auxAgroDistance)
{
	BROFILER_CATEGORY("LandEnemy_GENERATING_PathLogic", Profiler::Color::ForestGreen);

	timer += dt;

		distance = abs(App->objects->player->position.x - position.x); // Always get the distance only in X axis

	if (distance < auxAgroDistance)
	{
		if (timer > auxTimer)
		{

			int hostileValue; // Value to get inside the Player and kill him, and not to follow him friendly.

			// 16 px are the Width and Height of each path tile (Not a magic number)
			if (position.x > App->objects->player->position.x)
				hostileValue = -16;
			else
				hostileValue = +16;


			pathToPlayer.Clear();

			App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y),
				App->map->WorldToMap(App->objects->player->position.x + hostileValue, App->objects->player->position.y));

			for (uint i = 0; i < App->pathfinding->GetLastPath()->Count(); i++)
			{
				pathToPlayer.PushBack(*App->pathfinding->GetLastPath()->At(i));
			}


			chase = true;
			timer = 0;
		}
	}

	return true;
}


// ----------------------------------------------------------------------------------------
//  We follow the generated path in X Axis
// ----------------------------------------------------------------------------------------
bool j1EnemyLand::FollowingThePath(float auxSpeed, float dt) {

	BROFILER_CATEGORY("LandEnemy_FOLLOWING_PathLogic", Profiler::Color::Green);
	
	iPoint current = App->map->MapToWorld(pathToPlayer[pathToPlayer.Count() - 1].x, pathToPlayer[pathToPlayer.Count() - 1].y);

	if ((abs(position.x - current.x) > pathMinDist)) // Go to the next tile
	{
		if (current.x > position.x)
		{
			current_animation = &walk;

			if (speed.x < 0)
				speed.x = 0;

			speed.x += auxSpeed * dt;
			flip = SDL_FLIP_NONE;
		}
		else
		{
			current_animation = &walk;

			if (speed.x > 0)
				speed.x = 0;

			speed.x -= auxSpeed * dt;
			flip = SDL_FLIP_HORIZONTAL;
		}
	}
	else
	{
		pathToPlayer.Pop(current); // When we reach the next tile, pop it, and go to the next or stop if that was the last one.
	}


	
	if (pathToPlayer.Count() <= 0)
		return false;
	else
		return true;
}


// ----------------------------------------------------------------------------------------
//  We detect if the enemy is ready to jump with pathfinding, distance and enemy state 
// ----------------------------------------------------------------------------------------
bool j1EnemyLand::PathJumping() 
{

	BROFILER_CATEGORY("LandEnemy_PATHJUMP_PathLogic", Profiler::Color::Green);

	if (pathToPlayer.Count() > 1)
	{
		iPoint next = { pathToPlayer[pathToPlayer.Count() - 2].x, pathToPlayer[pathToPlayer.Count() - 2].y + 2 }; 

		if (App->pathfinding->IsWalkable(next) && !checkingFall)
		{

			iPoint result = { -1, -1 };

			iPoint next = { pathToPlayer[pathToPlayer.Count() - 1].x, pathToPlayer[pathToPlayer.Count() - 1].y + 2 }; 

			if (App->pathfinding->IsWalkable(next) && !checkingFall)
			{
				for (uint i = pathToPlayer.Count(); i > 0; --i)
				{
					next = { pathToPlayer[i].x, pathToPlayer[i].y + 2 };

					if (!App->pathfinding->IsWalkable(next))
					{
						result = next; // Get if there will be some floor to land
						break;
					}
				}
			}

			iPoint auxNext = result;

			if (auxNext.x != -1 && auxNext.y != -1)
			{
				if (distance < jumpDistance)
				{
					if (position.x < App->objects->player->position.x && flip == SDL_FLIP_NONE) // If the player is close enough and the sprite is looking and it, can jump
					{
						if (canJump)
						{
							speed.y = movementForce.y - 40;
							canJump = false;
						}

					}
					else if (position.x > App->objects->player->position.x && flip == SDL_FLIP_HORIZONTAL)
					{
						if (canJump)
						{
							speed.y = movementForce.y - 40;
							canJump = false;
						}
					}
					else
						return true;
				}
				else
					return true;
			}
			return true;
		}
	}

	return false;
}


// ----------------------------------------------------------------------------------------
// We detect if there is floor in the next path tile and return the position 
// ----------------------------------------------------------------------------------------
iPoint j1EnemyLand::Stop()
{

	BROFILER_CATEGORY("LandEnemy_PATHSTOP_PathLogic", Profiler::Color::Green);

	iPoint result = { -1, -1 }; 

	iPoint next = { pathToPlayer[pathToPlayer.Count() - 1].x, App->map->WorldToMap(position.x, round(position.y)).y + 3 };

	if (!App->pathfinding->IsWalkable(next))
	{
		for (uint i = pathToPlayer.Count() - 1; i > 0; --i)
		{
			next = { pathToPlayer[i].x, App->map->WorldToMap(position.x, round(position.y)).y + 3 };

			if (!App->pathfinding->IsWalkable(next))
			{
				result = next;
				break;
			}
		}
	}
	return result;
}

// ----------------------------------------------------------------------------------------
// All the logic when we are jumping or falling down
// ----------------------------------------------------------------------------------------
void j1EnemyLand::JumpFallLogic(float dt)
{
	//Checking if there is no collision, then apply gravity // 

	if (checkingFall)
	{
		current_animation = &jump;
		speed.y += (extraJumpForce + gravityForce) * (dt * DT_CALIBRATED);
	}


	// When it's jumping we apply some AXIS X force //

	if (flip == SDL_FLIP_NONE && !canJump)
		speed.x += (movementForce.x * 10) * dt;
	else if (flip == SDL_FLIP_HORIZONTAL && !canJump)
		speed.x -= (movementForce.x * 10) * dt;


	// We always get checkingFall to true, and COLL system put it to false if detect floor/platform COLL //

	checkingFall = true;
}


// ----------------------------------------------------------------------------------------
// After Update the position with speed, we use this function to check the limit we want
// ----------------------------------------------------------------------------------------
void j1EnemyLand::speedLimitChecker()
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


// ----------------------------------------------------------------------------------------
// All Collision logic system via directions to optimize and avoid creating more collisions
// ----------------------------------------------------------------------------------------
void j1EnemyLand::OnCollision(Collider* c1, Collider* c2) {

	// Improved coll system from the first assignment //
	// We only check DIR when we need it, with floor, platforms and enemies //


	// - - - - - - - COLLISIONS LOGIC - - - - - - - 

	int detectCollDir[DIR_MAX];
	detectCollDir[DIR_UP] = (c2->rect.y + c2->rect.h) - position.y;
	detectCollDir[DIR_DOWN] = (position.y + entity_collider.h) - c2->rect.y;
	detectCollDir[DIR_RIGHT] = (position.x + entity_collider.w) - c2->rect.x;
	detectCollDir[DIR_LEFT] = (c2->rect.x + c2->rect.w) - position.x;


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

		if ((c2->type == COLLIDER_PLAYER))
		{
			switch (dirCheck) {

			case DIR_UP:

				break;

			}
		}

		if ((c2->type == COLLIDER_STONE))
		{
			if (hurted == true) 
			{
				elim = true;
				App->audio->PlayFx(8, 0, App->audio->SpatialAudio(App->audio->FXvolume, distance));
				App->objects->particle->AddParticle(App->objects->particle->death, position.x, position.y, flip, COLLIDER_NONE);
				
			}
			else 
			{			
				hurtedConsec = true;
				hurted = true;
			}


			App->audio->PlayFx(11, 0, App->audio->SpatialAudio(App->audio->FXvolume, distance));
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
				checkingFall = false;

				if (!canJump) 
				{
					App->objects->particle->AddParticle(App->objects->particle->dustJumping, position.x, position.y + entity_collider.h, flip, COLLIDER_NONE);
					App->audio->PlayFx(5, 0, App->audio->SpatialAudio(App->audio->FXvolume, distance));
					speed.x = 0;
					canJump = true;
				}

				break;

			case DIR_LEFT:

				position.x = c2->rect.x + c2->rect.w + 1;

				current_animation = &idle;
				speed.x = 0;
				break;

			case DIR_RIGHT:

				position.x = c2->rect.x - entity_collider.w - 1;

				current_animation = &idle;
				speed.x = 0;
				break;

			case -1:
				break;
			}

		}

		if ((c2->type == COLLIDER_DEATH))
		{
			elim = true;
		}

		if (speed.y >= 0) {

			if ((c2->type == COLLIDER_PLATFORM))
			{
				switch (dirCheck) {

				case DIR_UP:
					break;

				case DIR_DOWN:
					position.y = c2->rect.y - entity_collider.h;

					checkingFall = false;

					if (!canJump)
					{
						App->objects->particle->AddParticle(App->objects->particle->dustJumping, position.x, position.y + entity_collider.h, flip, COLLIDER_NONE);
						App->audio->PlayFx(5, 0, App->audio->SpatialAudio(App->audio->FXvolume, distance));
						speed.x = 0;
						canJump = true;
					}

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

		CollisionPosUpdate();

	}
}