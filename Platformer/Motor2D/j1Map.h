#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Collision.h"

struct Properties{

	p2SString name;
	p2SString value;	
};
struct Levels
{
	p2SString name;
};

struct MapLayer
{
	p2SString name;
	uint width;
	uint height;
	uint* gid = nullptr;
	Properties	properties;
	float speed_x;

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
	SDL_Texture* texture = nullptr;
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
	int			x;
	int			y;
	uint		width;
	uint		height;
	Collider*	colliders[100] = { nullptr };
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
	p2List<TileSet*>			tilesets;
	p2List<MapLayer*>			layers;
	p2List<ObjectsGroup*>		objectgroups;
	p2List<Levels*>				levels;		
	p2List<Properties*>			properties;

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

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadObjects(pugi::xml_node& node, ObjectsGroup* group);
	bool LoadProperties(pugi::xml_node& node, Properties* property);
	bool CreateColliders(ObjectsData* data, int i);
	TileSet* GetTileset(int id);
	void log_properties();



public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	uint				window_width;
	uint				window_height;
	int					scale;
	
};

#endif // __j1MAP_H__