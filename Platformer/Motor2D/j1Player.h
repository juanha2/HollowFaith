#ifndef __j1Player_H__
#define __j1Player_H__

#define JUMP_INIT_VY 11.4166666666 //initial velocity on the Y coord. //16.2
#define JUMP_INIT_AY 0.47568444444 //initial acceleration on the Y coord. //0.867

#define JUMP_TIME 3000
#define MAXNUMOFCOLLIDERS 1

#include "j1Module.h"
#include "p2Animation.h"

enum player_states
{
	ST_UNKNOWN = -1,

	ST_IDLE,
	ST_AT_AIR,
	ST_WALK_RIGHT,
	ST_WALK_LEFT,
};


enum player_inputs
{
	IN_UNKNOWN = -1,

	IN_JUMPING,
	IN_JUMP_FINISH,
	IN_FALLING,
	IN_WALK_RIGHT,
	IN_WALK_LEFT,
	
	IN_RIGHT_UP,
	IN_RIGHT_DOWN,

	IN_LEFT_UP,
	IN_LEFT_DOWN,
};


enum collDirCheck
{
	DIR_UNKNOWN = -1,

	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
	DIR_MAX
};

class j1Player : public j1Module
{

public:

	Collider* colisionadores[MAXNUMOFCOLLIDERS];	
	SDL_Texture* graphics = nullptr;
	SDL_Texture* graphics1 = nullptr;	

	iPoint playerPosition = {0, 670 }; //Player position on the world value
	bool checkingFall = false;

	// - - - - INPUT - - - - 
	p2List<player_inputs> inputs; //Stores player related inputs
	player_states current_state = ST_UNKNOWN; //Intializing player status
	// - - - - - - - - - - - 


public:

	j1Player();
	// Destructor
	~j1Player();

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

	// Called when the player collides
	void OnCollision(Collider* c1, Collider* c2);

	// State machine
	player_states process_fsm(p2List<player_inputs>& inputs); 
	

private:

	// - - - - PLAYER - - - - 
	SDL_Rect playerTexture = { 0, 0, 17, 27 };
	SDL_RendererFlip playerFlip = SDL_RendererFlip::SDL_FLIP_NONE;
	
	fPoint playerSpeed = { 0.0f,0.0f }; // Player speed AXIS value
	float playerAcceleration = 0.0f;// Player acceleration AXIS value

	fPoint movementForce = { -20.0f, - 95.0f }; // Force applied to the movement in AXIS value

	void PlayerPositionUpdate(float dt); //Update player's position
	// - - - - - - - - - - - 

	// - - CAMERA PLAYER - - 
	float cameraAcceleration = 0.0f; // Camera acceleration AXIS value
	fPoint cameraSpeed = { 0.0f,0.0f }; // Camera speed AXIS value
	fPoint cameraSpeedLimit = { 200.0f, 250.0f }; // Camera max speed value

	float cameraFollowingPoint = 340.0f;
	float cameraFollowingGuide = 0.0f;

	void cameraSpeedLimitChecker();
	void CameraPositionUpdate(float dt);

	void cameraBraking();
	// - - - - - - - - - - - 

	//  - - - - TIME - - - - 
	float previousTime = 0.0f;
	float frameToSecondValue = 0.0f;
	// - - - - - - - - - - - 


	//  - - - - SPEED - - - - 
	void speedLimitChecker();  // Player limit speed in AXIS
	fPoint speedLimit = { 60.0f,300.0f }; // Player max speed value

	float slowlingValue = 1.2f;
	int slowlingLimitValue = 200;

	float gravityForce = 2.0f;// Player acceleration Y value
	float minGravityConst = 5.0f;

	void braking();  //Player smoothly slow
	// - - - - - - - - - - - 

	Animation* current_animation;
	Animation walk;
	Animation idle;
	Animation jump;

	p2SString jump_fx;
};

#endif // __j1PLAYER_H__
