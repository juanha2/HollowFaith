#include "j1GUIinputBox.h"


j1GUIinputBox::j1GUIinputBox() {

	this->type = GUItype::GUI_INPUTBOX;

}

j1GUIinputBox::~j1GUIinputBox() {

}


bool j1GUIinputBox::Awake(pugi::xml_node&)
{

	return true;
}


bool j1GUIinputBox::PreUpdate()
{
	

	return true;
}

bool j1GUIinputBox::Update(float dt)
{
	return true;
}

bool j1GUIinputBox::PostUpdate()
{

	Draw();

	return true;
}

bool j1GUIinputBox::CleanUp()
{
	return true;
}

void j1GUIinputBox::OnClick()
{

}

void j1GUIinputBox::OnRelease()
{

}