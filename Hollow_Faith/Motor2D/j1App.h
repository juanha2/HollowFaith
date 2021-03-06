#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

#include "Brofiler/Brofiler.h"
#pragma comment( lib, "Brofiler/ProfilerCore32.lib")


// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Collision;
class j1Scene;
class j1Map;
class j1FadeToBlack;
class j1PathFinding;
class j1EntityManager;
class j1Checkpoint;
class j1GUI;
class j1Fonts;
class j1IntroScene;
class j1Console;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module, bool init=true);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;
	p2SString GetLoadString();

	float DeltaTime() const;
	pugi::xml_node LoadConfig(pugi::xml_document&) const;
private:

	// Load config file
	

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SaveGameNow() const;

public:

	// - - - - MODULES - - - -

	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1Scene*			scene;
	j1Map*				map;
	j1Collision*		coll;	
	j1FadeToBlack*		fade_to_black;	
	j1PathFinding*		pathfinding;
	j1EntityManager*	objects;
	j1Checkpoint*		checkpoint;
	j1GUI*				gui;
	j1Fonts*			fonts;
	j1IntroScene*		intro;
	j1Console*			console;

	pugi::xml_document	config_file;
	// - - - - DELTA TIME - - - -

	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	uint32				frames_on_last_update;
	uint32				last_frame_ms;
	float				avg_fps;
	float				seconds_since_startup;
	float				dt;


	// - - - - FRAMERATE CAP - - - -

	uint32				frameratecap;
	uint32				desiredFrameratecap;
	

	bool				pause = false;


private:


	// - - - - APP BASICS - - - -

	p2List<j1Module*>	modules;
	uint				frames;
	int					argc;
	char**				args;


	// - - - - INFO - - - -

	p2SString			title;
	p2SString			organization;


	// - - - - LOAD/SAVING - - - -

	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game;
	mutable p2SString	save_game;
};

extern j1App* App; 

#endif