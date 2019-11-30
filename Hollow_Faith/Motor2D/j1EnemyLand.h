#ifndef __j1ENEMYLAND_H__
#define __j1ENEMYLAND_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entity.h"
#include "j1Module.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "p2Point.h"
#include "j1Enemy.h"
#include "j1Collision.h"

struct SDL_Texture;
struct Collider;

class j1EnemyLand : public j1Enemy
{

public:
	Animation idle;
	Animation walk;

	bool checkingFall = false;
	bool canJump = false;

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

	bool GeneratingThePath(float timer, float dt, int auxAgroDistance);
	bool FollowingThePath(float speed, float dt);

	void JumpFallLogic(float dt);
	void OnCollision(Collider* c1, Collider* c2);
};

#endif // __ENEMYLAND_H_