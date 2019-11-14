#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "j1Particles.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Enemies.h"
#include "p2Log.h"
#include "j1Audio.h"

Enemy::Enemy(int x, int y) : position(x, y)
{
	App->audio->LoadFx("audio/fx/death_enemy.wav");
}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{	
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &animation->GetCurrentFrame(), 1.0, 1.0);
}

void Enemy::OnCollision(Collider* collider)
{
	if (collider->type == COLLIDER_STONE) {
		LOG("YES");
		App->audio->PlayFx(8, 0, App->audio->FXvolume);
		App->particles->AddParticle(App->particles->death, position.x, position.y, SDL_RendererFlip::SDL_FLIP_NONE, COLLIDER_NONE);
	}
	//App->particles->AddParticle(App->particles->explosion, position.x, position.y);
	
}