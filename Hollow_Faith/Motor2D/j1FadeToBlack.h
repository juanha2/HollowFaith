#ifndef __j1FADETOBLACK_H__
#define __j1FADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"



class j1FadeToBlack : public j1Module
{

public:

	j1FadeToBlack();
	~j1FadeToBlack() {} ;
	
	bool Awake(pugi::xml_node&);
	bool Start();
	//bool Update(float dt);
	bool PostUpdate();
	bool FadeToBlack(const char* lvlName, float time = 1.0f);
	bool FadeToBlack(j1Module* SceneIn, j1Module* SceneOut);
	bool IsFading() const;
	
	bool black_screen = false;
	

private:

	enum fade_step
	{

		none,
		fade_to_black,
		fade_from_black

	} current_step = fade_step::none;

	j1Module* to_enable = nullptr;
	j1Module* to_disable = nullptr;

	Uint32		start_time = 0;
	Uint32		total_time = 0;
	SDL_Rect	screen;
	p2SString	level_to_load;
	
};

#endif //__J1FADETOBLACK_H__