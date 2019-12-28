#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2DynArray.h"


struct SDL_Texture;
class j1GUIelement;

struct InGameMenu {

	j1GUIelement* menu_button = nullptr;
	j1GUIelement* title = nullptr;
	j1GUIelement* image = nullptr;
	j1GUIelement* exit_button = nullptr;
	j1GUIelement* return_button = nullptr;
	j1GUIelement* resume_button = nullptr;
	j1GUIelement* volume_scroll = nullptr;
	j1GUIelement* music_scroll = nullptr;
	j1GUIelement* save = nullptr;
	j1GUIelement* load = nullptr;
	j1GUIelement* label1 = nullptr;
	j1GUIelement* label2 = nullptr;
};

struct CompleteMenu {

	j1GUIelement* image = nullptr;
	j1GUIelement* exit_button = nullptr;
	j1GUIelement* title = nullptr;
	j1GUIelement* timer = nullptr;
	j1GUIelement* score = nullptr;
};


class j1Scene : public j1Module
{
public:
	
	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

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

	void sceneswitch();	
	void LoadMap(int num_map);

	void GuiObserver(GUI_Event type, j1GUIelement* element);

	void AddUIElements();
	void EnableDisableMenu();
	void EnableDisableVictoryMenu();

public:

	// - - - - SCENE DATA - - - -

	bool			debug = false;
	SDL_Texture*	debug_tex;
	int				currentmap;
	int				savedcurrentmap;
	bool			different_map = false;
	bool			ready_to_load = false;
	bool			sound_repeat = false;
	bool			first = true;
	bool			want_exit = false;

	int					lifes;
	int					num_coins = 0;	
	int					score; 
	

	p2SString			jump_fx;
	p2SString			death_fx;
	p2SString			win1_Fx;
	p2SString			win2_Fx;
	p2SString			landing_Fx;
	p2SString			hover_Fx;
	p2SString			bounce_Fx;
	p2SString			stone_Fx;
	p2SString			fire_Fx;
	p2SString			fuse_Fx;
	p2SString			death;
	p2SString			hurt_Fx;
	p2SString			coin_Fx;
	p2SString			life_Fx;
	p2SString			score_Fx;
	
	float				timer;
	float				life_timer;
	char				timerText[100];
	char				scoreText[100];
	char				lifesText[100];
	char				coinsText[100];

	j1GUIelement*		lifes_label=nullptr;
	j1GUIelement*		coins_label = nullptr;
	j1GUIelement*		lifes_icon = nullptr;
	j1GUIelement*		coins_icon = nullptr;
	j1GUIelement*		input_box = nullptr;
	j1GUIelement*		timer_label = nullptr;	
	j1GUIelement*		timer_icon = nullptr;
	j1GUIelement*		score_label = nullptr;
	j1GUIelement*		score_icon = nullptr;

	InGameMenu			menu;	
	CompleteMenu		complete;
};

#endif // __j1SCENE_H__