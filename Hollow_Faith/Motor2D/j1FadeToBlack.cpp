#include <math.h>
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Checkpoint.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1IntroScene.h"


j1FadeToBlack::j1FadeToBlack(): j1Module()
{
	name.create("FadeToBlack");
}

bool j1FadeToBlack::Awake(pugi::xml_node&)
{	

	uint width, height = 0u;
	App->win->GetWindowSize(width, height);	

	screen = { 0, 0, (int)width * (int)App->win->GetScale(), (int)height * (int)App->win->GetScale() };
	return true;
}


// Load assets
bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1FadeToBlack::PostUpdate()
{

	BROFILER_CATEGORY("OnlyFadeToBlack_DontPanic", Profiler::Color::GhostWhite);

	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = 1.0f < ((float)now / (float)total_time) ? 1.0f : ((float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			to_disable->Disable();
			App->intro->Disable();
			App->pathfinding->CleanUp();
			App->objects->CleanUp();
			App->coll->CleanUp();
			App->input->blockingInput();
			App->input->Disable();
			
			if (App->map->Reset()) {
				
				if (App->map->Load(level_to_load.GetString())) {				
					
					App->scene->ready_to_load = false;
					App->scene->sound_repeat = false;
					
					App->checkpoint->Start();				
					App->render->camera = App->render->camera_init;
					
					int w, h;
					uchar* data = nullptr;
					if (App->map->CreateWalkabilityMap(w, h, &data))
					{
						App->pathfinding->SetMap(w, h, data);
						RELEASE_ARRAY(data);
					}
					App->audio->PlayMusic(App->map->data.music.GetString(), 1.0f);
				}

				black_screen = true;
			}	

		
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;	
		black_screen = false;
		if (now >= total_time) 
		{
			current_step = fade_step::none;
			App->input->Enable();
			to_enable->Enable();
		}
			
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(const char* lvlName, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		level_to_load.create(lvlName);
		ret = true;
	}

	return ret;
}


bool j1FadeToBlack::FadeToBlack(j1Module* SceneIn, j1Module* SceneOut, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;	
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		to_enable = SceneIn;
		to_disable = SceneOut;
		ret = true;
	}
	return true;
}

bool j1FadeToBlack::IsFading() const
{
	return current_step != fade_step::none;
}