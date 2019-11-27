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
#include "j1EnemyFly.h"



j1Enemy::j1Enemy(j1Entity::entityType type) : j1Entity(type)
{
	if (App->objects->enemy == nullptr)
		App->objects->enemy = this;
}

j1Enemy::~j1Enemy() {};

// Load Enemies State
void j1Enemy::Load(pugi::xml_node& data)
{

}

// Save Enemies State
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