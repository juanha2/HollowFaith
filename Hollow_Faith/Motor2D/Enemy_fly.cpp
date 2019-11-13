#include "j1App.h"
#include "Enemy_fly.h"
#include "j1Collision.h"
#include "p2Animation.h"
Enemy_fly::Enemy_fly(int x, int y) : Enemy(x, y)
{
	fly.PushBack({ 3,7,21,25 });
	fly.PushBack({ 35, 6, 21, 26 });
	fly.PushBack({ 68, 5, 20, 27 });
	fly.PushBack({ 99, 7, 21, 25 });
	fly.PushBack({ 132, 6, 20, 26 });
	fly.PushBack({ 164, 5, 20, 27 });	
	fly.speed = 0.2f;

	animation = &fly;

	collider = App->coll->AddCollider({ position.x, position.y, 24, 24 }, COLLIDER_TYPE::COLLIDER_NONE, 0,0,0,0,(j1Module*)App->enemies);

}

void Enemy_fly::Move()
{

	//position.y = int(float(original_y) + (25.0f * sinf(wave)));
	position.x += 1;
}
