#include "j1GUIimage.h"
#include "j1App.h"
#include "j1Render.h"

j1GUIimage::j1GUIimage() {

	this->type = GUItype::GUI_IMAGE;
	texture = App->gui->GetAtlasTexture();
}

j1GUIimage::~j1GUIimage() {

}


bool j1GUIimage::PostUpdate()
{	
	//If parent is console, draw quad instead of texture
	if (parent && enabled && parent->type == GUItype::GUI_CONSOLE)
		App->render->DrawQuad(rect, 0, 0, 0, 255, true, false);

	else if(enabled)
		Draw();

	return true;
}

bool j1GUIimage::CleanUp()
{
	return true;
}

