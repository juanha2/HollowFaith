#include "j1GUIlabel.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

j1GUIlabel::j1GUIlabel() 
{
	this->type = GUItype::GUI_LABEL;
}

j1GUIlabel::~j1GUIlabel() {

}


bool j1GUIlabel::Awake(pugi::xml_node&)
{
	
	return true;
}

bool j1GUIlabel::Start()
{	
	texture = App->fonts->Print(text);
	return true;
}


bool j1GUIlabel::PreUpdate() 
{
	CleanUp();
	App->fonts->CalcSize(App->input->GetText().GetString(), rect.w, rect.h);
	return true;
}

bool j1GUIlabel::Update(float dt) 
{	
	
	texture = App->fonts->Print(text);
	return true;
}

bool j1GUIlabel::PostUpdate()
{		
	if (enabled) 			
		App->render->Blit(texture, globalPosition.x + localPosition.x, globalPosition.y + localPosition.y, nullptr, 0.0f);
			

	return true;
}

bool j1GUIlabel::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}
