
#ifndef __j1PARTICLES_H__
#define __j1PARTICLES_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "j1Collision.h"

#define MAX_ACTIVE_PARTICLES 100

struct SDL_Texture;

struct Particle
{	
	// - - - - PARTICLE BASICS - - - -

	int id;
	SDL_RendererFlip fliped = SDL_FLIP_NONE;
	Collider* collider = nullptr;
	Animation anim;
	uint fx = 0;
	bool fx_played = false;


	// - - - - MOBILITY - - - -

	fPoint position;
	fPoint speed;


	// - - - - LIFE - - - -

	Uint32 born = 0;
	Uint32 life = 0;
	

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
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	void AddParticle(const Particle& particle, float x, float y, SDL_RendererFlip fliped, COLLIDER_TYPE collider_type = COLLIDER_NONE, int num=0, Uint32 delay = 0);

private:

	// - - - - PARTICLES - - - -

	uint		last_particle = 0;


	// - - - - DESTROY - - -

	Uint32		defaultParticleLife;
	Uint32		stoneLife;
	Uint32		deathlife;

public:

	// - - - - PARTICLES - - - -

	Particle*	active[MAX_ACTIVE_PARTICLES];
	Particle	dustJumping;
	Particle	lifeUp;
	Particle	dustRunning;
	Particle	stone;
	Particle	death;


	// - - - - DESTROY - - -

	bool		elim;

};

#endif // __j1PARTICLES_H__
