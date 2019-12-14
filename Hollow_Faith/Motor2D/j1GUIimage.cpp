#include "j1GUIimage.h"
#include "j1App.h"


j1GUIimage::j1GUIimage() {

	this->type = GUItype::GUI_IMAGE;
}

j1GUIimage::~j1GUIimage() {

}


bool j1GUIimage::Awake(pugi::xml_node&)
{

	return true;
}


bool j1GUIimage::PostUpdate()
{	
	Draw();
	return true;
}

bool j1GUIimage::CleanUp()
{
	return true;
}

