#ifndef __j1GUIELEMENT_H__
#define __j1GUIELEMENT_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "p2Log.h"
#include "SDL/include/SDL_render.h"


class j1GUIelement
{

public:

	enum class GUItype
	{
		GUI_ERROR,
		GUI_BUTTON,
		GUI_MAX
	};

public:

	j1GUIelement(GUItype type);
	~j1GUIelement();

	virtual bool Awake(pugi::xml_node& config) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };

	virtual bool CleanUp() { return true; };
	virtual bool Save(pugi::xml_node& file) const { return true; };
	virtual bool Load(pugi::xml_node& file) { return true; };

	void Draw(float dt);


};

#endif // __j1GUIELEMENT_H__
