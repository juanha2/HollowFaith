#ifndef __j1Player_H__
#define __j1Player_H__

#define JUMP_INIT_VY 11.4166666666 //initial velocity on the Y coord. //16.2
#define JUMP_INIT_AY 0.47568444444 //initial acceleration on the Y coord. //0.867

#define JUMP_TIME 3000
#define MAXNUMOFCOLLIDERS 1

#include "j1Module.h"

enum player_states
{
	ST_UNKNOWN,

	ST_IDLE,
	ST_AIR,
};

class j1Player : public j1Module
{

public:

	SDL_Rect r = { 0, 0, 50, 57 };
	SDL_Texture* graphics = nullptr;
	SDL_RendererFlip playerFlip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	player_states current_state = ST_UNKNOWN;

	iPoint position;

	Collider* colisionadores[MAXNUMOFCOLLIDERS];

	float currentTime;
	float gravity = -50.0f;

public:

	j1Player();

	// Destructor
	~j1Player();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);
	

private:
};


#endif// __j1PLAYER_H__
