#ifndef __j1COINS_H__
#define __j1COINS_H__

#define JUMP_TIME 3000

#include "j1Module.h"
#include "p2Animation.h"
#include "j1Entity.h"

struct SDL_Texture;
struct Collider;

class j1Coins : public j1Entity
{

public:

	bool			alreadyCollided;
	Animation			coin_animation;

public:

	j1Coins();

	// Destructor
	~j1Coins();

	bool Awake(pugi::xml_node&);
	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node& file);
	bool Save(pugi::xml_node& file) const;

	void OnCollision(Collider* c1, Collider* c2);
};

#endif //  __j1BONFIRE_H__
