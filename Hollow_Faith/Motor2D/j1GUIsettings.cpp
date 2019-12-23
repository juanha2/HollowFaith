#include "j1App.h"
#include "j1GUIsettings.h"
#include "j1Input.h"
#include "j1GUI.h"


j1GUIsettings::j1GUIsettings() {

	this->type = GUItype::GUI_SETTINGS;
	//text = "SETTINGS";
	//texture = App->gui->GetAtlasTexture();
}

j1GUIsettings::~j1GUIsettings() {

}


bool j1GUIsettings::Awake(pugi::xml_node&)
{

	return true;
}

bool j1GUIsettings::Start()
{
	image = App->gui->AddGUIelement(GUItype::GUI_IMAGE, this, globalPosition, localPosition, false, false, rect);
	exit = App->gui->AddGUIelement(GUItype::GUI_BUTTON, this, { 260,260 }, localPosition, true, false, { 513,53,14,14 });
	button = App->gui->AddGUIelement(GUItype::GUI_BUTTON, this, { 50,200 }, { 0,0 }, true, true, { 4,69,130,37 }, text);
	label = App->gui->AddGUIelement(GUItype::GUI_LABEL, this, { 100,250 }, { 0,0 }, true, false, { 4,69,130,37 }, text);
	return true;
}

bool j1GUIsettings::PreUpdate()
{
	if (button->focus) {
		image->enabled = true;
		exit->enabled = true;
		label->enabled = true;
		button->focus = false;
	}

	if (exit->focus) {
		image->enabled = false;
		exit->enabled = false;
		label->enabled = false;
		exit->focus = false;
	}
	
	above = OnAbove();

	return true;
}

bool j1GUIsettings::Update(float dt)
{

	if (above)
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			OnClick();


	return true;
}

bool j1GUIsettings::PostUpdate()
{

	if(texture!=nullptr)
		Draw();

	return true;
}

bool j1GUIsettings::CleanUp()
{
	label = nullptr;
	image = nullptr;
	exit = nullptr;
	button = nullptr;
	return true;
}

void j1GUIsettings::OnClick()
{
	focus = true;
	image->enabled = true;
	exit->enabled = true;
}

void j1GUIsettings::OnRelease()
{

}