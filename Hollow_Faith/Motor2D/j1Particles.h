
#ifndef __j1PARTICLES_H__
#define __j1PARTICLES_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "j1Entity.h"
#include "j1Collision.h"

#define MAX_ACTIVE_PARTICLES 100

struct SDL_Texture;

struct Particle
{	
	p2SString name;
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

class j1Particles : public j1Entity
{

public:
	j1Particles();
	~j1Particles();

	bool Start();
	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	void CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	void AddParticle(const Particle& particle, int x, int y, SDL_RendererFlip fliped, COLLIDER_TYPE collider_type = COLLIDER_NONE, p2SString name="", Uint32 delay = 0);

private:

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

	bool elim;

};

#endif // __j1PARTICLES_H__
