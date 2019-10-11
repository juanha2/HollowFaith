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

	ST_AT_FLOOR,
	ST_AT_AIR
};

enum player_inputs
{
	IN_UNKNOWN,

	IN_JUMPING,
	IN_JUMP_FINISH
};

class j1Player : public j1Module
{

public:

	Collider* colisionadores[MAXNUMOFCOLLIDERS];	
	SDL_Texture* graphics = nullptr;
	

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

	// Called when the player collides
	void OnCollision(Collider* c1, Collider* c2);

	// State machine
	player_states process_fsm(p2List<player_inputs>& inputs); 
	

private:

	// - - - - PLAYER - - - - 
	SDL_Rect playerTexture = { 0, 0, 50, 57 };
	SDL_RendererFlip playerFlip = SDL_RendererFlip::SDL_FLIP_NONE;

	fPoint playerPosition = { 0.0f, 700.0f }; //Player position on the world value
	fPoint playerSpeed = { 0.0f,0.0f }; // Player speed AXIS value
	float playerAcceleration = 0.0f;// Player acceleration AXIS value

	fPoint movementForce = { -90.0,-300.0f }; // Force applied to the movement in AXIS value

	void PlayerPosUpdate(float dt); //Update player's position
	// - - - - - - - - - - - 


	//  - - - - TIME - - - - 
	float previousTime = 0.0f;
	float frameToSecondValue = 0.0f;
	// - - - - - - - - - - - 


	//  - - - - SPEED - - - - 
	void speedLimitChecker();  // Player limit speed in AXIS
	fPoint speedLimit = { 130.0f, 250.0f }; // Player max speed value

	float slowlingValue = 1.25f;
	int slowlingLimitValue = 200;

	float gravityForce = 50.0f;// Player acceleration Y value

	void braking();  //Player smoothly slow
	// - - - - - - - - - - - 


	// - - - - INPUT - - - - 
	p2List<player_inputs> inputs; //Stores player related inputs
	player_states current_state = ST_UNKNOWN; //Intializing player status
	// - - - - - - - - - - - 


};

#endif // __j1PLAYER_H__
