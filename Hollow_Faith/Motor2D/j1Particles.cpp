#include <math.h>
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Particles.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Entity.h"

#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles() : j1Entity(entityType::STONE)
{
	if (App->objects->particle == nullptr)
		App->objects->particle = this;
}

j1Particles::~j1Particles()
{}

bool j1Particles::Awake(pugi::xml_node& config)
{
	texture_path = config.child("graphics_player").attribute("path").as_string();

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

bool j1Particles::Start()
{
	texture = App->tex->Load(texture_path.GetString());	
	elim = true;	
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
			
			App->render->Blit(texture, p->position.x, p->position.y, &(p->anim.GetCurrentFrame(dt)), 1.0, 1.0, p->fliped, NULL, entity_collider.w / 2);
			if (p->fx_played == false)
			{
				// Play particle fx here
				p->fx_played = true;
			}
		}
	}

	return true;
}


void j1Particles::AddParticle(const Particle& particle, float x, float y, SDL_RendererFlip fliped, COLLIDER_TYPE collider_type, p2SString name,Uint32 delay)
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
			p->name = name.GetString();			

			if (collider_type != COLLIDER_NONE)
				p->collider = App->coll->AddCollider(p->anim.GetCurrentFrame(0), collider_type, this);

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
		if (name == "stone") {
			speed.y++;
		}

		
		if ((SDL_GetTicks() - born) > life) {
			if (name == "stone")
			{
				App->objects->particle->elim = true;
			}
				
			ret = false;
		}
	}

	else
		if (anim.Finished())
			ret = false;

	position.x += speed.x * App->dt;
	position.y += speed.y * App->dt;

	speed.y += App->objects->player->gravityForce * (App->dt * DT_CALIBRATED);


	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	return ret;
}

