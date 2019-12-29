#include "j1GUIconsole.h"
#include "j1App.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Window.h"

j1GUIconsole::j1GUIconsole() {

	this->type = GUItype::GUI_CONSOLE;
	texture = App->gui->GetAtlasTexture();
}

j1GUIconsole::~j1GUIconsole() {

}

bool j1GUIconsole::Start() {

	//Adding console elements
	image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, this, { 160, 60 }, { 0,0 }, interactable, enabled, { 0, 0, App->win->screen_surface->w, App->win->screen_surface->h / 2 }, nullptr);
	inputbox = App->gui->AddGUIelement(GUItype::GUI_INPUTBOX, this, { 0,App->win->screen_surface->h / 4 - 26 }, { 20,5 }, interactable, enabled, { 0,App->win->screen_surface->h / 2 - 42, App->win->screen_surface->w,42 }, nullptr);
	label = App->gui->AddGUIelement(GUItype::GUI_LABEL, this, { 0, 0 }, { 20,20 }, interactable, enabled, { 288, 144, 198, 100 }, nullptr);
	
	return true;
}

bool j1GUIconsole::PreUpdate() {

	//Update element's enabled from parent's
	inputbox->enabled = enabled;
	image->enabled = enabled;
	label->enabled = enabled;
	inputbox->focus = focus;
	
	//Delete string if not enabled
	if (!inputbox->enabled)
		consoleString.Clear();

	return true;
}

bool j1GUIconsole::CleanUp() {

	//Deleting elements
	inputbox = nullptr;
	image = nullptr;
	label = nullptr;
	return true;
}

void j1GUIconsole::PrintText(const char* txt) 
{
	consoleString += "- ";
	consoleString += txt;
	consoleString += ";";
	consoleString += "\n";

	//If there's a label, print console String with new chars(- , ; , \n)
	if (label) {
		App->tex->UnLoad(label->texture);
		label->texture = App->fonts->Print(consoleString.GetString());
		App->fonts->CalcSize(consoleString.GetString(), label->rect.w, label->rect.h);

		if (label->rect.h > image->rect.h - 250)		
			consoleString.Cut(consoleString.FindFirst("-"), consoleString.FindFirst(";") + 2);		
	}	
	
}