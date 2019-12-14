#include "j1GUIlabel.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"


j1GUIlabel::j1GUIlabel() {

	this->type = GUItype::GUI_LABEL;

	// We want label's texture to be the string we write, not the default texture GUIElement provides	
	fontTexture = App->fonts->Print("Your Name", { 255,255,255,255 }, App->fonts->default);
	
}

j1GUIlabel::~j1GUIlabel() {

}


bool j1GUIlabel::Awake(pugi::xml_node&)
{

	return true;
}

bool j1GUIlabel::PostUpdate()
{
	
	App->render->Blit(fontTexture, globalPosition.x + localPosition.x, globalPosition.y + localPosition.y, nullptr, 0.0f);

	return true;
}

bool j1GUIlabel::CleanUp()
{
	App->tex->UnLoad(fontTexture);
	return true;
}
