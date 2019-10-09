#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	
	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;	

	// TODO 5: Prepare the loop to iterate all the tiles in a layer

	for (p2List_item<MapLayer*>* layer = data.layers.start; layer != NULL; layer = layer->next) {
		
		for (int x = 0; x < data.width; x++)
		{
			for (int y = 0; y < data.height; y++)
			{
				int gid = layer->data->Get(x, y); //We get the gid from each tile
				
				if (gid != 0) {

					TileSet* tileset = GetTileset(gid);
					
					if (tileset != nullptr) {
						
						SDL_Rect rect = tileset->GetRect(gid);
						iPoint vec = MapToWorld(x, y);
					
						App->render->Blit(tileset->texture, vec.x, vec.y, &rect, layer->data->speed_x);

					}
				}
			}
		}
	}		
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();
	
	// TODO 2: clean up all layer data
	// Remove all layers
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set_tileset = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set_tileset);
		}
		
		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set_tileset);
		}
				
		data.tilesets.add(set_tileset);
	}

	// Load all layers info ----------------------------------------------
	pugi::xml_node layernode;

	for (layernode = map_file.child("map").child("layer"); layernode && ret; layernode = layernode.next_sibling("layer"))
	{
		MapLayer* set_layer = new MapLayer();
				
		if (ret == true)
		{
			ret = LoadLayer(layernode, set_layer);
		}

		
		data.layers.add(set_layer);
	}

	// Load all objects info ----------------------------------------------
	pugi::xml_node object_group;
	for (object_group = map_file.child("map").child("objectgroup"); object_group && ret; object_group = object_group.next_sibling("objectgroup"))
	{
		ObjectsGroup* set_object = new ObjectsGroup();

		if (ret == true)
		{
			ret = LoadObjects(object_group, set_object);
		}
		data.objectgroups.add(set_object);
	}

	// LOG properties ----------------------------------------------
	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		log_properties();
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());
		
		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;				
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();	
	
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	  	pugi::xml_node image = tileset_node.child("image");

		if (image == NULL)
		{
			LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
			ret = false;
		}
		else
		{			
			set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
			int w, h;
			
			SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
			set->tex_width = image.attribute("width").as_int();

			if (set->tex_width <= 0)
			{
				set->tex_width = w;
			}

			set->tex_height = image.attribute("height").as_int();

			if (set->tex_height <= 0)
			{
				set->tex_height = h;
			}

			set->num_tiles_width = set->tex_width / set->tile_width;
			set->num_tiles_height = set->tex_height / set->tile_height;
		}
		
	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret =true;
	
	layer->name.create(node.attribute("name").as_string());	
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	layer->gid = new uint[layer->width * layer->height];	
	layer->speed_x = node.child("properties").child("property").attribute("value").as_float();	

	memset(layer->gid, 0, (layer->width * layer->height)*sizeof(uint)); //Fill with zeros

	pugi::xml_node *iterate_gid = &node.child("data").child("tile");

	for (uint i = 0; i < (layer->width * layer->height); i++) 
	{
		layer->gid[i] = iterate_gid->attribute("gid").as_uint();
		iterate_gid = &iterate_gid->next_sibling("tile");
		
	}
	
	return ret;
}

SDL_Rect TileSet::GetRect(int id) {

	//LOG("%i", id);
	int id_ = id - firstgid;
	
	SDL_Rect Rect= { 0, 0, 0, 0 };

	Rect.w = tile_width;
	Rect.h = tile_height;
	
	int columns = id_ % num_tiles_width;
	int rows = id_ / num_tiles_width;

	Rect.x = margin + (columns*Rect.w) + (columns*spacing);
	Rect.y = margin + (rows * Rect.h) + (rows * spacing);

	return Rect;

}

TileSet* j1Map::GetTileset(int id) {

	p2List_item<TileSet*>* aux_tile = data.tilesets.start;

	while (aux_tile != NULL)
	{
		if (aux_tile->next == nullptr)
			return aux_tile->data;
		else
		{
			if (id >= aux_tile->data->firstgid && aux_tile->next->data->firstgid > id)
			{
				return aux_tile->data;
			}
		}
		aux_tile = aux_tile->next;
	}

	return nullptr;

}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0, 0);

	switch (data.type) {

	case MAPTYPE_UNKNOWN:
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
		break;
	case MAPTYPE_ORTHOGONAL:
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
		break;
	}


	return ret;
}


iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	switch (data.type) {

	case MAPTYPE_UNKNOWN:
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = (x / (data.tile_width / 2) + y / (data.tile_height / 2)) / 2;
		ret.y = (y / (data.tile_height / 2) - x / (data.tile_width / 2)) / 2;

		break;
	case MAPTYPE_ORTHOGONAL:
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
		break;

	}
	return ret;
}

bool j1Map::LoadObjects(pugi::xml_node& node, ObjectsGroup* group) {

	bool ret = true;

	group->name.create(node.attribute("name").as_string());		

	for (pugi::xml_node& object = node.child("object"); object && ret; object = object.next_sibling("object"))
	{
		ObjectsData* data = new ObjectsData;

		data->name = object.attribute("name").as_uint();
		data->x = object.attribute("x").as_uint();
		data->y = object.attribute("y").as_uint();
		data->height = object.attribute("height").as_uint();
		data->width = object.attribute("width").as_uint();

		CreateColliders(data); //We set a collider for each object we have

		group->objects.add(data);
	}

	return ret;
}

void j1Map::log_properties() {

	LOG("width: %d height: %d", data.width, data.height);
	LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

	p2List_item<TileSet*>* item_tileset = data.tilesets.start;
	while (item_tileset != NULL)
	{
		TileSet* s = item_tileset->data;
		LOG("Tileset ----");
		LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
		LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
		LOG("spacing: %d margin: %d", s->spacing, s->margin);
		item_tileset = item_tileset->next;
	}

	p2List_item<MapLayer*>* item_layer = data.layers.start;
	while (item_layer != NULL)
	{
		MapLayer* l = item_layer->data;
		LOG("Layer ----");
		LOG("name: %s", l->name.GetString());
		LOG("tile width: %d tile height: %d", l->width, l->height);
		LOG("parallax speed: %f", l->speed_x);
		item_layer = item_layer->next;
	}

	p2List_item<ObjectsGroup*>* item_group = data.objectgroups.start;
	while (item_group != NULL)
	{
		ObjectsGroup* g = item_group->data;
		LOG("Object Layer ----");
		LOG("name: %s", g->name.GetString());

		p2List_item<ObjectsData*>* item_object = g->objects.start;
		while (item_object != NULL)
		{
			ObjectsData* o = item_object->data;

			LOG("name: %d", o->name);
			LOG("x: %d ; y: %d", o->x, o->y);
			LOG("width: %d ; height: %d", o->width, o->height);
			item_object = item_object->next;
		}

		item_group = item_group->next;
	}
}

bool j1Map::CreateColliders(ObjectsData* data)
{
	bool ret = true;

	SDL_Rect collider;

	collider.x = data->x;
	collider.y = data->y;
	collider.h = data->height;
	collider.w = data->width;

	//Type of Collider
	if (data->name == 1)
		App->coll->AddCollider(collider, COLLIDER_PLAYER);

	if (data->name == 2)
		App->coll->AddCollider(collider, COLLIDER_NONE);

	return ret;
}