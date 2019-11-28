#ifndef __j1ENEMY_H__
#define __j1ENEMY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entity.h"
#include "j1Module.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class j1Enemy : public j1Entity
{

public:
	
	p2DynArray<iPoint> pathToPlayer;
	iPoint originalPos;

	p2SString			death;
	float timer;

	bool chase = false;

	int agroDistance = 200;	
	int pathCadency = 1;

	float enemySpeed = 3.0f;

public:

	j1Enemy(j1Entity::entityType type);
	j1Enemy(j1Entity::entityType type, iPoint pos);
	// Destructor
	~j1Enemy();

	void Load(pugi::xml_node& file);
	void Save(pugi::xml_node& file) const;

	void OnCollision(Collider* c1, Collider* c2);
	
	
};

#endif // __ENEMY_H__