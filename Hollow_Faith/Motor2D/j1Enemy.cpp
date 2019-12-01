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
#include "j1Checkpoint.h"


j1Enemy::j1Enemy(j1Entity::entityType type) : j1Entity(type)
{
	if (App->objects->enemy == nullptr)
		App->objects->enemy = this;
}

j1Enemy::j1Enemy(j1Entity::entityType type, fPoint pos) : j1Entity(type)
{
	position = pos;	
}

j1Enemy::~j1Enemy() {};

// Load Enemies State
bool j1Enemy::Load(pugi::xml_node& data)
{
	chase = false;
	speed = { 0,0 };
	elim = true;
	App->objects->DeleteEntities();
	return true;
}

// Save Enemies State
bool j1Enemy::Save(pugi::xml_node& data) const
{		
	
	if (type == ENEMY_FLY)
	{
		if (!App->checkpoint->save_checkpoints) {
			pugi::xml_node fly_enemy = data.append_child("EnemyFly");;
			fly_enemy.append_child("position").append_attribute("x") = position.x;
			fly_enemy.child("position").append_attribute("y") = position.y;
		}
		else
		{
			pugi::xml_node fly_enemy = data.append_child("EnemyFly");;
			fly_enemy.append_child("position").append_attribute("x") = originalPos.x;
			fly_enemy.child("position").append_attribute("y") = originalPos.y;
		}
		
	}
	else if (type == ENEMY_LAND)
	{
		if (!App->checkpoint->save_checkpoints) {
			pugi::xml_node land_enemy = data.append_child("EnemyLand");;
			land_enemy.append_child("position").append_attribute("x") = position.x;
			land_enemy.child("position").append_attribute("y") = position.y;
		}
		else {
			pugi::xml_node land_enemy = data.append_child("EnemyLand");;
			land_enemy.append_child("position").append_attribute("x") = originalPos.x;
			land_enemy.child("position").append_attribute("y") = originalPos.y;
		}
	}

	return true;
}


