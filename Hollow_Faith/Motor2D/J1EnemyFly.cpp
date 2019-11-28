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
#include "j1EnemyFly.h"
#include "j1Player.h"

j1EnemyFly::j1EnemyFly() : j1Enemy(entityType::ENEMY_FLY)
{
	/*if (App->objects->enemy == nullptr)
		App->objects->enemy = this;*/
}

j1EnemyFly::j1EnemyFly(iPoint pos) : j1Enemy(entityType::ENEMY_FLY, pos)
{
	position = pos;
}


j1EnemyFly::~j1EnemyFly() {};


bool j1EnemyFly::Awake(pugi::xml_node& config)
{
	LOG("Loading Enemy Parser");
	bool ret = true;

	texture_path = config.child("graphics_bat").attribute("path").as_string();

	// Loading all Animations
	pugi::xml_node animIterator = config.child("animations").child("animation");
	animation.load_animation(animIterator, "idleBat");

	// Loading all FX
	pugi::xml_node fxIterator = config.child("fx");
	death = fxIterator.child("death_enemyFx").attribute("path").as_string();

	entity_collider = { 0, 0, 16, 16 };
	collider = new Collider(entity_collider, COLLIDER_ENEMY, this);

	originalPos = position;

	return ret;
}


bool j1EnemyFly::Start()
{
	bool ret = true;

	texture = App->tex->Load(texture_path.GetString());
	App->coll->AddColliderEntity(collider);
	current_animation = &animation;
	App->audio->LoadFx(death.GetString());

	checkingFall = true;
	ignoreColl = false;
	return ret;
}


bool j1EnemyFly::PreUpdate()
{
	bool ret = true;

	if (elim)
	{
		App->audio->PlayFx(8, 0, App->audio->FXvolume);
		App->objects->particle->AddParticle(App->objects->particle->death, position.x, position.y, flip, COLLIDER_NONE);
		collider->to_delete = true;

		pathToPlayer.Clear();
		App->objects->DeleteEntity();
	}

	
	PositionUpdate(App->dt);


	return ret;
}


bool j1EnemyFly::Update(float dt)
{
	BROFILER_CATEGORY("EnemyPathLogic", Profiler::Color::DarkBlue);

	bool ret = true;

	GeneratingThePath(pathCadency, dt, agroDistance); // Generates a path with a X cadency, using the time and only when the distance between player and enemy is X

	if (chase)
	{
		if (!FollowingThePath(enemySpeed)) // Follows the path logic generated
		{
			pathToPlayer.Clear();

			speed.x = 0;
			speed.y = 0;

			chase = false;		
		}
	}

	
	return ret;
}

bool j1EnemyFly::PostUpdate()
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

	//Draw(App->dt);
	App->render->Blit(texture, position.x, position.y,
		&current_animation->GetCurrentFrame(App->dt), 1.0, 1.0, flip, NULL, entity_collider.w / 2+10,10);
	return ret;
}

void j1EnemyFly::CleanUp()
{
	
}


bool j1EnemyFly::GeneratingThePath(float auxTimer, float dt, int auxAgroDistance)
{

	timer += dt;


	if (abs(abs(App->objects->player->position.x) - abs(position.x)) < auxAgroDistance)
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

			pathToPlayer.Flip();

			chase = true;
			timer = 0;
		}
	}

	return true;
}

bool j1EnemyFly::FollowingThePath(float auxSpeed) {

	iPoint current = App->map->MapToWorld(pathToPlayer[pathToPlayer.Count() - 1].x, pathToPlayer[pathToPlayer.Count() - 1].y);

	if (abs(abs(position.x) - abs(current.x)) > 3 || abs(abs(position.y) - abs(current.y)) > 3) {

		if (current.x > position.x)
		{
			speed.x += 3;
			flip = SDL_FLIP_NONE;
		}
		else
		{
			speed.x -= 3;
			flip = SDL_FLIP_HORIZONTAL;
		}

		if (current.y < position.y)
			speed.y -= 3;
		else
			speed.y += 3;


		if (abs(abs(position.x) - abs(current.x)) < 3)
			speed.x = 1;

		if (abs(abs(position.y) - abs(current.y)) < 3)
			speed.y = 1;

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