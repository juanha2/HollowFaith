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


	position = { 80,320 };

	return ret;
}

bool j1Enemy::PreUpdate()
{
	bool ret = true;

	//DEBUG MOVEMENT ENEMY

	
		
	/*if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT ) { // Pressing A (Running)
			position.x += 1;
			flip = SDL_FLIP_NONE;
		}

		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) { // Pressing A (Running)
			position.x -= 1;
			flip = SDL_FLIP_HORIZONTAL;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) { // Pressing A (Running)
			position.y -= 1;			
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) { // Pressing A (Running)
			position.y += 1;			
		}*/
		
	PositionUpdate(App->dt);


	return ret;
}


bool j1Enemy::Update(float dt)
{
	bool ret = true;

	timer += dt;


	if (abs(abs(App->objects->player->position.x) - abs(position.x)) < agroDistance)
	{
		if (timer > 5) 
		{
			chase = true;
			App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->objects->player->position.x, App->objects->player->position.y));
			pathToPlayer = *App->pathfinding->GetLastPath();	
			
			timer = 0;
		}
	}	

	if (chase) 
	{
		if (pathToPlayer.Count() > 0) 
		{
			iPoint current = App->map->MapToWorld(pathToPlayer.At(0)->x, pathToPlayer.At(0)->y);
			position.x = current.x + originalPos.x;
			pathToPlayer.Pop(current);
		
		}
		else
		{		
			chase = false;
		}		
	}
	
	
	

	return ret;
}

bool j1Enemy::PostUpdate()
{
	bool ret = true;
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

				position.x = c2->rect.x - entity_collider.w;
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