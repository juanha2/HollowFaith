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

	Collider*			colisionadores[MAXNUMOFCOLLIDERS];
	SDL_Texture*		graphics = nullptr;

	iPoint				startPosLevel1;
	iPoint				startPosLevel2;

	iPoint				savedPosition = { 0, 670 };

	iPoint				playerPosition = startPosLevel1;		//Player position on the world value
	bool				checkingFall = false;
	bool				ignoreColl = false;
	bool				godMode = false;
	bool				dead = false;
	bool				win = false;
	bool				can_climb = false;

	// - - - - INPUT - - - - 

	p2List<player_inputs>	 inputs;							//Stores player related inputs
	player_states			 current_state = ST_UNKNOWN;		//Intializing player status

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

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


private:

	// - - - - PLAYER - - - - 

	SDL_Rect			playerTexture = { 0, 0, 17, 27 };
	SDL_RendererFlip	playerFlip = SDL_RendererFlip::SDL_FLIP_NONE;

	float				playerClimbSpeed;
	fPoint				playerSpeed;							// Player speed AXIS value
	float				playerAcceleration;						// Player acceleration AXIS value
	fPoint				movementForce;							// Force applied to the movement in AXIS value

	float				hoverAcceleration;
	float				hoverSpeedActivation;
	float				hoverFallSmooth;

	float				particlePosMargin = 10.0f;

	void PlayerPositionUpdate(float dt);						//Update player's position

	// - - - - - - - - - - - 


	// - - - CAMERA PLAYER - - -

	float				cameraAcceleration = 0.0f;				// Camera acceleration AXIS value
	fPoint				cameraSpeed = { 0.0f,0.0f };			// Camera speed AXIS value
	fPoint				cameraSpeedLimit = { 200.0f, 250.0f };	// Camera max speed value

	int					startCameraFollowingPoint = 1325;
	float				cameraFollowingGuide = 0.0f;

	void cameraSpeedLimitChecker();
	void CameraPositionUpdate(float dt);
	void cameraBraking();

	// - - - - - - - - - - - 


	//  - - - - TIME - - - - 

	float				previousTime = 0.0f;
	float				frameToSecondValue = 0.0f;
	float				maxFrameToSecondValue = 0.16f;

	// - - - - - - - - - - - 


	//  - - - - SPEED - - - - 

	void				speedLimitChecker();					// Player limit speed in AXIS
	fPoint				speedLimit;								// Player max speed value

	float				slowingValue;
	int					slowlingLimitValue;
	float				gravityForce;							//Player acceleration Y value

	void braking();												//Player smoothly slow

	// - - - - - - - - - - - 


	//  - - - - ANIMATION - - - - 

	Animation*			current_animation;
	Animation			walk;
	Animation			idle;
	Animation			jump;
	Animation			climb;
	Animation			death;

	// - - - - - - - - - - - - - - 


	//  - - - - - - FX - - - - - - 

	p2SString			graphics_path;
	p2SString			jump_fx;
	p2SString			death_fx;
	p2SString			win1_Fx;
	p2SString			win2_Fx;

	// - - - - - - - - - - - - - - 

};

#endif // __j1PLAYER_H__
