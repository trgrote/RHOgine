#ifndef TILE_SYSTEM_H
#define TILE_SYSTEM_H

#include <vector>
using std::vector;

#include <list>
using std::list;

#include "TextureManager.h"

#include <json/json.h>
#include <json/config.h>

#include "Camera.h"

#include "../HashString.h"

namespace rho
{

/// Integer Value Identifiying a unique tile in a tile set
typedef unsigned long TileID;

/// 2-D Array of Tile IDs, not really
typedef std::vector< TileID > TileGrid;

/**
  *	\brief Information needed by TileLayer to load the desired tile layer
  */
struct TileLayerInfo
{
	/// The Width of Each Tile
	unsigned int tileWidth;
	/// The Height of Each Tile
	unsigned int tileHeight;
	
	/// Structure to hold Info for Each tile set used in this layer/map
	struct TileSetInfo_
	{
		unsigned int firstId;
		std::string textureName;
		unsigned int spacing;
	};
	
	/// Dynamic Array of all the TileSets Available for this Layer
	std::vector< TileSetInfo_ > tileSets;
	
	//// Map Information
	
	unsigned int mapTileWidth;		// Number of tiles wide the map is
	unsigned int mapTileHeight;		// Number of tiles High the map is

	TileGrid mapGrid;		// Map Grid
};

bool loadTileLayerFromJSON( std::string json_file, TileLayerInfo & layer_info );

/**
  *	\brief Drawable Tile Layer.
  *	Has it's own Tileset, and tile map coordinates.
  */
class TileLayer : public sf::Drawable
{
private:
	/// Tile Layer Information this Tile Layer Uses
	TileLayerInfo m_tileLayerInfo;
	
	/// Sprite for each tile in the tile set.
	std::vector< sf::Sprite > m_drawingTiles;

	/// Texture of the entire tile set
	rho::pTexture m_tileTexture;
	
	/// List of all the Tile Set Textures
	std::list< rho::pTexture > m_tileTextures;

	/// The Width of the current tile cache
	int m_cacheWidth;

	/// The Height of the current tile cache
	int m_cacheHeight;

	/// Cache of all the currently drawn Spirtes
	/// copied from m_drawingTiles
	std::list< sf::Sprite > m_cache;
	
	/// The Max Tile ID this tile layer can use
	/// used by tileExists to ensure tile on map is drawable
	/// \sa tileExists
	TileID m_miMaxTileID;

	/// Construct the m_drawingTiles Sprites
	void buildSpritesFromTileSet( rho::TextureManager & texture_manager );

	/// Removes Flip Information from Tile ID so it can be used to determine real tile id
	static TileID removeFlipped( TileID tile_id );	
	
	/// Returns true if Tile on map is a flipped tile
	static bool isHorizFlipped( TileID const & tile_id );
	
	/// Returns true if Tile on map is a flipped tile
	static bool isVertFlipped( TileID const & tile_id );
	
	/// Returns true if Tile on map is a flipped tile
	static bool isDiagFlipped( TileID const & tile_id );
	
public:
	/**
	  *	\brief Ctor 
	  *	\arg camera Camera which we will base our screen culling off
	  *	\arg layer_info All the information needed for this Tile Layer
	  *	\arg texture_manager Texture Manager so Layer can load all 
	  *		needed tile textures. 
	  *	\note Does not hold Reference to this TextureManager
	  */
	TileLayer( 	rho::Camera const & camera,
				rho::TileLayerInfo const & layer_info,
				rho::TextureManager & texture_manager );
				
	/// Destructor
	virtual ~TileLayer();

	/**
	  * \brief Return the Tile ID of the tile at that position in the tile layer.
	  */
	TileID getTileAt( int const & column, int const & row ) const;
	
	/**
	  *	\brief Returns true if a tile exists at the position in the tile layer.
	  */
	bool isTileAt( unsigned int const & column, unsigned int const & row ) const;
	
	/**
	  *	\brief Returns true if a tile exists with that tile id in the tile set.
	  */
	bool tileExists( TileID const & tile_id ) const;
	
	/**
	  *	\brief Event Handler to respond to the Camera moving.
	  *	\arg camera This Tile layer's Camera.
	  */
	void OnCameraMoved( rho::Camera const & camera );

	/**
	  *	\brief Event Handler to respond to the Camera Resizing.
	  *	\arg camera This Tile layer's Camera.
	  */
	void OnCameraResized( rho::Camera const & camera );

	/**
	  *	\brief Draw the Tile Layer.
	  */
	virtual void draw( sf::RenderTarget &target, sf::RenderStates states ) const;
};

}

#endif
