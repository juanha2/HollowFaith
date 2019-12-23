#ifndef __J1GUISETTINGS__
#define __J1GUISETTINGS__

#include "j1Module.h"
#include"j1GUIelement.h"


class j1GUIsettings : public j1GUIelement
{
public:

	j1GUIsettings();
	~j1GUIsettings();


	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();



	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };


	void OnClick();
	void OnRelease();

private:

	j1GUIelement* label = nullptr;
	j1GUIelement* image = nullptr;
	j1GUIelement* exit = nullptr;
	j1GUIelement* button = nullptr;
};






#endif // ! __J1GUISETTINGS__
