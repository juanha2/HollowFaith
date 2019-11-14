#include "j1App.h"
#include "Enemy_fly.h"
#include "j1Collision.h"
#include "p2Animation.h"
#include "j1Pathfinding.h"
#include "j1Audio.h"

Enemy_fly::Enemy_fly(int x, int y) : Enemy(x, y)
{

	fly.PushBack({ 3,39,21,25 });
	fly.PushBack({ 35, 38, 21, 26 });
	fly.PushBack({ 68, 37, 20, 27 });
	fly.PushBack({ 99, 39, 21, 25 });
	fly.PushBack({ 132, 38, 20, 26 });
	fly.PushBack({ 164, 37, 20, 27 });	
	fly.speed = 0.08f;

	animation = &fly;

	collider = App->coll->AddCollider({ position.x, position.y, 24, 24 }, COLLIDER_TYPE::COLLIDER_ENEMY, 0,0,0,0,(j1Module*)App->enemies);

}

void Enemy_fly::Move()
{	
	//App->pathfinding->CreatePath({ 80,350 }, { 100,350 });
	//position.x += 1;
	
}


