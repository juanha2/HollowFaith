#ifndef __j1ENEMY_H__
#define __j1ENEMY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entity.h"
#include "j1Module.h"
#include "p2DynArray.h"

struct SDL_Texture;
struct Collider;

class j1Enemy : public j1Entity
{

public:

	Animation animation;
	p2DynArray<iPoint> pathToPlayer;

	bool chase;
	int agroDistance = 50;

public:

	j1Enemy();

	// Destructor
	~j1Enemy();

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

	void Load(pugi::xml_node& file);
	void Save(pugi::xml_node& file) const;

	void OnCollision(Collider* c1, Collider* c2);
};

#endif // __ENEMY_H__