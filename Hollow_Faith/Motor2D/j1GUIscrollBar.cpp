#include "j1GUIscrollBar.h"


j1GUIscrollBar::j1GUIscrollBar() {

	this->type = GUItype::GUI_SCROLLBAR;

}

j1GUIscrollBar::~j1GUIscrollBar() {

}


bool j1GUIscrollBar::Awake(pugi::xml_node&)
{

	return true;
}


bool j1GUIscrollBar::PreUpdate()
{


	return true;
}

bool j1GUIscrollBar::Update(float dt)
{
	return true;
}

bool j1GUIscrollBar::PostUpdate()
{

	Draw();

	return true;
}

bool j1GUIscrollBar::CleanUp()
{
	return true;
}

void j1GUIscrollBar::OnClick()
{

}

void j1GUIscrollBar::OnRelease()
{

}