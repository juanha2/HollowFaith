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

	ST_CLIMB,	
	ST_CLIMB_IDLE,

	ST_DEAD,
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

	IN_UPWARDS_UP,
	IN_CLIMB,
	IN_DOWNWARDS_UP,

	IN_LEFT_UP,
	IN_LEFT_DOWN,

	IN_DEAD,
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

	iPoint startPosLevel1 = { 0, 340 };
	iPoint startPosLevel2 = { 0, 340 };

	iPoint savedPosition = { 0, 670 };

	iPoint playerPosition = startPosLevel1; //Player position on the world value
	bool checkingFall = false;
	bool ignoreColl = false;
	bool godMode = false;

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
	
	bool dead = false;
private:

	// - - - - PLAYER - - - - 
	SDL_Rect playerTexture = { 0, 0, 17, 27 };
	SDL_RendererFlip playerFlip = SDL_RendererFlip::SDL_FLIP_NONE;
	
	fPoint playerSpeed = { 0.0f,0.0f }; // Player speed AXIS value
	float playerAcceleration = 0.0f;// Player acceleration AXIS value
	float hoverAcceleration = 0.0f;
	fPoint movementForce = { -20.0f ,-420.0f }; // Force applied to the movement in AXIS value

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
	fPoint speedLimit = { 250.0f, 750.0f }; // Player max speed value

	float slowlingValue = 10.0f;
	int slowlingLimitValue = 200;

	float gravityForce = 30.0f;// Player acceleration Y value

	void braking();  //Player smoothly slow
	// - - - - - - - - - - - 

	Animation* current_animation;
	Animation walk;
	Animation idle;
	Animation jump;
	Animation climb;
	Animation death;
	SDL_Rect	climb_idle;
	

	p2SString jump_fx;

	bool can_climb = false;
	bool win = false;
};

#endif // __j1PLAYER_H__
