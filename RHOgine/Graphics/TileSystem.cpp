#include "TileSystem.h"

#include <cmath>

#include <iostream>
#include <fstream>

#include "../defines.h"

// Bits on the far end of the 32-bit global tile ID are used for tile flags
#define FLIPPED_HORIZONTALLY_FLAG 		0x80000000
#define FLIPPED_VERTICALLY_FLAG   		0x40000000
#define FLIPPED_DIAGONALLY_FLAG   		0x20000000

using namespace rho;


///////////////////////////////////////////////////////////////////////////////

bool rho::loadTileLayerFromJSON( std::string json_file, rho::TileLayerInfo & layer_info )
{
	Json::Value root;
	Json::Reader reader;
	std::fstream file_in( json_file, std::fstream::in );

	if ( !reader.parse( file_in, root ) )
	{
		std::cerr << "Failed to to load " << json_file << std::endl;
		std::cerr << reader.getFormatedErrorMessages() << std::endl;
		return false;
	}

	//// Load Map Dimensions

	// map width
	if ( root["width"].isInt() )
	{
		layer_info.mapTileWidth = root["width"].asUInt();
	}
	else
	{
		std::cerr << "Could not find width\n";
		return false;
	}

	// map height
	if ( root["height"].isInt() )
	{
		layer_info.mapTileHeight = root["height"].asUInt();
	}
	else
	{
		std::cerr << "Could not find height\n";
		return false;
	}

	// Create Vector Equal to the dimensions
	layer_info.mapGrid.resize( layer_info.mapTileWidth * layer_info.mapTileHeight, 0 );

	//// Get universal Tile Wdith
	// map width
	if ( root["tilewidth"].isInt() )
	{
		layer_info.tileWidth = root["tilewidth"].asUInt();
	}
	else
	{
		std::cerr << "Could not find tile width\n";
		return false;
	}

	// map height
	if ( root["tileheight"].isInt() )
	{
		layer_info.tileHeight = root["tileheight"].asUInt();
	}
	else
	{
		std::cerr << "Could not find tile height\n";
		return false;
	}

	//// Load up the tile set information
	if ( root[ "tilesets" ].isArray() )
	{
		Json::Value tile_sets = root[ "tilesets" ];

		if ( tile_sets < 1 )
		{
			std::cerr << "No Tilesets Given for Map\n";
			return false;
		}

		// Foreach TileSet in the value
		for ( Json::Value::ArrayIndex i = 0 ;
			  i < tile_sets.size() ;
			  ++i )
		{
			TileLayerInfo::TileSetInfo_ tile_set;

			Json::Value js_tile_set = tile_sets[ i ];	// Grab the current tile set

			// Texture Name
			if ( js_tile_set[ "image" ].isString() )
			{
				tile_set.textureName = js_tile_set[ "image" ].asString();
			}
			else
			{
				std::cerr << "Couldn't find textureName info\n";
				return false;
			}

			// First Tile ID
			if ( js_tile_set[ "firstgid" ].isInt() )
			{
				tile_set.firstId = js_tile_set[ "firstgid" ].asUInt();
			}
			else
			{
				std::cerr << "Couldn't find firstId info\n";
				return false;
			}

			// spacing
			if ( js_tile_set[ "spacing" ].isInt() )
			{
				tile_set.spacing = js_tile_set[ "spacing" ].asUInt();
			}
			else
			{
				std::cerr << "Couldn't find spacing info\n";
				return false;
			}

			// Add to Tile Set List
			layer_info.tileSets.push_back( tile_set );
		}
	}
	else
	{
		std::cerr << "Could not find tilesets\n";
		return false;
	}

	//// Load Map
	if ( root[ "layers" ].isArray() )
	{
		Json::Value layers = root[ "layers" ];

		// Load in order, overwrite the previous layer's entry
		for ( Json::Value::ArrayIndex i = 0 ;
			  i < layers.size() ;
			  ++i )
		{
			if ( ! layers[ i ][ "data" ].isArray() )
			{
				std::cerr << "Could not find data in a layer\n";
			}

			if ( ! layers[ i ][ "name" ].isString() )
			{
				std::cerr << "Couldn't Find layer name\n";
				return false;
			}

			// Ignore if name equals collision
			if ( layers[ i ][ "name" ] == "collision" || layers[ i ][ "name" ] == "Collision" )
			{
				std::cout << "Ignoring Collision Layer\n";
				continue;
			}

			// Make sure it's visible
			if ( layers[ i ].get( "visible", false ) == false )
			{
				std::cout << "Ignoring Invisible layer\n";
				continue;	// skip is visible is set to false
			}

			Json::Value data = layers[ i ][ "data" ];	// Grab data

			// Load the grid, for each layer
			for ( Json::Value::ArrayIndex data_index = 0;
			      data_index < data.size();
				  ++data_index )
			{
				if ( !data[ data_index ].isInt() && !data[ data_index ].isUInt())
				{
					std::cerr << "Tile ID at " << data_index << " is invalid\n";
					return false;
				}

				// If entry is 0, ignore
				TileID tile = data[ data_index ].asUInt();

				if ( tile == 0 )
				{
					continue;
				}

				// Else overwrite the old value
				layer_info.mapGrid[ data_index ] = tile;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////

TileLayer::TileLayer( 	rho::Camera const & camera,
						rho::TileLayerInfo const & layer_info,
						rho::TextureManager & texture_manager  )
:	m_tileLayerInfo( layer_info )
{
	//std::cout << "TileLayer Created\n";

	// Based off the tiles in the given TileSet
	// Create all the sprites needed for shindigs
	buildSpritesFromTileSet(texture_manager);

	OnCameraResized( camera );
}

///////////////////////////////////////////////////////////////////////////////

TileLayer::~TileLayer()
{
}

///////////////////////////////////////////////////////////////////////////////

void TileLayer::buildSpritesFromTileSet( rho::TextureManager & texture_manager )
{
	m_miMaxTileID = 0;

	//// Foreach Tile set
	for ( TileLayerInfo::TileSetInfo_ const & tile_set : m_tileLayerInfo.tileSets )
	{
		// Load Texture
		rho::pTexture texture = texture_manager.getTexture( rho::HashString( tile_set.textureName ) );

		// Assert if the dimensions don't match
		sf::Vector2u size = texture->getSize();

		// Make Sure the texture matches the dimensions described in the tile set info
		assert_msg( ( size.x + tile_set.spacing ) % ( m_tileLayerInfo.tileWidth + tile_set.spacing ) == 0,
				   "Texture Given doesen't have correct width for given Tile Set info" );
		assert_msg( ( size.y + tile_set.spacing ) % ( m_tileLayerInfo.tileHeight + tile_set.spacing ) == 0,
				   "Texture Given doesen't have correct height for given Tile Set info" );

		int tiles_per_row = ( size.x + tile_set.spacing ) / ( m_tileLayerInfo.tileWidth + tile_set.spacing );
		int tiles_per_column = ( size.y + tile_set.spacing ) / ( m_tileLayerInfo.tileHeight + tile_set.spacing );

		// Section of the Texture to Draw
		sf::IntRect texel( 0, 0, m_tileLayerInfo.tileWidth, m_tileLayerInfo.tileHeight );

		// m_tileLayerInfo.tileCount = tiles_per_column * tiles_per_row;

		for ( int row = 0; row < tiles_per_column ; ++row )
		{
			for ( int column = 0; column < tiles_per_row; ++column )
			{
				TileID tile_index = row * tiles_per_row + column + tile_set.firstId;	// Tile ID + first id

				m_miMaxTileID = std::max( tile_index, m_miMaxTileID );

				texel.left = column * ( m_tileLayerInfo.tileWidth + tile_set.spacing );
				texel.top = row * ( m_tileLayerInfo.tileHeight + tile_set.spacing );

				// std::cout << "Tile Index at " << tile_index << std::endl;
				//std::cout << "Setting Texel position to " << texel.left << ", " << texel.top << std::endl;

				sf::Sprite tile_sprite( *texture.get(), texel );

				// m_drawingTiles[ tile_index ] = tile_sprite;
				m_drawingTiles.push_back( tile_sprite );	// Add the Sprite at the index found in the order on the sprite sheet
			}
		}

		// Add Texture to Collection
		m_tileTextures.push_back( texture );

	}	// foreach tile set in the layer info

//	std::cout << "Built Sprites\n";
}
///////////////////////////////////////////////////////////////////////////////

TileID TileLayer::removeFlipped( TileID tile_id )
{
	tile_id &= ~(FLIPPED_HORIZONTALLY_FLAG |
				 FLIPPED_VERTICALLY_FLAG |
                 FLIPPED_DIAGONALLY_FLAG);

	return tile_id;
}
///////////////////////////////////////////////////////////////////////////////

bool TileLayer::isHorizFlipped( TileID const & tile_id )
{
	return ( tile_id & FLIPPED_HORIZONTALLY_FLAG );
}

///////////////////////////////////////////////////////////////////////////////

bool TileLayer::isVertFlipped( TileID const & tile_id )
{
	return ( tile_id & FLIPPED_VERTICALLY_FLAG );
}

///////////////////////////////////////////////////////////////////////////////

bool TileLayer::isDiagFlipped( TileID const & tile_id )
{
	return ( tile_id & FLIPPED_DIAGONALLY_FLAG );
}

///////////////////////////////////////////////////////////////////////////////

TileID TileLayer::getTileAt( int const & column, int const & row ) const
{
	return m_tileLayerInfo.mapGrid[ column + row * m_tileLayerInfo.mapTileWidth ] - 1;
}

///////////////////////////////////////////////////////////////////////////////

bool TileLayer::isTileAt( unsigned int const & column, unsigned int const & row ) const
{
	return ( 0 <= column && column < m_tileLayerInfo.mapTileWidth ) &&
		   ( 0 <= row 	 && row    < m_tileLayerInfo.mapTileHeight );
}

///////////////////////////////////////////////////////////////////////////////

bool TileLayer::tileExists( TileID const & tile_id ) const
{
	return 0 <= tile_id && tile_id < m_miMaxTileID + 1; 	// return true if tile is within bounds of all available tiles
	// return true;
}

///////////////////////////////////////////////////////////////////////////////

void TileLayer::OnCameraMoved( rho::Camera const & camera )
{
	// Get the Rectangle of where the camera will display
	sf::FloatRect view_rect = camera.getRect();

	unsigned int tile_width = m_tileLayerInfo.tileWidth;
	unsigned int tile_height = m_tileLayerInfo.tileHeight;

	int left_most_tile = floor( view_rect.left / (float) tile_width );
	int top_most_tile = floor( view_rect.top / (float) tile_height );

	int right_most_tile = left_most_tile + m_cacheWidth;
	int bottom_most_tile = top_most_tile + m_cacheHeight;

	std::list< sf::Sprite >::iterator sprite_iter = m_cache.begin();

	//std::cout << "Should Make " << ( bottom_most_tile - top_most_tile ) * ( right_most_tile - left_most_tile ) << std::endl;

	for ( int row = top_most_tile ; row < bottom_most_tile && row < (int)m_tileLayerInfo.mapTileHeight ; ++row )
	{
		if ( row < 0 )
		{
			continue;
		}

        for ( int column = left_most_tile ; column < right_most_tile && column < (int)m_tileLayerInfo.mapTileWidth ; ++column )
		{
		    //std::cout << "Adding " << column << ", " << row << " to drawing spirtes\n";

		    //std::cout << "Location " << column * tile_width << ", " << row * tile_height << std::endl;

			if ( column < 0 )
			{
				continue;
			}

			// Check to make sure we are still have space on the list
			//std::cout << "Drawing Tile " << count << " " << column << ", " << row << std::endl;
			assert_msg( sprite_iter != m_cache.end(), "We have more tiles to draw than what was given to us in cache" );

            TileID index = getTileAt( column, row );

            //std::cout << "Index " << index << std::endl;
			// if ( index == 0 )
			// {
				// // Ignore 0 index tiles
				// continue;
			// }

			// assert_msg ( tileExists( index ), "Tile Requested Does not exist in tile set" );

			// Ignore if Tile Doesn't Exist
			if ( ! tileExists( removeFlipped( index ) ) )
			{
				continue;
			}

			float rotation = 0.0f;

			if ( isHorizFlipped( index ) && isDiagFlipped( index ) )
			{
				// std::cout << index;
				// std::cout << " Found Horiz Flipped\n";
				rotation = 90.0f;
			}

			if ( isHorizFlipped( index ) && isVertFlipped( index ) )
			{
				// std::cout << index;
				// std::cout << " Found Vert Flipped\n";
				rotation = 180.0f;
			}

			if ( isVertFlipped( index ) && isDiagFlipped( index ) )
			{
				// std::cout << index;
				// std::cout << " Found Diag Flipped\n";
				rotation = 270.0f;
			}

			index = removeFlipped( index );

			(*sprite_iter) = m_drawingTiles[ index ];	// copy the sprite to the value at this iter
			(*sprite_iter).setRotation( rotation );
			(*sprite_iter).setPosition( column * tile_width, row * tile_height );
			if ( rotation != 0.0f )
			{
				(*sprite_iter).setOrigin( tile_width / 2, tile_height / 2 );
				(*sprite_iter).setPosition( (*sprite_iter).getPosition().x + tile_width / 2,
					(*sprite_iter).getPosition().y + tile_height / 2 );
			}

			++sprite_iter;	// go to next position
		}
	}

	// After the loop
	// assert_msg( sprite_iter == m_cache.end(), "We didn't use all the sprites in the cache" );
}

///////////////////////////////////////////////////////////////////////////////

void TileLayer::OnCameraResized( rho::Camera const & camera )
{
	sf::FloatRect view_rect = camera.getRect();

	m_cacheWidth  = view_rect.width / m_tileLayerInfo.tileWidth + 2;		// Add 2 for potential overlap
	m_cacheHeight = view_rect.height / m_tileLayerInfo.tileHeight + 2;		// Add 2 for potential overlap

	// Recreate the ... list?
	// m_cache.clear();
	// m_cache.resize( m_cacheWidth * m_cacheHeight );

	unsigned int tile_width = m_tileLayerInfo.tileWidth;
	unsigned int tile_height = m_tileLayerInfo.tileHeight;

	int left_most_tile = floor( view_rect.left / (float) tile_width );
	int top_most_tile = floor( view_rect.top / (float) tile_height );

	int right_most_tile = left_most_tile + m_cacheWidth;
	int bottom_most_tile = top_most_tile + m_cacheHeight;

	//std::cout << "Should Make " << ( bottom_most_tile - top_most_tile ) * ( right_most_tile - left_most_tile ) << std::endl;

	sf::Sprite new_sprite;		// Sprite to hold the current sprite we are going to add to the cache

	for ( int row = top_most_tile ; row < bottom_most_tile && row < (int)m_tileLayerInfo.mapTileHeight ; ++row )
	{
		if ( row < 0 )
		{
			continue;
		}

        for ( int column = left_most_tile ; column < right_most_tile && column < (int)m_tileLayerInfo.mapTileWidth ; ++column )
		{
		    //std::cout << "Adding " << column << ", " << row << " to drawing spirtes\n";

		    //std::cout << "Location " << column * tile_width << ", " << row * tile_height << std::endl;

			if ( column < 0 )
			{
				continue;
			}

            TileID index = getTileAt( column, row );

			// Ignore if Tile Doesn't Exist
			if ( ! tileExists( removeFlipped( index ) ) )
			{
				continue;
			}

			float rotation = 0.0f;

			if ( isHorizFlipped( index ) && isDiagFlipped( index ) )
			{
				// std::cout << index;
				// std::cout << " Found Horiz Flipped\n";
				rotation = 90.0f;
			}

			if ( isHorizFlipped( index ) && isVertFlipped( index ) )
			{
				// std::cout << index;
				// std::cout << " Found Vert Flipped\n";
				rotation = 180.0f;
			}

			if ( isVertFlipped( index ) && isDiagFlipped( index ) )
			{
				// std::cout << index;
				// std::cout << " Found Diag Flipped\n";
				rotation = 270.0f;
			}

			index = removeFlipped( index );

			sf::Sprite new_sprite = m_drawingTiles[ index ];	// copy the sprite to the value at this iter
			new_sprite.setRotation( rotation );
			new_sprite.setPosition( column * tile_width, row * tile_height );
			if ( rotation != 0.0f )
			{
				new_sprite.setOrigin( tile_width / 2, tile_height / 2 );
				new_sprite.setPosition( new_sprite.getPosition().x + tile_width / 2,
					new_sprite.getPosition().y + tile_height / 2 );
			}

			// Push Back new sprite
			m_cache.push_back( new_sprite );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void TileLayer::draw( sf::RenderTarget &target, sf::RenderStates states ) const
{
	for ( sf::Sprite const & tile : m_cache )
	{
		// std::cout << "Drawing Spirte\n";
		target.draw( tile, states );
	}

	// std::cout << "Done Drawing\n";
}

/* Commented Code for future reference */
// rho::TileSetInfo rho::loadFromJSON( std::string json_file )
// {
	// Json::Value root;
	// Json::Reader reader;
	// rho::TileSetInfo rval;
	// std::fstream file_in( json_file, std::fstream::in );

	// if ( !reader.parse( file_in, root ) )
	// {
		// std::cerr << "Failed to to load " << json_file << std::endl;
		// return rval;
	// }

	// // std::string encoding = root.get("encoding", "UTF-8" ).asString();
	// for ( std::string const & member : root.getMemberNames() )
	// {
		// std::cout << member << std::endl;
	// }

	// // tile width
	// if ( root["tileWidth"].isInt() )
	// {
		// rval.tileWidth = root["tileWidth"].asInt();
	// }
	// else
	// {
		// std::cerr << "Could not find tileWidth\n";
	// }

	// // tile height
	// if ( root["tileHeight"].isInt() )
	// {
		// rval.tileHeight = root["tileHeight"].asInt();
	// }
	// else
	// {
		// std::cerr << "Could not find tileHeight\n";
	// }

	// // tile count
	// if ( root["tileCount"].isInt() )
	// {
		// rval.tileCount = root["tileCount"].asInt();
	// }
	// else
	// {
		// std::cerr << "Could not find tileCount\n";
	// }

	// // horizOffset
	// if ( root["horizOffset"].isInt() )
	// {
		// rval.horizOffset = root["horizOffset"].asInt();
	// }
	// else
	// {
		// std::cerr << "Could not find horizOffset\n";
	// }

	// // tile width
	// if ( root["vertOffset"].isUInt() )
	// {
		// rval.vertOffset = root["vertOffset"].asUInt();
	// }
	// else
	// {
		// std::cerr << "Could not find vertOffset\n";
	// }

	// // tileSheet
	// if ( root["tileSheet"].isString() )
	// {
		// rval.tileSheet = rho::HashString( root["tileSheet"].asString() );
	// }
	// else
	// {
		// std::cerr << "Could not find tileSheet\n";
	// }

	// return rval;
// }

// rho::TileMapInfo rho::loadTileMapFromJSON( std::string json_file )
// {
	// Json::Value root;
	// Json::Reader reader;
	// rho::TileMapInfo rval;
	// std::fstream file_in( json_file, std::fstream::in );

	// if ( !reader.parse( file_in, root ) )
	// {
		// std::cerr << "Failed to to load " << json_file << std::endl;
		// return rval;
	// }

	// // std::string encoding = root.get("encoding", "UTF-8" ).asString();
	// for ( std::string const & member : root.getMemberNames() )
	// {
		// std::cout << member << std::endl;
	// }

	// // tile width
	// if ( root["mapTileWidth"].isInt() )
	// {
		// rval.mapTileWidth = root["mapTileWidth"].asInt();
	// }
	// else
	// {
		// std::cerr << "Could not find mapTileWidth\n";
	// }

	// // tile height
	// if ( root["mapTileHeight"].isInt() )
	// {
		// rval.mapTileHeight = root["mapTileHeight"].asInt();
	// }
	// else
	// {
		// std::cerr << "Could not find mapTileHeight\n";
	// }

	// // Read in Map
	// if ( root["mapGrid"].isArray() )
	// {
		// Json::Value const & map_grid = root["mapGrid"];

		// // std::cout << "Going to Make Grid Array\n";
		// // std::cout << map_grid.size() << std::endl;

		// for (   Json::Value::ArrayIndex index = 0;
                // index < map_grid.size();
                // ++index )
		// {
		    // // std::cout << index << std::endl;
			// TileID gid = map_grid[ index ].asUInt();

			// rval.mapGrid.push_back( gid );
		// }
	// }

	// return rval;
// }

// bool rho::loadTileSetMapFromJSON( std::string tiled_out, rho::TileSetInfo & set_info, rho::TileMapInfo & map_info )
// {
	// Json::Value root;
	// Json::Reader reader;
	// rho::TileMapInfo rval;
	// std::fstream file_in( tiled_out, std::fstream::in );

	// if ( !reader.parse( file_in, root ) )
	// {
		// std::cerr << "Failed to to load " << tiled_out << std::endl;
		// return false;
	// }

	// // Read in Tile Set Information
	// if ( root[ "tilesets" ].isArray() )
	// {
		// Json::Value tile_sets = root[ "tilesets" ];

		// if ( tile_sets.size() == 0 )
		// {
			// std::cerr << "No Tile sets found\n";
			// return false;
		// }

		// if ( tile_sets.size() > 1 )
		// {
			// std::cerr << "More than one tile set detected, no more than one layer is supported\n";
		// }

		// // Grab the first one
		// Json::Value tile_set = tile_sets[Json::Value::ArrayIndex(0)];

		// // file name
		// if ( tile_set[ "image" ].isString() )
		// {
			// set_info.tileSheet = rho::HashString( tile_set[ "image" ].asString() );
		// }
		// else
		// {
			// std::cerr << "Could not find tile set name\n";
			// return false;
		// }

		// // tile width
		// if ( tile_set[ "tilewidth" ].isInt() )
		// {
			// set_info.tileWidth = tile_set[ "tilewidth" ].asInt();
		// }
		// else
		// {
			// std::cerr << "Could not find tile width\n";
			// return false;
		// }

		// // tile height
		// if ( tile_set[ "tileheight" ].isInt() )
		// {
			// set_info.tileHeight = tile_set[ "tileheight" ].asInt();
		// }
		// else
		// {
			// std::cerr << "Could not find tile height\n";
			// return false;
		// }
	// }
	// else
	// {
		// std::cerr << "Could not find tile set information\n";
	// }

	// // We don't set offset
	// set_info.horizOffset = 0;
	// set_info.vertOffset = 0;
	// set_info.tileCount = 0;

	// // Read in Map Information

	// // map width
	// if ( root["width"].isInt() )
	// {
		// map_info.mapTileWidth = root["width"].asInt();
	// }
	// else
	// {
		// std::cerr << "Could not find width\n";
		// return false;
	// }

	// // map height
	// if ( root["height"].isInt() )
	// {
		// map_info.mapTileHeight = root["height"].asInt();
	// }
	// else
	// {
		// std::cerr << "Could not find height\n";
		// return false;
	// }

	// // Read in Map
	// if ( ! root["layers"].isArray() )
	// {
		// std::cerr << "Could not find layers attribute\n";
		// return false;
	// }

	// Json::Value layers = root["layers"];

	// if ( layers.size() == 0 )
	// {
		// std::cerr << "No Layers found\n";
		// return false;
	// }

	// if ( layers.size() > 1 )
	// {
		// std::cerr << "More than one layer detected, no more than one layer is supported\n";
	// }

	// if ( layers[Json::Value::ArrayIndex(0)]["data"].isArray() )	// Grab only the first layer
	// {
		// Json::Value const & map_grid = layers[Json::Value::ArrayIndex(0)]["data"];

		// // std::cout << "Going to Make Grid Array\n";
		// // std::cout << map_grid.size() << std::endl;

		// for (   Json::Value::ArrayIndex index = 0;
                // index < map_grid.size();
                // ++index )
		// {
		    // // std::cout << index << std::endl;
			// map_info.mapGrid.push_back( map_grid[ index ].asUInt() );
		// }
	// }
	// else
	// {
		// std::cerr << "Could not find data\n";
		// return false;
	// }

	// return true;
// }
