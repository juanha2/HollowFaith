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



	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;


	return ret;
}

bool j1EntityManager::PreUpdate()
{

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
		tmp = tmp->next;
	}

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

	while (tmp != nullptr)
	{
		if (tmp->data->type == j1Entity::entityType::PLAYER)
		{
			tmp->data->Load(file.child("player"));
		}
		else if (tmp->data->type == j1Entity::entityType::ENEMY_FLY)
		{
			tmp->data->Load(EnemyFly);
			EnemyFly = EnemyFly.next_sibling("EnemyFly");

		}
		else if (tmp->data->type == j1Entity::entityType::ENEMY_LAND)
		{
			tmp->data->Load(EnemyLand);
			EnemyLand = EnemyLand.next_sibling("EnemyLand");	
		}		

		tmp = tmp->next;
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


j1Entity* j1EntityManager::AddEntity(j1Entity::entityType type, fPoint position)
{
	j1Entity* tmp = nullptr;

	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file);

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
	}

	if (tmp) {
		Entities.add(tmp);
		tmp->Awake(config.child(App->objects->name.GetString()));
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