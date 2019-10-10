#ifndef __j1Collision_H__
#define __j1Collision_H__

#define MAX_COLLIDERS 50

#include "SDL/include/SDL.h"
#include "j1Module.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_FLOOR,
	COLLIDER_PLAYER,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;
	int damage = NULL;
	int delayPlayer = NULL; // We'll use this to set the player animation delay when he doesn't land a hit, as well for the player knockback when the attack is landed.
	int delayEnemy = NULL;	//Same as above
	int attackType = NULL; //We'll use this to set the attack type: punch, slash, kick... To set the sound effects & particles

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, int damage, int delayPlayer, int delayEnemy, int attackType, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		damage(damage),
		delayPlayer(delayPlayer),
		delayEnemy(delayEnemy),
		attackType(attackType),
		callback(callback)
	{}
	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	void setType(COLLIDER_TYPE type1)
	{
		type = type1;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collision : public j1Module
{
public:

	j1Collision();
	~j1Collision();

	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, int damage = 0, int delayPlayer = 0, int delayEnemy = 0, int attackType = 0, j1Module * callback = nullptr);
	void DebugDraw();
	

private:

	bool debug = false;
	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	
};

#endif // __j1Collision_H__
