#ifndef __j1ENEMYFLY_H__
#define __j1ENEMYFLY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entity.h"
#include "j1Module.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "p2Point.h"
#include "j1Enemy.h"

struct SDL_Texture;
struct Collider;

class j1EnemyFly : public j1Enemy
{

public:

	Animation animation;	
public:

	j1EnemyFly();

	// Destructor
	~j1EnemyFly();

	bool Awake(pugi::xml_node&);
	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	void CleanUp();

};

#endif // __ENEMYFLY_H__