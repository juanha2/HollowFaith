#ifndef __j1ENTITYMANAGER_H__
#define __j1ENTITYMANAGER_H__

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

	j1Entity* AddEntity(j1Entity::entityType type, fPoint position);
	void DeleteEntities();
	void DeleteEntity();
	void DeleteEnemies();

	void OnCollision(Collider* c1, Collider* c2) {};

public:
	
	p2List<j1Entity*>	Entities;

	j1Player* player;
	j1Enemy* enemy;
	j1Particles* particle;
	j1Bonfire* bonfire[3];

	p2SString			jump_fx;
	p2SString			death_fx;
	p2SString			win1_Fx;
	p2SString			win2_Fx;
	p2SString			landing_Fx;
	p2SString			hover_Fx;
	p2SString			bounce_Fx;
	p2SString			stone_Fx;
	p2SString			fire_Fx;
	p2SString			fuse_Fx;
	p2SString			death;
	p2SString			hurt_Fx;
	
	int					count;
	
};



#endif // !

