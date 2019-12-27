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
{
	
}

bool j1GUIelement::Start()
{
	

	return true;
}


void j1GUIelement::Draw()
{
	if (above && interactable)
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}
	else if (!above && interactable)
	{
		SDL_SetTextureColorMod(texture, 125, 125, 125);
		SDL_SetTextureAlphaMod(texture, 255);
	}

	if (!interactable) 
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}

	App->render->Blit(texture, globalPosition.x, globalPosition.y, &rect, 0.0f, 0.0f);

	if(App->scene->debug)
		App->render->DrawQuad({ globalPosition.x, globalPosition.y, rect.w, rect.h }, 0, 255, 255, 255, false, false, true);
}


bool j1GUIelement::OnAbove()
{
	bool ret = false;

	SDL_Point mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	SDL_Rect intersect = {globalPosition.x , globalPosition.y, rect.w, rect.h };

	if (SDL_PointInRect(&mouse, &intersect) && this->enabled && this->interactable) {
		if (listener != nullptr)
		{
			this->listener->GuiObserver(GUI_Event::EVENT_HOVER, this);
		}
		ret = true;
	}

	return ret;
}


void j1GUIelement::OnClick()
{

	if (listener != nullptr) 
	{
		this->listener->GuiObserver(GUI_Event::EVENT_ONCLICK, this);
	}		

}



void j1GUIelement::OnRelease()
{
	
}


void j1GUIelement::Dragging()
{

}
