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
	if(text != nullptr)
		label = App->gui->AddGUIelement(GUItype::GUI_LABEL, this, { globalPosition.x + rect.w/4 -10,globalPosition.y+15 }, localPosition, true, true, { 0,0,0,0 }, text);

	return true;
}

bool j1GUIButton::PreUpdate()
{
	if (label != nullptr)
		label->enabled = enabled;

	above = OnAbove();

	return true;
}

bool j1GUIButton::Update(float dt)
{
	if (interactable) 
	{
		if (above) 
		{
			if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
				OnClick();

			if (App->input->GetMouseButtonDown(1) == KEY_REPEAT) 
			{		
				if(X_drag || Y_drag)
					dragging = true;

				iPoint mouseClick = { 0,0 };
				App->input->GetMousePosition(mouseClick.x, mouseClick.y);
				accuratedDrag = { mouseClick.x - (this->globalPosition.x), mouseClick.y - (this->globalPosition.y) };

			}

		}

		if (dragging) {

			if (App->input->GetMouseButtonDown(1) == KEY_IDLE || App->input->GetMouseButtonDown(1) == KEY_UP) 
				dragging = false;	
			else 
			{
				Dragging();
				MovingIt(dt);
			}
		}			
	}
	return true;
}

bool j1GUIButton::PostUpdate()
{
	if(enabled)
		Draw();

	return true;
}

bool j1GUIButton::CleanUp()
{
	return true;
}

void j1GUIButton::Dragging()
{

}

void j1GUIButton::MovingIt(float dt)
{

	iPoint MousePos = { 0,0 };
	App->input->GetMousePosition(MousePos.x, MousePos.y);

	iPoint currentPos =  this->globalPosition;


	if(X_drag)
		this->globalPosition.x += ((MousePos.x - this->globalPosition.x) - accuratedDrag.x);

	if(Y_drag)
		this->globalPosition.y += ((MousePos.y - this->globalPosition.y) - accuratedDrag.y);


	if (parent != nullptr)
	{
		if (X_drag)
			this->localPosition.x += currentPos.x - this->globalPosition.x;

		if (Y_drag)
			this->localPosition.y += currentPos.y - this->globalPosition.y;

		if (X_drag)
			this->globalPosition.x = parent->globalPosition.x - localPosition.x;

		if (Y_drag)
			this->globalPosition.y = parent->globalPosition.y - localPosition.y;
	}

}

void j1GUIButton::OnRelease()
{

}