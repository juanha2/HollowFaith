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
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Scene.h"

j1EnemyFly::j1EnemyFly() : j1Enemy(entityType::ENEMY_FLY) {}

j1EnemyFly::j1EnemyFly(fPoint pos) : j1Enemy(entityType::ENEMY_FLY, pos) {}

j1EnemyFly::~j1EnemyFly() {};


bool j1EnemyFly::Awake(pugi::xml_node& config)
{
	bool ret = true;
	
	pugi::xml_node enemydata = config.child("enemydata");

	//Loading enemy Data
	texture_path = config.child("graphics_bat").attribute("path").as_string();
	pathCadency = enemydata.child("pathCadency").attribute("value").as_float();
	pathMinDist = enemydata.child("pathMinDist").attribute("value").as_int();
	agroDistance = enemydata.child("agroDistance").attribute("value").as_int();
	movementForce.x = enemydata.child("movementForce").attribute("x").as_float();
	movementForce.y = enemydata.child("movementForce").attribute("y").as_float();	
	gravityForce = config.child("data").child("gravity").attribute("value").as_float();

	// Loading all Animations
	pugi::xml_node animIterator = config.child("animations").child("animation");
	fly.load_animation(animIterator, "idleBat");

	// Loading collider
	entity_collider.x = enemydata.child("colliderFly").attribute("x").as_int();
	entity_collider.y = enemydata.child("colliderFly").attribute("y").as_int();
	entity_collider.w = enemydata.child("colliderFly").attribute("w").as_int();
	entity_collider.h = enemydata.child("colliderFly").attribute("h").as_int();

	return ret;
}

bool j1EnemyFly::Start()
{
	bool ret = true;

	// Loading Texture and collider
	texture = App->tex->Load(texture_path.GetString());	
	collider = new Collider(entity_collider, COLLIDER_ENEMY, this);
	App->coll->AddColliderEntity(collider);

	// Setting initial values
	current_animation = &fly;
	ignoreColl = false;

	originalPos.x = position.x;
	originalPos.y = position.y;

	CollisionPosUpdate();
	return ret;
}


bool j1EnemyFly::PreUpdate()
{
	bool ret = true;

	if (elim) // When enemy dies
	{
		App->audio->PlayFx(10, 0, App->audio->SpatialAudio(App->audio->FXvolume, distance));

		App->objects->DeleteEntity();
	}

	if (hurtedConsec) // Gets the first stone hit
	{
		timeConsec += App->dt;

		if (timeConsec <= 0.10f)
		{
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




	return ret;
}


bool j1EnemyFly::Update(float dt)
{
	BROFILER_CATEGORY("Update_FlyingEnemyPathLogic", Profiler::Color::DarkBlue);

	bool ret = true;

	GeneratingThePath(pathCadency, dt, agroDistance); // Generates a path with a X cadency, using the time and only when the distance between player and enemy is X

	if (chase)
	{
		if (!FollowingThePath(movementForce.x, dt)) // Follows the path logic generated
		{
			pathToPlayer.Clear();

			speed.x = 0;
			speed.y = 0;

			chase = false;
		}
	}

		// Update with dt the enemy position all frames
	PositionUpdate(App->dt);

	return ret;
}

bool j1EnemyFly::PostUpdate()
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

bool j1EnemyFly::CleanUp()
{
	//Unloading data
	App->tex->UnLoad(texture);	

	collider->to_delete = true;

	pathToPlayer.Clear();

	return true;
}

// ----------------------------------------------------------------------------------
// We generate the path from our Enemy position to the Player position
// ----------------------------------------------------------------------------------
bool j1EnemyFly::GeneratingThePath(float auxTimer, float dt, int auxAgroDistance)
{
	BROFILER_CATEGORY("FlyingEnemy_GENERATING_PathLogic", Profiler::Color::Blue);
	timer += dt;

	
	distance = abs(App->objects->player->position.x - position.x); // Always get the distance only in X axis

	if (distance < auxAgroDistance)
	{
		if (timer > auxTimer)
		{

			int hostileValueX, hostileValueY; // Value to get inside the Player and kill him, and not to follow him friendly 

			// 16 px are the Width and Height of each path tile (Not a magic number)
			if (position.x > App->objects->player->position.x)
				hostileValueX = -16;
			else
				hostileValueX = +16;

			if (position.y > App->objects->player->position.y)
				hostileValueY = -16;
			else
				hostileValueY = +16;


			pathToPlayer.Clear();
			App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->objects->player->position.x + hostileValueX, App->objects->player->position.y + hostileValueY));

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


// ----------------------------------------------------------------------------------
// We follow the generated path in X & Y Axis
// ----------------------------------------------------------------------------------
bool j1EnemyFly::FollowingThePath(float auxSpeed, float dt) {

	BROFILER_CATEGORY("FlyingEnemy_FOLLOWING_PathLogic", Profiler::Color::CadetBlue);

	iPoint current = App->map->MapToWorld(pathToPlayer[pathToPlayer.Count() - 1].x, pathToPlayer[pathToPlayer.Count() - 1].y);

	if (abs(position.x - current.x) > pathMinDist || abs(abs(position.y) - abs(current.y)) > pathMinDist) {

		if (current.x > position.x) // Go to the next tile
			speed.x += auxSpeed * dt;		
		else
			speed.x -= auxSpeed * dt;
				

		if (current.y < position.y)
			speed.y -= auxSpeed * dt;
		else
			speed.y += auxSpeed * dt;


		if (speed.x > 0)
			flip = SDL_FLIP_NONE;
		else
			flip = SDL_FLIP_HORIZONTAL;

	}
	else
	{
		pathToPlayer.Pop(current); // When we reach the next tile, pop it, and go to the next or stop if that was the last one.
	}

	return true;
}


// ----------------------------------------------------------------------------------------
// All Collision logic system via directions to optimize and avoid creating more collisions
// ----------------------------------------------------------------------------------------
void j1EnemyFly::OnCollision(Collider* c1, Collider* c2) {

	// Improved coll system from the first assignment //
	// We only check DIR when we need it, with floor, platforms and enemies //

	// - - - - - - - COLLISIONS LOGIC - - - - - - - 

	float detectCollDir[DIR_MAX];
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


	if ((c2->type == COLLIDER_STONE))
	{
		if (hurted == true)
		{
			elim = true;
			App->audio->PlayFx(8, 0, App->audio->SpatialAudio(App->audio->FXvolume, distance));
			App->objects->particle->AddParticle(App->objects->particle->death, position.x, position.y, flip, COLLIDER_NONE);
			collider->to_delete = true;

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

			position.y = c2->rect.y - entity_collider.h - 1;

			speed.y = 0;
			break;

		case DIR_LEFT:

			position.x = c2->rect.x + c2->rect.w + 1;

			speed.x = 0;	
			break;

		case DIR_RIGHT:

			position.x = c2->rect.x - entity_collider.w - 1;

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


	CollisionPosUpdate();



}