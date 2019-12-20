#include "j1GUIlabel.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

j1GUIlabel::j1GUIlabel() 
{
	this->type = GUItype::GUI_LABEL;	
	texture = App->fonts->Print("Your Name: ", { 255,255,255,255 }, App->fonts->default);
}

j1GUIlabel::~j1GUIlabel() {

}


bool j1GUIlabel::Awake(pugi::xml_node&)
{
	
	return true;
}

bool j1GUIlabel::PreUpdate() 
{
	

	
	return true;
}

bool j1GUIlabel::Update(float dt) 
{	

	return true;
}

bool j1GUIlabel::PostUpdate()
{

	if (parent->focus)
	{		
		texture = App->fonts->Print(App->input->GetText(), { 255,255,255,255 }, App->fonts->default);
		SDL_Rect rect = { globalPosition.x + localPosition.x*7, globalPosition.y + localPosition.y*7,1,30 };		
		App->render->DrawQuad(rect, 255, 255, 255,255 ,true,false);
	}
	
	if(texture)
		App->render->Blit(texture, globalPosition.x + localPosition.x, globalPosition.y + localPosition.y, nullptr, 0.0f);


	return true;
}

bool j1GUIlabel::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}
