#include "j1GUIinputBox.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"

j1GUIinputBox::j1GUIinputBox(char* text) 
{
	this->type = GUItype::GUI_INPUTBOX;	
	texture = App->gui->GetAtlasTexture();	
}

j1GUIinputBox::~j1GUIinputBox() {

}

bool j1GUIinputBox::Start()
{
	//Adding input box's string
	string = App->gui->AddGUIelement(GUItype::GUI_LABEL, this, globalPosition, { 0,10 }, true, enabled, { localPosition.x,localPosition.y,50,50 }, text);
	return true;
}

bool j1GUIinputBox::PreUpdate()
{	
	string->enabled = enabled;	

	//If it's focused, enable TEXT INPUT
	if (focus && enabled) 
		App->input->EnableTextInput();

	else if (!focus || !enabled)
		App->input->DisableTextInput();

	above = OnAbove();

	return true;
}

bool j1GUIinputBox::Update(float dt)
{
	//Setting texture of the string written
	if (enabled) 
	{
		if (App->input->GetText().Length()!=0) {
			App->tex->UnLoad(string->texture);
			string->texture = App->fonts->Print(App->input->GetText().GetString());
		}	
		else {
			App->tex->UnLoad(string->texture);
			string->texture = App->fonts->Print(" ");
		}
	}

	//Setting focus of input box
	if (above) 
	{
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			OnClick();
	}	
	else {
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			focus=false;
	}	

	return true;
}

bool j1GUIinputBox::PostUpdate()
{	
	if (enabled) {

		//Draws the rectangle to write in
		App->render->DrawQuad(rect,255,255,255,255,false, false);				

		//Draws the cursor(rectangle)
		if (focus)
		{
			SDL_Rect rect = { (string->globalPosition.x + App->input->GetCursorPosition()) * App->win->GetScale(), 
				(string->globalPosition.y + localPosition.y) * App->win->GetScale() , 2,  string->rect.h-4  };
			App->render->DrawQuad(rect, 255, 255, 255, 255, true, false);
		}
	}

	return true;
}

bool j1GUIinputBox::CleanUp()
{
	return true;
}

void j1GUIinputBox::OnClick()
{
	focus = true;	
}

