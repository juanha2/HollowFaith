#include "j1GUI.h"
#include "j1GUIelement.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"



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
	App->render->Blit(texture, globalPosition.x + localPosition.x, globalPosition.y + localPosition.y, NULL);
}


bool j1GUIelement::OnHover()
{
	bool ret = false;

	SDL_Point mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	if (SDL_PointInRect(&mouse, &this->rect) && this->enabled && this->interactable)
		ret = true;

	return ret;
}


void j1GUIelement::OnClick()
{}


void j1GUIelement::OnRelease()
{}
