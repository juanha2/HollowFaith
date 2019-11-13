#ifndef __j1Enemies_H__
#define __j1Enemies_H__

#include "j1Module.h"
#include "j1Textures.h"
#include "Enemy_fly.h"
#include "p2SString.h"

#define MAX_ENEMIES 100


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


class j1Enemies : public j1Module
{
public:

	j1Enemies();
	virtual  ~j1Enemies();

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

	EnemyInfo		queue[MAX_ENEMIES];
	Enemy*			enemies[MAX_ENEMIES];

	p2SString		sprites_path;
	SDL_Texture*	sprites;
};

#endif // j1Enemies