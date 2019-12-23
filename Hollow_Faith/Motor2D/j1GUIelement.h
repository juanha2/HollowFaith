#ifndef __j1GUIELEMENT_H__
#define __j1GUIELEMENT_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "p2Log.h"
#include "SDL/include/SDL_render.h"

enum class GUItype
{
	GUI_ERROR,
	GUI_BUTTON,
	GUI_INPUTBOX,
	GUI_SCROLLBAR,
	GUI_LABEL,
	GUI_IMAGE,
	GUI_MAX
};


class j1GUIelement
{

public:

	j1GUIelement() { };
	~j1GUIelement();

	virtual bool Awake(pugi::xml_node& config) { return true; };
	virtual bool Start();
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };

	virtual bool CleanUp() { return true; };
	virtual bool Save(pugi::xml_node& file) const { return true; };
	virtual bool Load(pugi::xml_node& file) { return true; };

	bool OnAbove();
	virtual void OnClick();
	virtual void OnRelease();
	virtual void Dragging();
	void Draw();


public:

	j1GUIelement* parent = nullptr;
	GUItype type = GUItype::GUI_ERROR;
	iPoint globalPosition = { 0,0 };
	iPoint localPosition = { 0,0 };

	char* text = nullptr;
	bool enabled = false;
	bool interactable = false;
	bool above = false;

	SDL_Rect rect = { 0,0,0,0 };
	SDL_Texture* texture = nullptr;

	bool focus = false;

};

#endif // __j1GUIELEMENT_H__
