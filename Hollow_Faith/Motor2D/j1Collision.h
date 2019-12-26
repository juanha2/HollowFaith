#ifndef __j1Collision_H__
#define __j1Collision_H__

#define MAX_COLLIDERS 200

#include "SDL/include/SDL.h"
#include "j1Module.h"
#include "j1Entity.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,

	COLLIDER_FLOOR,
	COLLIDER_PLAYER,
	COLLIDER_PLATFORM,
	COLLIDER_CLIMB,
	COLLIDER_ENEMY,
	COLLIDER_STONE,
	COLLIDER_DEATH,
	COLLIDER_WIN,
	COLLIDER_BONFIRE,
	COLLIDER_COINS,
	COLLIDER_MAX,
	
};

struct Collider
{
	SDL_Rect		rect;
	bool			to_delete = false;
	COLLIDER_TYPE	type;
	j1Entity*		callback = nullptr;


	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Entity* callback = nullptr) :
		rect(rectangle),
		type(type),				
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
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Entity * callback = nullptr);
	void AddColliderEntity(Collider* collider);
	void DebugDraw();



private:

	bool		debug = false;
	Collider*	colliders[MAX_COLLIDERS];
	bool		matrix[COLLIDER_MAX][COLLIDER_MAX];

};

#endif // __j1Collision_H__