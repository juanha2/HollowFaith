#ifndef __j1INTROSCENE_H__
#define __j1INTROSCENE_H__

#include "j1Module.h"
#include "p2DynArray.h"

struct SDL_Texture;
class j1GUIelement;

class j1IntroScene : public j1Module
{
public:

	j1IntroScene();

	// Destructor
	virtual ~j1IntroScene();

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

	void GuiObserver(GUI_Event type, j1GUIelement* element);

public:

	SDL_Texture* texture;
	j1GUIelement* play_button= nullptr;
	j1GUIelement* continue_button = nullptr;
	j1GUIelement* exit_button = nullptr;

	j1GUIelement* scroll_bar = nullptr;

	j1GUIelement* settings_button = nullptr;
	j1GUIelement* settings_ui = nullptr;

	bool want_continue = false;
	bool want_exit = false;
};

#endif // __j1INTROSCENE_H__