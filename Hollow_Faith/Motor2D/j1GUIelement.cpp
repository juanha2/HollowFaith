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
	texture = App->gui->GetAtlasTexture();


	return true;
}


void j1GUIelement::Draw()
{
	if (hovering)
	{
		SDL_SetTextureColorMod(texture, 100, 200, 200);
		SDL_SetTextureAlphaMod(texture, 255);
	}
	else
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}

	App->render->Blit(texture, globalPosition.x + localPosition.x, globalPosition.y + localPosition.y, &rect, 0.0f);

	App->render->DrawQuad({ globalPosition.x + localPosition.x  * (int)App->win->GetScale(), globalPosition.y + localPosition.y  *(int)App->win->GetScale(), rect.w * (int)App->win->GetScale(), rect.h * (int)App->win->GetScale() }, 0, 255, 255, 255, false, false);
}


bool j1GUIelement::OnHover()
{
	bool ret = false;

	SDL_Point mouse;
	SDL_Rect intersect = { globalPosition.x + localPosition.x * (int)App->win->GetScale(), globalPosition.y + localPosition.y * (int)App->win->GetScale(), rect.w * (int)App->win->GetScale(), rect.h * (int)App->win->GetScale() };

	if (SDL_PointInRect(&mouse, &intersect) && this->enabled && this->interactable)
		ret = true;

	return ret;
}


void j1GUIelement::OnClick()
{}


void j1GUIelement::OnRelease()
{}
