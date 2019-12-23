#include "j1GUI.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"

#include "j1GUIbutton.h"
#include "j1GUIinputBox.h"
#include "j1GUIlabel.h"
#include "j1GUIimage.h"

#include "j1GUIscrollBar.h"
#include "j1GUIsettings.h"


j1GUI::j1GUI() : j1Module()
{
	name.create("gui");
}

// Destructor
j1GUI::~j1GUI()
{}


bool j1GUI::Awake(pugi::xml_node& config)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	//atlasFile = config.child("atlas").attribute("file").as_string("");
	atlasFile = ("Assets/GUI/atlas.png");

	return ret;
}


bool j1GUI::Start()
{
	atlasTexture = App->tex->Load(atlasFile.GetString());

	return true;
}


bool j1GUI::PreUpdate()
{
	
	bool ret = true;
	p2List_item<j1GUIelement*>* tmp = GUIelementList.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->PreUpdate();
		tmp = tmp->next;
	}

	return ret;

}


bool j1GUI::Update(float dt)
{

	bool ret = true;
	p2List_item<j1GUIelement*>* tmp = GUIelementList.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->Update(dt);
		tmp = tmp->next;
	}

	return ret;

}

bool j1GUI::PostUpdate()
{

	bool ret = true;

	p2List_item<j1GUIelement*>* tmp = GUIelementList.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->PostUpdate();
		tmp = tmp->next;
	}
	return ret;

}


bool j1GUI::CleanUp()
{
	LOG("Freeing GUI");
	
	for (p2List_item<j1GUIelement*>* item = GUIelementList.start; item; item = item->next)
	{
		item->data->CleanUp();
	}
	GUIelementList.clear();
	return true;
}


SDL_Texture* j1GUI::GetAtlasTexture() const
{
	return atlasTexture;
}


j1GUIelement* j1GUI::AddGUIelement(GUItype type, j1GUIelement* parent, iPoint globalPosition, iPoint localPosition, bool interactable, bool enabled, SDL_Rect section, char* text)
{
	j1GUIelement* tmp = nullptr;

	switch (type)
	{

	case GUItype::GUI_BUTTON:
		tmp = new j1GUIButton();
			break;
	case GUItype::GUI_INPUTBOX:
		tmp = new j1GUIinputBox();
		break;
	case GUItype::GUI_LABEL:
		tmp = new j1GUIlabel();
		break;
	case GUItype::GUI_IMAGE:
		tmp = new j1GUIimage();
		break;
	case GUItype::GUI_SCROLLBAR:
		tmp = new j1GUIscrollBar();
		break;
	case GUItype::GUI_SETTINGS:
		tmp = new j1GUIsettings();
		break;
	}

	if (tmp) 
	{

		tmp->parent = parent;
		tmp->globalPosition = globalPosition;
		tmp->localPosition = localPosition;
		tmp->interactable = interactable;
		tmp->enabled = enabled;
		tmp->rect = section;
		tmp->text = text;

		GUIelementList.add(tmp)->data->Start();
	}


	return tmp;
}

bool j1GUI::Save(pugi::xml_node& file) const {

	return true;
}


bool j1GUI::Load(pugi::xml_node& file) {

	return true;
}