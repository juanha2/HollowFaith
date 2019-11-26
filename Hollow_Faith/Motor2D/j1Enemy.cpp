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
#include "j1Objects.h"
#include "j1Pathfinding.h"
#include "j1Enemy.h"
#include "j1Scene.h"


j1Enemy::j1Enemy() : j1Entity(entityType::ENEMY)
{
	if (App->objects->enemy == nullptr)
		App->objects->enemy = this;
}


j1Enemy::~j1Enemy() {};


bool j1Enemy::Awake(pugi::xml_node& config)
{
	LOG("Loading Enemy Parser");
	bool ret = true;

	texture_path = config.child("graphics_enemy").attribute("path").as_string();

	// Loading all Animations
	pugi::xml_node animIterator = config.child("animations").child("animation");
	animation.load_animation(animIterator, "idle");

	entity_collider = { 0, 0, 17, 27 };
	collider = new Collider(entity_collider, COLLIDER_ENEMY, this);

	originalPos = position;

	return ret;
}


bool j1Enemy::Start()
{
	bool ret = true;

	texture = App->tex->Load(texture_path.GetString());
	App->coll->AddColliderEntity(collider);
	current_animation = &animation;



	return ret;
}


bool j1Enemy::PreUpdate()
{
	bool ret = true;

	
		
	PositionUpdate(App->dt);


	return ret;
}


bool j1Enemy::Update(float dt)
{
	BROFILER_CATEGORY("EnemyPathLogic", Profiler::Color::DarkBlue);

	bool ret = true;
	timer += dt;


	if (abs(abs(App->objects->player->position.x) - abs(position.x)) < agroDistance)
	{
		if (timer > 5) 
		{

			pathToPlayer.Clear();
			App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->objects->player->position.x, App->objects->player->position.y));

			for (uint i = 0; i < App->pathfinding->GetLastPath()->Count(); i++)
			{
				pathToPlayer.PushBack(*App->pathfinding->GetLastPath()->At(i));
			}

			pathToPlayer.Flip();

			chase = true;
			timer = 0;
		}
	}	

	if (chase) 
	{
		if (pathToPlayer.Count() > 0) 
		{
			iPoint current = App->map->MapToWorld(pathToPlayer[pathToPlayer.Count() - 1].x, pathToPlayer[pathToPlayer.Count() - 1].y);

			if (abs(abs(position.x) - abs(current.x)) > 3 || abs(abs(position.y) - abs(current.y)) > 3){

				if (current.x > position.x)
					speed.x += 3;
				else
					speed.x -= 3;

				if (current.y < position.y)
					speed.y -= 3;
				else
					speed.y += 3;


				if (abs(abs(position.x) - abs(current.x)) < 3)
					speed.x = 0;

				if (abs(abs(position.y) - abs(current.y)) < 3)
					speed.y = 0;

			}
			else
			{
				speed.x = 0;
				speed.y = 0;
				
				pathToPlayer.Pop(pathToPlayer[pathToPlayer.Count() - 1]);
			}
			
		}
		else
		{		
			pathToPlayer.Clear();
			chase = false;
		}		
	}
	
	return ret;
}

bool j1Enemy::PostUpdate()
{
	bool ret = true;


	for (uint i = 0; i < pathToPlayer.Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(pathToPlayer[i].x, pathToPlayer[i].y);
		App->render->Blit(App->scene->debug_tex, pos.x, pos.y);
	}


	Draw(App->dt);
	return ret;
}

void j1Enemy::CleanUp()
{
	App->audio->UnLoad();
}

void j1Enemy::Load(pugi::xml_node& data)
{

}

// Save Game State
void j1Enemy::Save(pugi::xml_node& data) const
{

}
void j1Enemy::OnCollision(Collider* c1, Collider* c2) {


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

		if ((c2->type == COLLIDER_PLAYER))
		{
			switch (dirCheck) {

			case DIR_UP:
				
				break;

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