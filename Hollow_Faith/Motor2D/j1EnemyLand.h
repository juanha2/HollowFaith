#ifndef __j1ENEMYLAND_H__
#define __j1ENEMYLAND_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "j1Module.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "p2Point.h"
#include "j1Collision.h"

struct SDL_Texture;
struct Collider;

class j1EnemyLand : public j1Enemy
{

public:
	// - - - - ANIMATIONS - - - -

	Animation			idle;
	Animation			walk;
	Animation			jump;

	// - - - - TRIGGERS - - - -
	
	bool				canJump = false;
	bool				checkingFall = false;

	int					jumpDistance;
	int					extraJumpForce;


	//  - - - - SPEED - - - - 

	void				speedLimitChecker();					// Enemy limit speed in AXIS
	fPoint				speedLimit;								// Enemy max speed value

public:

	j1EnemyLand();
	j1EnemyLand(fPoint position);
	// Destructor
	~j1EnemyLand();

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

	void JumpFallLogic(float dt);
	void OnCollision(Collider* c1, Collider* c2);

	// - - - - PATHFINDING LAND ENEMY LOGIC - - - -

	bool GeneratingThePath(float timer, float dt, int auxAgroDistance);
	bool FollowingThePath(float speed, float dt);

	bool PathJumping();
	iPoint Stop();
};

#endif // __ENEMYLAND_H_