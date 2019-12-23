#include "j1GUIscrollBar.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Input.h"


j1GUIscrollBar::j1GUIscrollBar() {

	this->type = GUItype::GUI_SCROLLBAR;	
}

j1GUIscrollBar::~j1GUIscrollBar() {

}


bool j1GUIscrollBar::Awake(pugi::xml_node&)
{

	return true;
}


bool j1GUIscrollBar::Start()
{
	scrollButton = App->gui->AddGUIelement(GUItype::GUI_BUTTON, this, globalPosition, localPosition, true, true, { 599, 527, 9 , 10 });
	texture = App->gui->GetAtlasTexture();
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
	if(enabled)
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