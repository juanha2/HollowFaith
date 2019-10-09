#ifndef __j1Player_H__
#define __j1Player_H__

#define JUMP_INIT_VY 11.4166666666 //initial velocity on the Y coord. //16.2
#define JUMP_INIT_AY 0.47568444444 //initial acceleration on the Y coord. //0.867

#define JUMP_TIME 3000

#include "j1Module.h"

class j1Player : public j1Module
{

public:
	SDL_Texture* graphics = nullptr;

	iPoint position;

	bool playerFlip = false;


public:

	j1Player();

	// Destructor
	 ~j1Player();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update();

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
};


#endif// __j1PLAYER_H__
