#ifndef __J1GUIINPUTBOX__
#define __J1GUIINPUTBOX__

#include "j1Module.h"
#include"j1GUIelement.h"


class j1GUIinputBox : public j1GUIelement
{
public:

	j1GUIinputBox();
	~j1GUIinputBox();


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






#endif // !__J1GUIINPUTBOX__

