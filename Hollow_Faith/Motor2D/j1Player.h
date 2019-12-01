#ifndef __j1Player_H__
#define __j1Player_H__

#define JUMP_INIT_VY 11.4166666666 //initial velocity on the Y coord. //16.2
#define JUMP_INIT_AY 0.47568444444 //initial acceleration on the Y coord. //0.867

#define JUMP_TIME 3000


#include "j1Module.h"
#include "p2Animation.h"
#include "j1Entity.h"

struct SDL_Texture;

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


class j1Player : public j1Entity
{

public:	


	// - - - - PLAYER STATE - - - -

	fPoint				startPosLevel1;
	fPoint				startPosLevel2;
	
	bool				checkingFall = false;
	bool				dead = false;
	bool				win = false;
	bool				can_climb = false;

	// - - - - - - - - - - - 

	
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

	// State machine
	player_states process_fsm(p2List<player_inputs>& inputs);

	bool Load(pugi::xml_node& file);
	bool Save(pugi::xml_node& file) const;

	void OnCollision(Collider* c1, Collider* c2);


private:

	// - - - - PLAYER - - - - 	

	float				playerClimbSpeed;	
	fPoint				movementForce;							// Force applied to the movement in AXIS value

	float				hoverAcceleration;
	float				hoverSpeedActivation;
	float				hoverFallSmooth;

	float				particlePosMargin = 10.0f;	

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

	void braking();												//Player smoothly slow

	// - - - - - - - - - - - 


	//  - - - - ANIMATION - - - - 
		
	Animation			walk;
	Animation			idle;
	Animation			jump;
	Animation			climb;
	Animation			death;

	// - - - - - - - - - - - - - - 


};

#endif // __j1PLAYER_H__
