#ifndef __j1ENEMY_H__
#define __j1ENEMY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "j1Module.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class j1Enemy : public j1Entity
{

public:
	
	// - - - - PATHFINDING - - - -

	p2DynArray<iPoint>		pathToPlayer;
	float					pathCadency;
	int						pathMinDist;
	int						agroDistance;

	float					timer;
	bool					chase = false;

	// - - - - MOBILITY - - - -

	fPoint					originalPos;
	fPoint					movementForce;

	// - - - - FIGHT - - - -

	bool					hurted = false;
	bool					hurtedConsec = false;
	float					timeConsec = 0.0f;
	

public:

	j1Enemy(j1Entity::entityType type);
	j1Enemy(j1Entity::entityType type, fPoint pos);

	// Destructor
	~j1Enemy();
	
	bool Load(pugi::xml_node& file);
	bool Save(pugi::xml_node& file) const;

};

#endif // __ENEMY_H__