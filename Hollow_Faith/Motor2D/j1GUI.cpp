#include "j1GUI.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"

#include "j1GUIbutton.h"


j1GUI::j1GUI() : j1Module()
{
	name.create("gui");
}

// Destructor
j1GUI::~j1GUI()
{}


bool j1GUI::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlasFile = conf.child("atlas").attribute("file").as_string("");

	return ret;
}


bool j1GUI::Start()
{
	atlasTexture = App->tex->Load(atlasFile.GetString());

	return true;
}


bool j1GUI::PreUpdate()
{
	
	return true;
}


bool j1GUI::PostUpdate()
{
	return true;
}


bool j1GUI::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}


SDL_Texture* j1GUI::GetAtlasTexture() const
{
	return atlasTexture;
}


j1GUIelement* j1GUI::AddGUIelement(j1GUIelement::GUItype type, fPoint position, bool active)
{
	j1GUIelement* tmp;

	switch (type)
	{
	case j1GUIelement::GUItype::GUI_BUTTON:
		//tmp = new j1GUIButton();
			break;

	}


	return nullptr;
}

bool j1GUI::Save(pugi::xml_node& file) const {

	return true;
}


bool j1GUI::Load(pugi::xml_node& file) {

	return true;
}