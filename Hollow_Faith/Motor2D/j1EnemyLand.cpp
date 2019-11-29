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
	canFly = false;
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

	PositionUpdate(App->dt);


	return ret;
}


bool j1EnemyLand::Update(float dt)
{
	BROFILER_CATEGORY("EnemyPathLogic", Profiler::Color::DarkBlue);

	bool ret = true;
	
	JumpFallLogic(dt);

	GeneratingThePath(pathCadency, dt, agroDistance); // Generates a path with a X cadency, using the time and only when the distance between player and enemy is X

	if (chase)
	{	
		if(!FollowingThePath(enemySpeed)) // Follows the path logic generated
		{
			pathToPlayer.Clear();
			speed.x = 0;
			current_animation = &idle;
		
			chase = false;
		}
	}
	
	
	

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

	timer += dt;

	distance = abs(abs(App->objects->player->position.x) - abs(position.x));

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
			App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->objects->player->position.x + hostileValue, App->objects->player->position.y));

			for (uint i = 0; i < App->pathfinding->GetLastPath()->Count(); i++)
			{
				pathToPlayer.PushBack(*App->pathfinding->GetLastPath()->At(i));
			}

			pathToPlayer.Flip();

			chase = true;
			timer = 0;
		}
	}

	return true;
}

bool j1EnemyLand::FollowingThePath(float auxSpeed) {

	iPoint current = App->map->MapToWorld(pathToPlayer[pathToPlayer.Count() - 1].x, pathToPlayer[pathToPlayer.Count() - 1].y);

	if (abs(abs(position.x) - abs(current.x)) > 3) {

		if (current.x > position.x)
		{
			current_animation = &walk;
			speed.x += auxSpeed;
			flip = SDL_FLIP_NONE;
		}
		else
		{
			current_animation = &walk;
			speed.x -= auxSpeed;
			flip = SDL_FLIP_HORIZONTAL;
		}

		if (abs(abs(position.x) - abs(current.x)) < 3)
			speed.x = 1;

	}
	else
	{
		pathToPlayer.Pop(pathToPlayer[pathToPlayer.Count() - 1]);
	}


	if (pathToPlayer.Count() > 1)
		return true;
	else
		return false;

}

void j1EnemyLand::JumpFallLogic(float dt)
{

	//Checking if there is no collision, then apply gravity

	if (checkingFall)
	{
		// current_animation = &jump;
		speed.y += gravityForce * (dt * 51);
	}

	

	if (flip == SDL_FLIP_NONE && checkingFall)
		speed.x += 5;
	else if (flip == SDL_FLIP_HORIZONTAL && checkingFall)
		speed.x -= 5;


	checkingFall = true;
}