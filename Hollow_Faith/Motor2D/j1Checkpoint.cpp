#include "j1Checkpoint.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1App.h"
#include "p2Point.h"

j1Checkpoint::j1Checkpoint() {

	name.create("checkpoint");
}

j1Checkpoint::~j1Checkpoint() {}

bool j1Checkpoint::Awake(pugi::xml_node&) {

	save_game = "checkpoint.xml";
	load_game = "checkpoint.xml";

	return true;
}
bool j1Checkpoint::Start()
{
	if (checkpoint)
		LoadCheckPoints();

	return true;
}

void j1Checkpoint::SaveCheckPoints() {

	save_checkpoints = true;	//We set this bool to true to let entities know if they have to save from save_game.xml or chekpoint.xml

	bool ret = true;

	LOG("Saving CHECKPOINTS data to %s...", save_game.GetString());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	p2SString enemyLand = "EnemyLand";
	p2SString enemyFly = "EnemyFly";

	p2List_item<j1Entity*>* entities = App->objects->Entities.start;

	while (entities != NULL && ret == true)
	{	
		ret = entities->data->Save(root);	//We call save method of each entity	

		entities = entities->next;
	}

	if (ret == true)
	{
		data.save_file("checkpoint.xml"); //checkpoint.GetString()
		LOG("... finished saving CHECKPOINTS state", );
	}
	else
		LOG("Couldn't save succesful CHECKPOINTS data");

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
		LOG("Loading CHEKPOITNS state from %s...", load_game.GetString());

		root = data.child("game_state");		
		App->objects->Load(root);		//We call load method of each entity	
			
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.GetString(), result.description());

	data.reset();
}

bool j1Checkpoint::CleanUp() { return true; }

