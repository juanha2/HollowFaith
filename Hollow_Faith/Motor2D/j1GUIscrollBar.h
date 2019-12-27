#ifndef __J1GUISCROLLBAR__
#define __J1GUISCROLLBAR__

#include "j1Module.h"
#include "j1GUIelement.h"
#include "j1GUI.h"



class j1GUIscrollBar : public j1GUIelement
{
public:

	j1GUIscrollBar(SCROLL_TYPE type);
	~j1GUIscrollBar();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };
	void ScrollLimits();


	j1GUIelement* scrollButton = nullptr;
	SCROLL_TYPE scrollType = SCROLL_TYPE::SCROLL_NONE;

	float value;
	float initialValue;
};






#endif // !__J1GUISCROLLBAR__

