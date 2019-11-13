#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "j1Module.h"
#include "j1Textures.h"
#include "Enemy_fly.h"

#define MAX_ENEMIES 100

// TODO 2: Add a new enemy: Brown Cookies!

// TODO 3: Have the Brown Cookies describe a path in the screen

// TODO 4: Create a new enemy type: the Mech

enum ENEMY_TYPES
{
	NO_TYPE,
	ENEMY_FLY,
};

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
};


class ModuleEnemies : public j1Module
{
public:

	ModuleEnemies();
	virtual  ~ModuleEnemies();

	bool Start();
	bool Awake(pugi::xml_node& config);
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool AddEnemy(ENEMY_TYPES type, int x, int y);

private:

	void SpawnEnemy(const EnemyInfo& info);

private:

	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];
	SDL_Texture* sprites;
};

#endif // __ModuleEnemies_H__