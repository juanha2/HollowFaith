#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "SDL/include/SDL.h"
#include "j1Fonts.h"
#include "j1Scene.h"
#include "j1Console.h"

#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name.create("input");

	keyboard = new j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);
	
}

// Destructor
j1Input::~j1Input()
{
	delete[] keyboard;
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	static SDL_Event event;
	
	
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (!text_input) {
		for (int i = 0; i < MAX_KEYS; ++i)
		{
			if (keys[i] == 1)
			{
				if (keyboard[i] == KEY_IDLE)
					keyboard[i] = KEY_DOWN;
				else
					keyboard[i] = KEY_REPEAT;
			}
			else
			{
				if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
					keyboard[i] = KEY_UP;
				else
					keyboard[i] = KEY_IDLE;
			}
		}

	}
	

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if(mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}
	
		while (SDL_PollEvent(&event) != 0)
		{
	
			switch (event.type)
			{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
				break;

			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
				case SDL_WINDOWEVENT_HIDDEN:
				case SDL_WINDOWEVENT_MINIMIZED:
				case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
				case SDL_WINDOWEVENT_SHOWN:
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				case SDL_WINDOWEVENT_MAXIMIZED:
				case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
				break;

			case SDL_TEXTINPUT:
				
				if (cursor_position != 0)
					text.insert(event.text.text, cursor_position);
				else
					text+=event.text.text;				

				break;

			case SDL_KEYDOWN:

				if (text_input) 
				{
					if (event.key.keysym.sym == SDLK_BACKSPACE && text.Length() > 0) {
						text.Cut(text.Length() - cursor_position-1, text.Length()-cursor_position);
					}
					if (event.key.keysym.sym == SDLK_LEFT && text.Length() > 0 && cursor_position < text.Length()) {
						cursor_position++;
					}
					if (event.key.keysym.sym == SDLK_RIGHT && text.Length() > 0 && cursor_position>0) {
						cursor_position--;
					}
					if (event.key.keysym.sym == SDLK_RETURN) {
						cursor_position = 0;
						LOG("%s",text.GetString());
						text.Clear();					
					}
					if (event.key.keysym.sym == SDLK_BACKQUOTE) {					
						App->console->EnableDisableConsole();
						text.Clear();											
						cursor_position = 0;					
					}
				}				

				break;	

			case SDL_MOUSEBUTTONDOWN:
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
				break;

			case SDL_MOUSEBUTTONUP:
				mouse_buttons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
				break;

			case SDL_MOUSEMOTION:
				int scale = App->win->GetScale();
				mouse_motion_x = event.motion.xrel / scale;
				mouse_motion_y = event.motion.yrel / scale;
				mouse_x = event.motion.x / scale;
				mouse_y = event.motion.y / scale;

				break;
			}
		}	
		
	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

void j1Input::blockingInput() {

	for (int i = 0; i < MAX_KEYS; ++i)
		keyboard[i] = KEY_UP;
	
}

void j1Input::EnableTextInput() 
{
	SDL_StartTextInput();
	text_input = true;	
}

void j1Input::DisableTextInput() {

	SDL_StopTextInput();
	text_input = false;
	App->input->text.Clear();
}

p2SString j1Input::GetText() {
	
	return text;
}

int j1Input::GetCursorPosition() {

	int width = 0;
	int height = 0;

	App->fonts->CalcSize(GetModifiedString().GetString(), width, height);

	return width;
}

p2SString j1Input::GetModifiedString() 
{

	if (cursor_position != 0) {

		p2SString new_text(text.GetString());
		new_text.Cut(text.Length() - cursor_position);
		return new_text;
	}

	else
		return text;
}

