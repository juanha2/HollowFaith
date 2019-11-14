
#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "p2Point.h"
#include "j1Collision.h"

#define MAX_ACTIVE_PARTICLES 100

struct SDL_Texture;
struct Collider;
enum COLLIDER_TYPE;

struct Particle
{
	Collider* collider = nullptr;
	Animation anim;
	uint fx = 0;
	iPoint position;
	iPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	bool fx_played = false;
	SDL_RendererFlip fliped = SDL_FLIP_NONE;



	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class j1Particles : public j1Module
{

public:
	j1Particles();
	~j1Particles();

	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);


	void AddParticle(const Particle& particle, int x, int y, SDL_RendererFlip fliped, COLLIDER_TYPE collider_type = COLLIDER_NONE, Uint32 delay = 0);

private:

	SDL_Texture* graphics = nullptr;
	uint last_particle = 0;
	Uint32 defaultParticleLife = 200;
	Uint32 stoneLife = 1000;
	Uint32 deathlife = 500;

public:

	Particle* active[MAX_ACTIVE_PARTICLES];
	Particle dustJumping;
	Particle dustRunning;
	Particle stone;
	Particle death;

	bool elim = true;

};

#endif // __MODULEPARTICLES_H__
