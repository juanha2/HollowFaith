#ifndef __j1GUI_H__
#define __j1GUI_H__


#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"

#include "j1GUIelement.h"


class j1Entity;

class j1GUI : public j1Module
{

public:

	j1GUI();
	~j1GUI();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	bool Draw(float dt);

	j1GUIelement* AddGUIelement(j1GUIelement::GUItype type, fPoint position, bool active = false);



};



#endif //  __j1GUI_H__
