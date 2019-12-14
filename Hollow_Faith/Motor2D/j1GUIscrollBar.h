#ifndef __J1GUISCROLLBAR__
#define __J1GUISCROLLBAR__

#include "j1Module.h"
#include"j1GUIelement.h"


class j1GUIscrollBar : public j1GUIelement
{
public:

	j1GUIscrollBar();
	~j1GUIscrollBar();


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






#endif // !__J1GUISCROLLBAR__
