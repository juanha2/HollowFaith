#ifndef __J1GUIBUTTON__
#define __J1GUIBUTTON__

#include "j1Module.h"
#include"j1GUIelement.h"


class j1GUIButton : public j1GUIelement
{
public:

	j1GUIButton();
	~j1GUIButton();


	bool Awake(pugi::xml_node&);

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();



	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };


	void OnClick();
	void OnRelease();
};






#endif // !__J1GUIBUTTON__
