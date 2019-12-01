#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2DynArray.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	bool debug = false;
	SDL_Texture* debug_tex;
	int currentmap;
	int savedcurrentmap;
	bool different_map = false;
	bool ready_to_load = false;
	bool sound_repeat = false;

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	void SpawnEnemies();

	void sceneswitch();	

private:	
	
	
	bool first = true;	

};

#endif // __j1SCENE_H__