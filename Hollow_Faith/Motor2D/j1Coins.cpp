#include "j1App.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Bonfire.h"
#include "j1Coins.h"
#include "j1Render.h"
#include "j1Scene.h"

j1Coins::j1Coins() : j1Entity(entityType::COINS)
{
	if (App->objects->coins == nullptr)
		App->objects->coins = this;
}

j1Coins::~j1Coins() {
	App->objects->coins = nullptr;
};

bool j1Coins::Awake(pugi::xml_node& config)
{
	bool ret = true;
	pugi::xml_node dataIterator = config.child("data");	
	collider = new Collider({ 0,0,15,15 }, COLLIDER_COINS, this);
	App->coll->AddColliderEntity(collider);

	return ret;
}


// Called before the first frame
bool j1Coins::Start()
{
	bool ret = true;
	//Loading texture and collider
	CollisionPosUpdate();
	texture = App->tex->Load("Assets/Sprites/coins.png");
	return ret;
}

// Called before quitting
bool j1Coins::CleanUp()
{
	//Unloading data	
	texture = nullptr;
	App->tex->UnLoad(texture);

	return true;
}

// Called each loop iteration
bool j1Coins::PreUpdate() {

	if (elim) 			
		App->objects->DeleteEntity();
	
	return true;
}

// Called each loop iteration
bool j1Coins::Update(float dt)
{	
	PositionUpdate(dt);
	return true;
}

bool j1Coins::PostUpdate() {

	SDL_Rect rect = { 0,0,15,15 };
	App->render->Blit(texture, position.x, position.y, &rect, 1.0, 1.0, flip, NULL, entity_collider.w / 2);

	return true;
}


void j1Coins::OnCollision(Collider* c1, Collider* c2) {
	
	if ((c2->type == COLLIDER_PLAYER))
	{
		if (!alreadyCollided)
		{
			elim = true;
			alreadyCollided = true;
			App->scene->num_coins++;			
			collider->to_delete = true;
		}
	}
}


// ----------------------------------------------------------------------------------
// Load Game State
// ----------------------------------------------------------------------------------
bool j1Coins::Load(pugi::xml_node& data)
{
	
	return true;
}


// ----------------------------------------------------------------------------------
// Save Game State
// ----------------------------------------------------------------------------------
bool j1Coins::Save(pugi::xml_node& data) const
{
	
	return true;

}
