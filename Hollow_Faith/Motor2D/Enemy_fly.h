#ifndef __ENEMY_FLY_H__
#define __ENEMY_FLY_H__

#include "Enemy.h"

class Enemy_fly : public Enemy
{
private:

	Animation fly;	

public:

	Enemy_fly(int x, int y);

	void Move();
	
};

#endif // __ENEMY_FLY_H__