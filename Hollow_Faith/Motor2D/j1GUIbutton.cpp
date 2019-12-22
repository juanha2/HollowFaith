#include "j1App.h"
#include "j1GUIButton.h"
#include "j1Input.h"
#include "j1GUI.h"


j1GUIButton::j1GUIButton() {

	this->type = GUItype::GUI_BUTTON;
	texture = App->gui->GetAtlasTexture();	
}

j1GUIButton::~j1GUIButton() {

}


bool j1GUIButton::Awake(pugi::xml_node&)
{

	return true;
}

bool j1GUIButton::Start()
{
	if(text!=nullptr)
		label = App->gui->AddGUIelement(GUItype::GUI_LABEL, this, globalPosition, localPosition, true, true, { 0,0,0,0 }, text);

	return true;
}

bool j1GUIButton::PreUpdate()
{

	above = OnAbove();

	return true;
}

bool j1GUIButton::Update(float dt)
{

	if (above)
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			OnClick();


	return true;
}

bool j1GUIButton::PostUpdate()
{
	
	Draw();

	return true;
}

bool j1GUIButton::CleanUp()
{
	return true;
}

void j1GUIButton::OnClick()
{
	focus = true;
}

void j1GUIButton::OnRelease()
{

}