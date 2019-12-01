#include "j1Checkpoint.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Enemy.h"
#include "j1Scene.h"
#include "p2Point.h"

j1Checkpoint::j1Checkpoint() {

	name.create("checkpoint");
}

j1Checkpoint::~j1Checkpoint() {


}

bool j1Checkpoint::Awake(pugi::xml_node&) {

	save_game = "checkpoint.xml";
	load_game = "checkpoint.xml";
	return true;
}

void j1Checkpoint::SaveCheckPoints() {

	save_checkpoints = true;
	bool ret = true;

	LOG("Saving Game CHECKPOINTS to %s...", save_game.GetString());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	p2SString enemyLand = "EnemyLand";
	p2SString enemyFly = "EnemyFly";

	p2List_item<j1Entity*>* entities = App->objects->Entities.start;

	while (entities != NULL && ret == true)
	{	
		ret = entities->data->Save(root);

		entities = entities->next;
	}

	if (ret == true)
	{
		data.save_file("checkpoint.xml"); //save_game.GetString()
		LOG("... finished saving", );
	}
	else
		LOG("NO");

	data.reset();
	save_checkpoints = false;
}



void j1Checkpoint::LoadCheckPoints()
{
	
	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.GetString()); //load_game.GetString()

	if (result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.GetString());

		root = data.child("game_state");

		App->objects->Load(root);		
		data.reset();
		
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.GetString(), result.description());
}

bool j1Checkpoint::CleanUp() {
		

	return true;
}