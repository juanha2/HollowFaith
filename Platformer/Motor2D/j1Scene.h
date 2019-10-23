#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:
	int currentmap;
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


private:
	
	void Win_Lose_Condition();
	bool first = true;
	SDL_Texture* graphics = nullptr;
	bool sound_repeat = false;

};

#endif // __j1SCENE_H__