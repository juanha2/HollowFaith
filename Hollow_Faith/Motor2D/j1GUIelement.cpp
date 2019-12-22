#include "j1GUI.h"
#include "j1GUIelement.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"



j1GUIelement::~j1GUIelement()
{
}

bool j1GUIelement::Start()
{
	

	return true;
}


void j1GUIelement::Draw()
{
	if (above)
	{
		SDL_SetTextureColorMod(texture, 200, 50, 50);
		SDL_SetTextureAlphaMod(texture, 200);
	}
	else
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}

	App->render->Blit(texture, globalPosition.x + localPosition.x, globalPosition.y + localPosition.y, &rect, 0.0f);

	App->render->DrawQuad({ globalPosition.x + localPosition.x, globalPosition.y + localPosition.y, rect.w, rect.h }, 0, 255, 255, 255, false, false, true);
}


bool j1GUIelement::OnAbove()
{
	bool ret = false;

	SDL_Point mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	SDL_Rect intersect = {globalPosition.x + localPosition.x, globalPosition.y + localPosition.y, rect.w, rect.h };

	if (SDL_PointInRect(&mouse, &intersect) && this->enabled && this->interactable)
		ret = true;

	return ret;
}


void j1GUIelement::OnClick()
{}


void j1GUIelement::OnRelease()
{}
