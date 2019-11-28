#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "p2Log.h"
#include "SDL/include/SDL_render.h"

#define MAXNUMOFCOLLIDERS 1

enum collDirCheck
{
	DIR_UNKNOWN = -1,

	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_MAX
};


class j1Entity
{
public:
	enum entityType
	{
		NO_TYPE,
		PLAYER,		
		ENEMY_FLY,
		ENEMY_LAND,
		STONE,
	};

public:
	j1Entity();
	j1Entity(entityType type);
	~j1Entity();
	
	virtual bool Awake(pugi::xml_node& config) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };	

	virtual void CleanUp();
	virtual void Save(pugi::xml_node& file) const {};
	virtual void Load(pugi::xml_node& file) {};	
	
	void PositionUpdate(float dt);						//Update entity's position
	void Draw(float dt);	

	virtual void OnCollision(Collider* c1, Collider* c2) {};
	void CollisionPosUpdate();

public:
	
	Animation*			current_animation = nullptr;
	entityType			type;
	fPoint				speed;
	iPoint				position;
	iPoint				maxSpeed;	

	Collider*			collider;
	bool				ignoreColl = false;
	bool				godMode = false;
	float				Acceleration;						// Player acceleration AXIS value
	SDL_RendererFlip	flip = SDL_RendererFlip::SDL_FLIP_NONE;

	SDL_Rect			entity_collider;
	
	p2SString			texture_path;
	SDL_Texture*		texture;
	bool				elim = false;
	iPoint				savedPosition;
	
};

#endif