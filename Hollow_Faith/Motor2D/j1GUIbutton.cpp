#include "j1App.h"
#include "j1GUIButton.h"
#include "j1Input.h"
#include "j1EntityManager.h"


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

	// Check if player is dead to prevent bugs with Fade to Black and dt
	if(App->objects->player->dead == false)
		App->pause = !App->pause;


}

void j1GUIButton::OnRelease()
{

}