#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Bonfire.h"
#include "j1Scene.h"


j1Bonfire::j1Bonfire(fPoint pos) : j1Entity(entityType::BONFIRE)
{
	position = pos;

	if (App->objects->bonfire == nullptr)
		App->objects->bonfire = this;
}


j1Bonfire::~j1Bonfire() {};

bool j1Bonfire::Awake(pugi::xml_node& config)
{

	bool ret = true;

	texture_path = config.child("graphics_bonfire").attribute("path").as_string();

	// Loading all Animations
	pugi::xml_node animIterator = config.child("animations").child("animation");
	light_on.load_animation(animIterator, "light");
	light_off.load_animation(animIterator, "bonfire");

	// Loading all FX
	pugi::xml_node fxIterator = config.child("fx");
	fire_Fx = fxIterator.child("bonfireFx").attribute("path").as_string();
	fuse_Fx = fxIterator.child("fuseFx").attribute("path").as_string();

	entity_collider = { 0, 0, 29, 40 };
	collider = new Collider(entity_collider, COLLIDER_BONFIRE, this);

	return ret;
}


// Called before the first frame
bool j1Bonfire::Start()
{
	bool ret = true;

	texture = App->tex->Load(texture_path.GetString());
	App->coll->AddColliderEntity(collider);
	current_animation = &light_off;

	App->audio->LoadFx(fire_Fx.GetString());
	App->audio->LoadFx(fuse_Fx.GetString());
	
	ignoreColl = false;
	alreadyCollided = false;

	return ret;
}

// Called before quitting
bool j1Bonfire::CleanUp()
{

	App->audio->UnLoad();	

	return true;

}
// Called each loop iteration
bool j1Bonfire::PreUpdate()
{

	PositionUpdate(App->dt);

	return true;
}

// Called each loop iteration
bool j1Bonfire::Update(float dt)
{
	this->distance = abs(App->objects->player->position.x - this->position.x);

	LOG("%d", this->distance);

	Mix_Volume(3, App->audio->SpatialAudio(100, this->distance / 2));
	

	return true;
}

bool j1Bonfire::PostUpdate() {

	Draw(App->dt);

	return true;
}


void j1Bonfire::OnCollision(Collider* c1, Collider* c2) {

	if ((c2->type == COLLIDER_PLAYER))
	{
		if (!alreadyCollided)
		{
			App->scene->checkpoint = true;
			App->audio->PlayFx(13, 0, App->audio->SpatialAudio(100, distance * 2));
			
			fired = true;
			current_animation = &light_on;
			//this->collider->type = COLLIDER_NONE;
			alreadyCollided = true;
		}			
	}
}

// Load Game State
void j1Bonfire::Load(pugi::xml_node& data)
{

}

// Save Game State
void j1Bonfire::Save(pugi::xml_node& data) const
{
	

}
