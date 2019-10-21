#include <math.h>
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Particles.h"
#include "j1Player.h"
#include "j1Collision.h"

#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;
}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	// Loading particles 


	graphics = App->tex->Load("Assets/Sprites/Monster.png");

	// Dust Running Effect
	dustRunning.anim.PushBack({ 225, 241, 17, 14 });
	dustRunning.anim.PushBack({ 258, 245, 13, 11 });
	dustRunning.anim.PushBack({ 289, 243, 18, 13 });
	dustRunning.anim.PushBack({ 321, 240, 17, 16 });
	dustRunning.anim.PushBack({ 352, 239, 17, 17 });
	dustRunning.anim.PushBack({ 385, 243, 13, 13 });
	dustRunning.anim.loop = true;
	dustRunning.anim.speed = 0.1f;
	dustRunning.speed.x = 0;
	dustRunning.life = 6000;
	dustRunning.anim.firstLoopFrame = 2.0f;

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

	App->tex->UnLoad(graphics);

	return true;
}

// Update: draw background
bool j1Particles::Update()
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
			//if (p->life == 999998) App->render->Blit(App->player->graphics, p->position.x, p->position.y, !p->fliped, &(p->anim.GetCurrentFrame()), 1.0, true, true, true);
			App->render->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()), 1.0);
			if (p->fx_played == false)
			{
				// Play particle fx here
				p->fx_played = true;
			}
		}
	}

	return true;
}

void j1Particles::AddParticle(const Particle& particle, int x, int y, bool fliped, COLLIDER_TYPE collider_type, Uint32 delay)
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
				p->collider = App->coll->AddCollider(p->anim.GetCurrentFrame(), collider_type, 20, 10, 10, 5, this);
			active[i] = p;
			/*if (p->life == 5000) App->player->swordTrack = i;

			if (p->life == 999999)
				App->player->swordTrackGround = i;
			if (p->life == 999998) App->player2->swordTrackGround = i;*/

			break;
		}
	}
}

void j1Particles::OnCollision(Collider* c1, Collider* c2)
{
	/*for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (active[i] != nullptr && active[i]->collider == c1)
		{
			//AddParticle(tornadoHaoImpact, active[i]->position.x + 30, active[i]->position.y + 30, COLLIDER_NONE);
			delete active[i];
			active[i] = nullptr;
			break;

		}
	}*/
}

// -------------------------------------------------------------
// -------------------------------------------------------------

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