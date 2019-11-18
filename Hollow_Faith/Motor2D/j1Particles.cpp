#include <math.h>
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Particles.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "p2Log.h"

#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles()
{

	name.create("particles");

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;
}

j1Particles::~j1Particles()
{}

bool j1Particles::Awake(pugi::xml_node& config)
{
	// Loading Particle Animations
	pugi::xml_node animIterator = config.child("animations").child("animation");

	dustJumping.anim.load_animation(animIterator, "dustJumping");
	dustJumping.life = defaultParticleLife;

	dustRunning.anim.load_animation(animIterator, "dustRunning");
	dustRunning.life = defaultParticleLife;

	stone.anim.load_animation(animIterator, "stone");
	stone.life = stoneLife;

	death.anim.load_animation(animIterator, "death");
	death.life = deathlife;
	

	return true;

}

// Unload assets
bool j1Particles::CleanUp()
{

	// Unloading particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}


	return true;
}

// Update: draw background
bool j1Particles::Update(float dt)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;
		
		if (p->Update() == false)
		{			
			delete p;
			active[i] = nullptr;				
		}

	
		else if (SDL_GetTicks() >= p->born)
		{
		
			//App->render->Blit(App->player->graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame(dt)), 1.0, 1.0, p->fliped);

			if (p->fx_played == false)
			{
				// Play particle fx here
				p->fx_played = true;
			}
		}
		
	
	}

	return true;
}

void j1Particles::AddParticle(const Particle& particle, int x, int y, SDL_RendererFlip fliped, COLLIDER_TYPE collider_type, Uint32 delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			p->fliped = fliped;

			if (collider_type != COLLIDER_NONE)
				p->collider = App->coll->AddCollider(p->anim.GetCurrentFrame(0), collider_type, nullptr);

			active[i] = p;

			break;
		}
	}
}
// Detecting particles collision

// -------------------------------------------------------------

void j1Particles::OnCollision(Collider* c1, Collider* c2) 

{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (active[i] != nullptr && active[i]->collider == c1)
		{
			elim = true;
			delete active[i];
			active[i] = nullptr;
			break;
		}
	}

} 
Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life)
{}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->to_delete = true;
}


bool Particle::Update()
{
	bool ret = true;

	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
		if (anim.Finished())
			ret = false;

	position.x += speed.x;
	position.y += speed.y;

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	return ret;
}