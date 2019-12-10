#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Textures.h"
#include "j1Enemy.h"
#include "j1Particles.h"
#include "j1EnemyFly.h"
#include "j1EnemyLand.h"
#include "j1Bonfire.h"
#include "j1Audio.h"


j1EntityManager::j1EntityManager()
{
	name.create("objects");
}

j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;

	pugi::xml_node fxIterator = config.child("fx");
	jump_fx = fxIterator.child("jumpFx").attribute("path").as_string();
	death_fx = fxIterator.child("deathFx").attribute("path").as_string();
	win1_Fx = fxIterator.child("win1Fx").attribute("path").as_string();
	win2_Fx = fxIterator.child("win2Fx").attribute("path").as_string();
	landing_Fx = fxIterator.child("landingFx").attribute("path").as_string();
	hover_Fx = fxIterator.child("hoverFx").attribute("path").as_string();
	stone_Fx = fxIterator.child("stoneFx").attribute("path").as_string();
	fire_Fx = fxIterator.child("bonfireFx").attribute("path").as_string();
	fuse_Fx = fxIterator.child("fuseFx").attribute("path").as_string();
	death = fxIterator.child("death_enemyFx").attribute("path").as_string();
	hurt_Fx = fxIterator.child("hurtFx").attribute("path").as_string();


	config_node = config;

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;

	//We load every Fx here so each entity doesn't have to repeat loading the same one	

	App->audio->LoadFx(jump_fx.GetString());		//1
	App->audio->LoadFx(death_fx.GetString());		//2
	App->audio->LoadFx(win1_Fx.GetString());		//3
	App->audio->LoadFx(win2_Fx.GetString());		//4
	App->audio->LoadFx(landing_Fx.GetString());		//5
	App->audio->LoadFx(hover_Fx.GetString());		//6	
	App->audio->LoadFx(stone_Fx.GetString());		//7
	App->audio->LoadFx(fire_Fx.GetString());		//8
	App->audio->LoadFx(fuse_Fx.GetString());		//9
	App->audio->LoadFx(death.GetString());			//10
	App->audio->LoadFx(hurt_Fx.GetString());		//11
	
	return ret;
}

bool j1EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("AllEntities_PreUpdate", Profiler::Color::PaleTurquoise);

	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->PreUpdate();
		tmp = tmp->next;
	}

	return ret;
}
bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("AllEntities_Update", Profiler::Color::Turquoise);

	if (dt > 0.15)
		dt = 0.15;

	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->Update(dt);
		tmp = tmp->next;
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("AllEntities_PostUpdate", Profiler::Color::DarkTurquoise);

	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->PostUpdate();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;
		
	p2List_item<j1Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->CleanUp();
		RELEASE(tmp->data);
		Entities.del(tmp);
		tmp = tmp->next;
	}

	App->objects->player = nullptr;
	App->objects->particle = nullptr;

	for (int i = 0; i <= MAX_BONFIRES; i++) 
	{
		if(App->objects->bonfire[i] != nullptr)
			App->objects->bonfire[i] = nullptr;
	}
		

	count = 0;

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& file) const
{
	
	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Save(file);
		tmp = tmp->next;
	}
	
	return ret;
}

bool j1EntityManager::Load(pugi::xml_node& file)
{
	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;
	pugi::xml_node EnemyFly = file.child("EnemyFly");
	pugi::xml_node EnemyLand = file.child("EnemyLand");
	pugi::xml_node Bonfire = file.child("Bonfire");	
	

	while (tmp != nullptr)
	{
		if (tmp->data->type == j1Entity::entityType::PLAYER)
		{
			tmp->data->Load(file.child("player"));
		}
		if (tmp->data->type == j1Entity::entityType::BONFIRE)
		{
			tmp->data->Load(file.child("Bonfire"));
		}
		tmp = tmp->next;
	}

	DeleteEnemies();

	p2SString enemyLand="EnemyLand";
	p2SString enemyFly="EnemyFly";
	p2SString bonfire = "Bonfire";

	for (pugi::xml_node iterator = file.child("EnemyLand"); iterator; iterator = iterator.next_sibling("EnemyLand"))
	{		
		if (enemyLand == iterator.name())
		{
			AddEntity(j1Entity::entityType::ENEMY_LAND, { iterator.child("position").attribute("x").as_float(),iterator.child("position").attribute("y").as_float() });
		}
	}

	for (pugi::xml_node iterator = file.child("EnemyFly"); iterator; iterator = iterator.next_sibling("EnemyFly"))
	{
		if (enemyFly == iterator.name())
		{
			AddEntity(j1Entity::entityType::ENEMY_FLY, { iterator.child("position").attribute("x").as_float(),iterator.child("position").attribute("y").as_float() });
		}
	}

	return ret;
}


bool j1EntityManager::Draw(float dt)
{

	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Draw(dt);
		tmp = tmp->next;
	}
	return ret;
}


j1Entity* j1EntityManager::AddEntity(j1Entity::entityType type, fPoint position,  bool active)
{
	j1Entity* tmp = nullptr;	

	switch (type)
	{
	case j1Entity::entityType::PLAYER:
		tmp = new j1Player();
		break;

	case j1Entity::entityType::ENEMY_FLY:
		
		tmp = new j1EnemyFly(position);		
		break;

	case j1Entity::entityType::ENEMY_LAND:

		tmp = new j1EnemyLand(position);	
		break;

	case j1Entity::entityType::STONE:
		tmp = new j1Particles();
		break;

	case j1Entity::entityType::BONFIRE:
		tmp = new j1Bonfire(position, count, active);
		count++;
		break;
	}

	if (tmp) 
	{
		tmp->position.x = position.x;
		tmp->position.y = position.y;


		Entities.add(tmp);
		tmp->Awake(config_node);
		tmp->Start();
	}
	
	return tmp;
}

void j1EntityManager::DeleteEntities()
{
	p2List_item<j1Entity*>* tmp = Entities.end;
	while (tmp != nullptr)
	{
		p2List_item<j1Entity*>* tmp2 = tmp;
		RELEASE(tmp->data);
		Entities.del(tmp2);
		tmp = tmp->prev;
	}	
}

void j1EntityManager::DeleteEntity()
{
	p2List_item<j1Entity*>* tmp = Entities.end;

	while (tmp != nullptr)
	{
		p2List_item<j1Entity*>* tmp2 = tmp;

		if (tmp->data->elim) 
		{
			tmp->data->CleanUp();
			RELEASE(tmp->data);
			Entities.del(tmp2);
		}
		
		tmp = tmp->prev;
	}
	
}

void j1EntityManager::DeleteEnemies()
{
	p2List_item<j1Entity*>* tmp = Entities.end;

	while (tmp != nullptr)
	{
		p2List_item<j1Entity*>* tmp2 = tmp;

		if ((tmp->data->type == j1Entity::entityType::ENEMY_FLY) || (tmp->data->type == j1Entity::entityType::ENEMY_LAND))
		{		
			tmp->data->CleanUp();
			tmp->data->collider->to_delete = true;
			RELEASE(tmp->data);
			Entities.del(tmp2);
			tmp = tmp->prev;
		}
		else
			tmp = tmp->prev;		
	}

}