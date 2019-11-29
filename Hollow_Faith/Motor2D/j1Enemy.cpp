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


