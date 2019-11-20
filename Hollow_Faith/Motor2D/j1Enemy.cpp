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

	// Loading all Animations
	pugi::xml_node animIterator = config.child("animations").child("animation");
	animation.load_animation(animIterator, "idle");

	entity_collider = { 0, 0, 17, 27 };
	collider = new Collider(entity_collider, COLLIDER_ENEMY, this);

	return ret;
}

bool j1Enemy::Start()
{
	bool ret = true;

	App->coll->AddColliderEntity(collider);
	current_animation = &animation;

	position = { 80,320 };

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
	bool ret = true;

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

void j1Enemy::OnCollision(Collider* c1, Collider* c2)
{

}

void j1Enemy::Load(pugi::xml_node& data)
{

}

// Save Game State
void j1Enemy::Save(pugi::xml_node& data) const
{

}
