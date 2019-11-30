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


j1Bonfire::j1Bonfire(fPoint pos, int count) : j1Entity(entityType::BONFIRE)
{
	position = pos;

	if (App->objects->bonfire[count] == nullptr)
		App->objects->bonfire[count] = this;

	num = count + 1;
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
	
	ignoreColl = false;
	alreadyCollided = false;

	return ret;
}

// Called before quitting
bool j1Bonfire::CleanUp()
{
	App->tex->UnLoad(texture);
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
	if (App->scene->checkpoint)
		distance = abs(App->objects->player->position.x - App->objects->bonfire[App->scene->num_checkpoint - 1]->position.x);

	Mix_Volume(3, App->audio->SpatialAudio(100, this->distance));
	

	return true;
}

bool j1Bonfire::PostUpdate() {

	Draw(App->dt);

	return true;
}


void j1Bonfire::OnCollision(Collider* c1, Collider* c2) {

	if ((c2->type == COLLIDER_PLAYER))
	{
		distance = abs(App->objects->player->position.x - App->objects->bonfire[c1->callback->num - 1]->position.x);

		if (!alreadyCollided)
		{
			App->scene->num_checkpoint = c1->callback->num;
			App->scene->checkpoint = true;

			App->audio->PlayFx(9, 0, App->audio->SpatialAudio(100, distance * 2));
			App->audio->PlayFx(8, 10, App->audio->SpatialAudio(200, distance * 2), 3);

			current_animation = &light_on;


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
