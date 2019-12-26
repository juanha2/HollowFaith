#ifndef __j1ENTITYMANAGER_H__
#define __j1ENTITYMANAGER_H__

#define MAX_BONFIRES 2

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "j1Entity.h"
#include "j1Player.h"
#include "j1Enemy.h"
#include "j1Particles.h"
#include "j1EnemyFly.h"
#include "j1Bonfire.h"
#include "j1Coins.h"

class j1Entity;

class j1EntityManager : public j1Module
{
	
public:

	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	bool Draw(float dt);

	j1Entity* AddEntity(j1Entity::entityType type, fPoint position, bool active=false);
	void DeleteEntities();
	void DeleteEntity();
	void DeleteEnemies();

	void OnCollision(Collider* c1, Collider* c2) {};

public:
	
	p2List<j1Entity*>	Entities;

	j1Player*			player = nullptr;
	j1Particles*		particle =nullptr;
	j1Bonfire*			bonfire[MAX_BONFIRES];
		
	int					count;	
	pugi::xml_node		config_node;

};



#endif // !

