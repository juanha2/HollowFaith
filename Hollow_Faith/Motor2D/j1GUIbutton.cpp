#include "j1GUIButton.h"


j1GUIButton::j1GUIButton() {

	this->type = GUItype::GUI_BUTTON;

}

j1GUIButton::~j1GUIButton() {

}


bool j1GUIButton::Awake(pugi::xml_node&)
{

	return true;
}


bool j1GUIButton::PreUpdate()
{
	hovering = OnHover();

	return true;
}

bool j1GUIButton::Update(float dt)
{
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

}

void j1GUIButton::OnRelease()
{

}