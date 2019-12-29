#include "j1GUI.h"
#include "j1GUIelement.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Scene.h"


j1GUIelement::~j1GUIelement()
{ }

bool j1GUIelement::Start()
{ return true; }


void j1GUIelement::Draw()
{
	BROFILER_CATEGORY("GUI_Drawing", Profiler::Color::DarkKhaki);

	// All the visual feedback from the state of the GUI

	if (above && interactable && !decorative)
	{
		SDL_SetTextureColorMod(texture, 160, 160, 160);
		SDL_SetTextureAlphaMod(texture, 255);
	}
	else if (!above && interactable && !decorative)
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}

	if (!interactable && this->type != GUItype::GUI_BUTTON && !decorative)
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}
	else if (!interactable && this->type == GUItype::GUI_BUTTON && !decorative) 
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 50);
	}

	if (decorative) 
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}

	App->render->Blit(texture, globalPosition.x, globalPosition.y, &rect, 0.0f, 0.0f);

	if(App->scene->debug)
		App->render->DrawQuad({ globalPosition.x, globalPosition.y, rect.w, rect.h }, 0, 255, 255, 255, false, false, true);
}

// Detects when the mouse is above any GUI element and sends the info to the observer.
bool j1GUIelement::OnAbove()
{
	bool ret = false;

	SDL_Point mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	SDL_Rect intersect = {globalPosition.x , globalPosition.y, rect.w, rect.h };

	if (SDL_PointInRect(&mouse, &intersect) && this->enabled && this->interactable) {
		if (listener != nullptr)
		{
			this->listener->GuiObserver(GUI_Event::EVENT_HOVER, this, nullptr, nullptr);
		}
		ret = true;
	}

	return ret;
}

// Detects when click to any GUI element and sends the info to the observer.
void j1GUIelement::OnClick()
{

	if (listener != nullptr) 
	{
		this->listener->GuiObserver(GUI_Event::EVENT_ONCLICK, this, nullptr, nullptr);
	}		

}



void j1GUIelement::OnRelease()
{
	
}


void j1GUIelement::Dragging()
{

}

void j1GUIelement::PrintText(const char* txt) {

}
