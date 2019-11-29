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



j1Enemy::j1Enemy(j1Entity::entityType type) : j1Entity(type)
{

}

j1Enemy::j1Enemy(j1Entity::entityType type, iPoint pos) : j1Entity(type)
{
	position = pos;	
}

j1Enemy::~j1Enemy() {};


// Load Enemies State
void j1Enemy::Load(pugi::xml_node& data)
{
	chase = false;
	speed = { 0,0 };

	if (App->scene->different_map) {

		App->scene->ready_to_load = true;
		ignoreColl = true;
		savedPosition.x = data.child("position").attribute("x").as_int();
		savedPosition.y = data.child("position").attribute("y").as_int();
	}

	else if (!App->scene->different_map) {

		position.x = data.child("position").attribute("x").as_int();
		position.y = data.child("position").attribute("y").as_int();		
	}
}

// Save Enemies State
void j1Enemy::Save(pugi::xml_node& data) const
{	

	if (type == ENEMY_FLY)
	{
		pugi::xml_node fly_enemy = data.append_child("EnemyFly");;
		fly_enemy.append_child("position").append_attribute("x") = position.x;
		fly_enemy.child("position").append_attribute("y") = position.y;
	}
	else if (type == ENEMY_LAND)
	{
		pugi::xml_node land_enemy = data.append_child("EnemyLand");;
		land_enemy.append_child("position").append_attribute("x") = position.x;
		land_enemy.child("position").append_attribute("y") = position.y;
	}
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

		if ((c2->type == COLLIDER_STONE))
		{
			elim = true;

			App->audio->PlayFx(8, 0, App->audio->FXvolume);
			App->objects->particle->AddParticle(App->objects->particle->death, position.x, position.y, flip, COLLIDER_NONE);
			collider->to_delete = true;
			
			App->objects->DeleteEntity();			
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
				canJump = true;
				break;

			case DIR_LEFT:

				position.x = c2->rect.x + c2->rect.w + 1;

				if (canJump && !canFly) {
					App->audio->PlayFx(1, 0, App->audio->SpatialAudio(App->audio->FXvolume, distance));
					App->objects->particle->AddParticle(App->objects->particle->dustJumping, position.x, position.y + entity_collider.h, flip, COLLIDER_NONE);
					speed.y = -420.0f;
					canJump = false;
				}

				speed.x = 0;
				break;

			case DIR_RIGHT:

				position.x = c2->rect.x - entity_collider.w - 1;

				if (canJump && !canFly) {
					App->audio->PlayFx(1, 0, App->audio->SpatialAudio(App->audio->FXvolume, distance));
					App->objects->particle->AddParticle(App->objects->particle->dustJumping, position.x, position.y + entity_collider.h, flip, COLLIDER_NONE);
					speed.y = -420.0f;
					canJump = false;
				}

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

