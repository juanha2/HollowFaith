#include "j1GUIButton.h"




bool j1GUIButton::Awake(pugi::xml_node&)
{
	this->type = GUItype::GUI_BUTTON;

	return true;
}

bool j1GUIButton::Start()
{
	return true;
}

bool j1GUIButton::PreUpdate()
{

	return true;
}

bool j1GUIButton::Update(float dt)
{
	return true;
}

bool j1GUIButton::PostUpdate()
{
	return true;
}

bool j1GUIButton::CleanUp()
{
	return true;
}

void j1GUIButton::OnClick()
{

}

void j1GUIButton::OnRelease()
{

}