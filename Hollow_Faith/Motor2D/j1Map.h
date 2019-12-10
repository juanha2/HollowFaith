#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Collision.h"

struct Properties
{
	struct Property
	{
		p2SString	name;
		float		value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	float Get(const char* name, float default_value = 0) const;

	p2List<Property*>    list;
};

struct Levels
{
	p2SString	name;
};

struct MapLayer
{

	p2SString	name;
	uint		width;
	uint		height;
	uint*		gid = nullptr;
	Properties	properties;
	float		speed_x;	
	
	MapLayer() : gid(NULL)
	{}

	~MapLayer()
	{
		RELEASE(gid);
	}

	inline uint Get(int x, int y) const {
		return gid[x + (y * width)];
	}
};

struct TileSet
{
	SDL_Rect GetRect(int id);

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture = nullptr;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

struct ObjectsData
{
	uint		name;
	float		x;
	float		y;
	int		width;
	int		height;
	
};

struct ObjectsGroup
{
	p2SString				name;
	p2List<ObjectsData*>	objects;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// ----------------------------------------------------
struct MapData
{
	int							width;
	int							height;
	int							tile_width;
	int							tile_height;
	SDL_Color					background_color;
	MapTypes					type;	
	p2SString					music;
	p2List<TileSet*>			tilesets;
	p2List<MapLayer*>			layers;
	p2List<ObjectsGroup*>		objectgroups;
	p2List<Levels*>				levels;		
	Properties					properties;
	
	uint numLevels;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	bool Reset();

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadObjects(pugi::xml_node& node, ObjectsGroup* group);
	bool LoadProperties(pugi::xml_node& node, Properties& property);
	bool CreateColliders(ObjectsData* data, int i);
	TileSet* GetTileset(int id) const;
	void log_properties();
	



public:

	MapData data;
	bool				map_loaded;

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	
	uint				window_width;
	uint				window_height;
	int					scale;
	
};

#endif // __j1MAP_H__