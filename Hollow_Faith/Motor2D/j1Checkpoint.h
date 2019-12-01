#ifndef __j1CHECKPOINT_H__
#define __j1CHECKPOINT_H__

#include "j1Module.h"
#include "p2Point.h"

struct SDL_Window;
struct SDL_Surface;

class j1Checkpoint : public j1Module
{
public:

	j1Checkpoint();

	// Destructor
	~j1Checkpoint() ;

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	void SaveCheckPoints();
	void LoadCheckPoints();

public:

	// - - - - CHECKPOINT BASICS - - - -

	p2SString			save_game;
	p2SString			load_game;

	bool				save_checkpoints = false;	

	bool				checkpoint = false;
	fPoint				checkpointpos;
	int					num_checkpoint = 0;	

};

#endif //  __j1CHECKPOINT_H__