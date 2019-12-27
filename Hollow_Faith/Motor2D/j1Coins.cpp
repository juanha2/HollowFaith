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

}

j1Coins::~j1Coins() {
	
};

bool j1Coins::Awake(pugi::xml_node& config)
{
	bool ret = true;
	pugi::xml_node dataIterator = config.child("data");		

	// Loading all Animations
	pugi::xml_node animIterator = config.child("animations").child("animation");
	coin_animation.load_animation(animIterator, "coins");

	return ret;
}


// Called before the first frame
bool j1Coins::Start()
{
	bool ret = true;
	
	//Loading texture and collider
	
	texture = App->tex->Load("Assets/Sprites/coins.png");
	// Loading Texture and collider
	collider = new Collider({ 0,0,15,15 }, COLLIDER_COINS, this);
	App->coll->AddColliderEntity(collider);

	current_animation = &coin_animation;
	CollisionPosUpdate();
	return ret;
}

// Called before quitting
bool j1Coins::CleanUp()
{
	//Unloading data	
	App->tex->UnLoad(texture);
	texture = nullptr;
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

	if(texture!=nullptr)
		Draw(App->dt);

	return true;
}


void j1Coins::OnCollision(Collider* c1, Collider* c2) {
	
	if ((c2->type == COLLIDER_PLAYER))
	{		
		App->audio->PlayFx(12, 0,128);
		elim = true;		
		App->scene->num_coins++;			
		collider->to_delete = true;		
	}
}


// ----------------------------------------------------------------------------------
// Load Game State
// ----------------------------------------------------------------------------------
bool j1Coins::Load(pugi::xml_node& data)
{
	App->objects->CleanUp();
	return true;
}


// ----------------------------------------------------------------------------------
// Save Game State
// ----------------------------------------------------------------------------------
bool j1Coins::Save(pugi::xml_node& data) const
{
	pugi::xml_node coins = data.append_child("Coins");;
	coins.append_child("position").append_attribute("x") = position.x;
	coins.child("position").append_attribute("y") = position.y;

	return true;

}
