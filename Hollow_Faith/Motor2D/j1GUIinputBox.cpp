#include "j1GUIinputBox.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"


j1GUIinputBox::j1GUIinputBox() {

	this->type = GUItype::GUI_INPUTBOX;
	
	background = App->gui->AddGUIelement(GUItype::GUI_IMAGE, this, { 50,50 }, { 0,0 }, false, true, { 295,343,199,31 });
	text = App->gui->AddGUIelement(GUItype::GUI_LABEL, this, { 50,50 }, { 10,8 }, true, true, { 0,0,0,0 });
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