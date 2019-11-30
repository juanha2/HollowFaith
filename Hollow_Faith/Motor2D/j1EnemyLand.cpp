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
#include "j1Enemy.h"
#include "j1Scene.h"
#include "j1EnemyLand.h"

j1EnemyLand::j1EnemyLand() : j1Enemy(entityType::ENEMY_LAND) {}

j1EnemyLand::j1EnemyLand(fPoint pos) : j1Enemy(entityType::ENEMY_LAND, pos) {  }

j1EnemyLand::~j1EnemyLand() {};


bool j1EnemyLand::Awake(pugi::xml_node& config)
{
	LOG("Loading Enemy Parser");
	bool ret = true;

	texture_path = config.child("graphics_enemy").attribute("path").as_string();

	// Loading all Animations
	pugi::xml_node animIterator = config.child("animations").child("animation");
	idle.load_animation(animIterator, "idleLand");
	walk.load_animation(animIterator, "walkLand");

	// Loading all FX
	pugi::xml_node fxIterator = config.child("fx");
	death = fxIterator.child("death_enemyFx").attribute("path").as_string();

	entity_collider = { 0, 0, 17, 27 };
	collider = new Collider(entity_collider, COLLIDER_ENEMY, this);

	canJump = true;
	originalPos = position;

	return ret;
}


bool j1EnemyLand::Start()
{
	bool ret = true;

	texture = App->tex->Load(texture_path.GetString());
	App->coll->AddColliderEntity(collider);

	current_animation = &idle;
	App->audio->LoadFx(death.GetString());

	
	ignoreColl = false;

	return ret;
}


bool j1EnemyLand::PreUpdate()
{
	bool ret = true;

	if (elim) // When enemy dies
	{

		App->audio->PlayFx(8, 0, App->audio->SpatialAudio(App->audio->FXvolume, distance));
		App->objects->particle->AddParticle(App->objects->particle->death, position.x, position.y, flip, COLLIDER_NONE);
		collider->to_delete = true;

		pathToPlayer.Clear();
		App->objects->DeleteEntity();
	}

	if (hurtedConsec) 
	{
		timeConsec += App->dt;

		if (timeConsec <= 0.10f)
		{
			SDL_SetTextureColorMod(texture, 255, 0, 0);
			speed.y = movementForce.y / 3;

			if (flip == SDL_FLIP_NONE)
				speed.x = -movementForce.y;
			else
				speed.x = movementForce.y;
	
		}
		else 
		{
			SDL_SetTextureColorMod(texture, 255, 255, 255);
			speed.x = 0;
			hurtedConsec = false;
		}
		
	}



	PositionUpdate(App->dt);


	return ret;
}


bool j1EnemyLand::Update(float dt)
{
	BROFILER_CATEGORY("ALL_LandEnemyPathLogic", Profiler::Color::DarkGreen);

	bool ret = true;


	GeneratingThePath(pathCadency, dt, agroDistance); // Generates a path with a X cadency, using the time and only when the distance between player and enemy is X

	if (chase)
	{	
		if(!FollowingThePath(movementForce.x, dt)) // Follows the path logic generated
		{
			pathToPlayer.Clear();
			speed.x = 0;
			current_animation = &idle;
		
			chase = false;
		}
	}
	else 
	{
		// Apatrullando la ciudad
	}
	
	
	JumpFallLogic(dt);

	return ret;
}

bool j1EnemyLand::PostUpdate()
{
	bool ret = true;

	if (App->scene->debug)
	{
		for (uint i = 0; i < pathToPlayer.Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(pathToPlayer[i].x, pathToPlayer[i].y);
			App->render->Blit(App->scene->debug_tex, pos.x, pos.y);
		}
	}

	Draw(App->dt);

	return ret;
}

bool j1EnemyLand::CleanUp()
{
	return true;
}


bool j1EnemyLand::GeneratingThePath(float auxTimer, float dt, int auxAgroDistance)
{
	BROFILER_CATEGORY("LandEnemy_GENERATING_PathLogic", Profiler::Color::ForestGreen);

	timer += dt;

	distance = abs(App->objects->player->position.x - position.x);

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

bool j1EnemyLand::FollowingThePath(float auxSpeed, float dt) {

	BROFILER_CATEGORY("LandEnemy_FOLLOWING_PathLogic", Profiler::Color::Green);
	
	iPoint current = App->map->MapToWorld(pathToPlayer[pathToPlayer.Count() - 1].x, pathToPlayer[pathToPlayer.Count() - 1].y);

	if (abs(position.x - current.x) > pathMinDist) {

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
		pathToPlayer.Pop(current);
	}


	if (pathToPlayer.Count() > 1)
	{
		iPoint next = { pathToPlayer[pathToPlayer.Count() - 2].x, pathToPlayer[pathToPlayer.Count() - 2].y + 2 };

		if (App->pathfinding->IsWalkable(next))
		{

			bool jumpStated = false;

			for (uint i = pathToPlayer.Count() - 2; i >= 0; --i)
			{

				next = { pathToPlayer[i].x, pathToPlayer[i].y + 2 };

				if (!App->pathfinding->IsWalkable(next))
				{
					jumpStated = true;
					break;
				}
			}

			if (jumpStated)
			{

				if (distance < jumpDistance)
				{
					if (canJump)
					{
						speed.y = movementForce.y;
						
						if (flip == SDL_FLIP_NONE)
							speed.x += 200;
						else
							speed.x -= 200;

						canJump = false;
					}
				}
				else
				{
					speed.x = 0;
					current_animation = &idle;
				}

			}

		}

	}

	if (pathToPlayer.Count() <= 0) 
		return false;
	else
		return true;
	
}

void j1EnemyLand::JumpFallLogic(float dt)
{

	//Checking if there is no collision, then apply gravity

	if (checkingFall)
	{
		// current_animation = &jump;
		speed.y += gravityForce * (dt * DT_CALIBRATED);
	}


	if (flip == SDL_FLIP_NONE && checkingFall)
		speed.x += movementForce.x * dt;
	else if (flip == SDL_FLIP_HORIZONTAL && checkingFall)
		speed.x -= movementForce.x * dt;


	
	checkingFall = true;
}


void j1EnemyLand::OnCollision(Collider* c1, Collider* c2) {


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
					canJump = true;
				}

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

		if (speed.y >= 0) {

			if ((c2->type == COLLIDER_PLATFORM))
			{
				switch (dirCheck) {

				case DIR_UP:
					break;

				case DIR_DOWN:
					position.y = c2->rect.y - entity_collider.h;
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