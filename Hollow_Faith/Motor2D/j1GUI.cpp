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


bool j1GUI::Awake(pugi::xml_node& config)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	//atlasFile = config.child("atlas").attribute("file").as_string("");
	atlasFile = ("Assets/GUI/button.png");

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

	return true;
}


SDL_Texture* j1GUI::GetAtlasTexture() const
{
	return atlasTexture;
}


j1GUIelement* j1GUI::AddGUIelement(GUItype type, j1GUIelement* parent, iPoint globalPosition, iPoint localPosition, bool interactable, bool enabled, SDL_Rect section)
{
	j1GUIelement* tmp = nullptr;

	switch (type)
	{

	case GUItype::GUI_BUTTON:
		tmp = new j1GUIButton();
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


		GUIelementList.add(tmp)->data->Start();
	}


	return nullptr;
}

bool j1GUI::Save(pugi::xml_node& file) const {

	return true;
}


bool j1GUI::Load(pugi::xml_node& file) {

	return true;
}