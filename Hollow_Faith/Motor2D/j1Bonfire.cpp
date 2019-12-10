#include "j1App.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Bonfire.h"
#include "j1Scene.h"
#include "j1Checkpoint.h"


j1Bonfire::j1Bonfire(fPoint pos, int count, bool actv) : j1Entity(entityType::BONFIRE)
{	
	if (App->objects->bonfire[count] == nullptr)
		App->objects->bonfire[count] = this;

	//We set initial values
	num_bonfire = count + 1;
	active = actv;

}

j1Bonfire::~j1Bonfire() {
	App->objects->bonfire[num_bonfire -1 ] = nullptr;
};

bool j1Bonfire::Awake(pugi::xml_node& config)
{
	bool ret = true;
	pugi::xml_node dataIterator = config.child("data");

	//Loading texture
	texture_path = config.child("graphics_bonfire").attribute("path").as_string();	

	// Loading all Animations
	pugi::xml_node animIterator = config.child("animations").child("animation");
	light_on.load_animation(animIterator, "light");
	light_off.load_animation(animIterator, "bonfire");	

	// Loading collider
	entity_collider.x = dataIterator.child("collider_bonfire").attribute("x").as_int();
	entity_collider.y = dataIterator.child("collider_bonfire").attribute("y").as_int();
	entity_collider.w = dataIterator.child("collider_bonfire").attribute("w").as_int();
	entity_collider.h = dataIterator.child("collider_bonfire").attribute("h").as_int();	

	return ret;
}


// Called before the first frame
bool j1Bonfire::Start()
{
	bool ret = true;

	//Loading texture and collider
	texture = App->tex->Load(texture_path.GetString());
	collider = new Collider(entity_collider, COLLIDER_BONFIRE, this);
	App->coll->AddColliderEntity(collider);

	//Setting initial values
	current_animation = &light_off;	
	ignoreColl = false;
	alreadyCollided = false;

	return ret;
}

// Called before quitting
bool j1Bonfire::CleanUp()
{
	//Unloading data
	App->tex->UnLoad(texture);

	return true;

}
// Called each loop iteration
bool j1Bonfire::PreUpdate(){


	return true;
}

// Called each loop iteration
bool j1Bonfire::Update(float dt)
{
	//If bonfire is active, we set its animation to on and apply spatial audio
	if (active) 
	{
		current_animation = &light_on;		

		distance = abs(App->objects->player->position.x - App->checkpoint->checkpointpos.x);
		Mix_Volume(1, App->audio->SpatialAudio(100, distance));
	}

	PositionUpdate(dt);

	return true;
}

bool j1Bonfire::PostUpdate() {

	Draw(App->dt); 

	return true;
}


void j1Bonfire::OnCollision(Collider* c1, Collider* c2) {

	// Saving bonfire data when collides with player

	if ((c2->type == COLLIDER_PLAYER))
	{		
		App->checkpoint->checkpointpos = position;  

		if (!alreadyCollided)
		{		
			App->audio->PlayFx(9, 0, App->audio->SpatialAudio(100, distance * 2));
			App->audio->PlayFx(8, 10, App->audio->SpatialAudio(200, distance * 2), 1);

			App->checkpoint->checkpoint = true;
			App->checkpoint->num_checkpoint = c1->callback->num_bonfire;					
			
			active = true;
			App->checkpoint->SaveCheckPoints();
			alreadyCollided = true;
		}
	}
}


// ----------------------------------------------------------------------------------
// Load Game State
// ----------------------------------------------------------------------------------
bool j1Bonfire::Load(pugi::xml_node& data)
{
	App->checkpoint->num_checkpoint = data.attribute("num").as_int();

	// Checkpoints Logic ----------------------------------------

	alreadyCollided = false;

	for (int i = 0; i < MAX_BONFIRES; i++) {

		if (App->checkpoint->num_checkpoint == i)
		{
			if (i == 0)
				App->checkpoint->checkpoint = false;
			else
			{
				App->checkpoint->checkpointpos = position;
				App->checkpoint->num_checkpoint = i;
			}			
		}			
	}	
	
	return true;
}


// ----------------------------------------------------------------------------------
// Save Game State
// ----------------------------------------------------------------------------------
bool j1Bonfire::Save(pugi::xml_node& data) const
{
	pugi::xml_node bonfire = data.append_child("Bonfire");	

	bonfire.append_child("position").append_attribute("x") = position.x;
	bonfire.child("position").append_attribute("y") = position.y;
	bonfire.append_child("active").append_attribute("value") = active;

	if(App->checkpoint->save_checkpoints)
		bonfire.append_attribute("num") = num_bonfire;
	else
		bonfire.append_attribute("num") = App->checkpoint->num_checkpoint;

	return true;

}
