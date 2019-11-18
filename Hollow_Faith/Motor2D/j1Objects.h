#ifndef __j1OBJECTS_H__
#define __j1OBJECTS_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "j1Entity.h"

class j1Entity;

class j1Objects : public j1Module
{
	
public:
	j1Objects();
	~j1Objects();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	bool Draw(float dt);
	bool DebugDraw();

	j1Entity* AddEntity(j1Entity::entityType type, iPoint position);
	void DeleteEntities();

	void OnCollision(Collider* c1, Collider* c2) {};

public:
	
	p2SString		texture_path;
	SDL_Texture* texture;
	p2List<j1Entity*> Entities;	
	iPoint savedPosition;


};



#endif // !

