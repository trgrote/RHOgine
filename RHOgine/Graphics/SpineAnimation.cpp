#include "SpineAnimation.h"

#include <json/json.h>
#include <fstream>

using namespace rho;
using namespace spine;

///////////////////////////////////////////////////////////////////////////////

bool SpineManager::getMixInfoFromJSON( char const * json_file, SpineManager::MixInfoArray & mix_array )
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

	if ( root["mixes"].isArray() )
    {
        // Foreach entry
        Json::Value mixes = root[ "mixes" ];

		if ( mixes < 1 )
		{
			std::cerr << json_file << " - WARN: No Mixes Given Found\n";
			return true;
		}

		for ( Json::Value::ArrayIndex i = 0 ;
			i < mixes.size() ;
			++i )
		{
            if ( mixes[ i ].isObject() )	// Grab the current mix
            {
				Json::Value mix_obj = mixes[ i ];

                if ( mix_obj[ "from" ].isString() &&
					 mix_obj[ "to" ].isString() &&
					 mix_obj[ "duration" ].isDouble() )
				{
					Json::Value from		= mix_obj[ "from" ];
					Json::Value to			= mix_obj[ "to" ];
					Json::Value duration	= mix_obj[ "duration" ];

					mix_array.push_back(
						MixInfo(
							from.asCString(),
							to.asCString(),
							(float) duration.asDouble()
						)
					);
				}
				else
				{
					std::cerr << json_file << " - Couldn't find Mix Info from Mix Index: " << i << std::endl;
					continue;
				}
            }
            else
            {
                std::cerr << json_file << " - Not Given String for Script Name, WTF am I supposed to do with that?!\n";
                return false;
            }
		}
    }
    else
    {
        std::cerr << json_file << " - Could not find mixes, improper format, ya butthead\n";
        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////

Atlas * SpineManager::getAtlas( char const * atlas_file )
{
	auto iter = m_atlasMap.find( HashString( atlas_file ) );

	if ( iter != m_atlasMap.end() )
	{
		return (*iter).second.get();
	}

	Atlas * atlas = Atlas_readAtlasFile( atlas_file );  // read in the atlas file
	if ( !atlas )
	{
		return NULL;
	}

	m_atlasMap[ HashString( atlas_file ) ] = pAtlasData( atlas );		// Save for the future

	return atlas;
}

///////////////////////////////////////////////////////////////////////////////

SkeletonData * SpineManager::getSkeletonData(
	Atlas * atlas,
	char const * skeleton_file )
{
	auto iter = m_skelDataMap.find( HashString( skeleton_file ) );

	if ( iter != m_skelDataMap.end() )
	{
		return (*iter).second.get();
	}

	SkeletonJson * json = SkeletonJson_create( atlas ); // Create Skeleton json based off Atlas file
	if ( ! json )
	{
		return NULL;
	}

	SkeletonData * skeletonData = SkeletonJson_readSkeletonDataFile( json, skeleton_file ); // read more skeleton stuff?
	if ( !skeletonData )
	{
		return NULL;
	}

	SkeletonJson_dispose(json);

	m_skelDataMap[ HashString( skeleton_file ) ] = pSkeletonData( skeletonData );		// Save for the future

    return skeletonData;
}

///////////////////////////////////////////////////////////////////////////////

bool SpineManager::getMixInfo(
	char const * mix_file,
	SpineManager::MixInfoArray & mix_array )
{
    HashString mix_file_hs( mix_file );
	MixInfoMap::iterator iter = m_mixInfoMap.find( mix_file_hs );

	if ( iter != m_mixInfoMap.end() )
	{
		mix_array = m_mixInfoMap[ mix_file_hs ];
		return true;
	}

	if ( getMixInfoFromJSON( mix_file, mix_array ) )
	{
		m_mixInfoMap[ mix_file_hs ] = mix_array;
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////

AnimationStateData * SpineManager::getMixData(
	SkeletonData * skeletonData,
	char const * mix_file )
{
	// Load mix info
	SpineManager::MixInfoArray mix_array;
	AnimationStateData * stateData = NULL;

	if ( getMixInfo( mix_file, mix_array ) )
    {
        stateData = AnimationStateData_create(skeletonData);

		for ( MixInfo const & mix_info : mix_array )
		{
			spAnimation * from = SkeletonData_findAnimation(skeletonData, mix_info.from.getString().c_str() );
			spAnimation * to = SkeletonData_findAnimation(skeletonData, mix_info.to.getString().c_str() );

			if ( from == NULL )
			{
				std::cerr << "Loading: " << mix_file
					<< ", Could not find animation named "
					<< mix_info.from.getString() << std::endl;

				continue;
			}

			if ( to == NULL )
			{
				std::cerr << "Loading: " << mix_file
					<< ", Could not find animation named "
					<< mix_info.to.getString() << std::endl;

				continue;
			}

			// set mix info, ya doofus
			AnimationStateData_setMix(
				stateData,
				from,
				to,
				mix_info.duration );
		}
    }
    else
    {
        return NULL;
    }

	return stateData;
}

///////////////////////////////////////////////////////////////////////////////

spine::SkeletonDrawable * SpineManager::createSkeletonDrawable(
	char const * atlas_file,
	char const * skeleton_file,
	char const * mix_file )
{
	Atlas * atlas = getAtlas( atlas_file );
	if ( atlas == NULL )
	{
		return NULL;
	}

	SkeletonData * skeletonData = getSkeletonData( atlas, skeleton_file ); // read more skeleton stuff?
	if ( !skeletonData )
	{
		return NULL;
	}

	// Load mix info
	SpineManager::MixInfoArray mix_array;
	AnimationStateData * stateData = getMixData( skeletonData, mix_file );
	
	if ( stateData == NULL )
    {
        return NULL;
    }	

	return new SkeletonDrawable(skeletonData, stateData );
}

///////////////////////////////////////////////////////////////////////////////

spine::SkeletonDrawable * SpineManager::createSkeletonDrawable(
	char const * atlas_file,
	char const * skeleton_file )
{
	Atlas * atlas = getAtlas( atlas_file );
	if ( atlas == NULL )
	{
		return NULL;
	}

	SkeletonData * skeletonData = getSkeletonData( atlas, skeleton_file ); // read more skeleton stuff?
	if ( !skeletonData )
	{
		return NULL;
	}
	
	// The reason we need to give it an empty animationstate data is because, without it, 
	// trying to find animations by name will crash the system, It's dumb and I'm trying to see
	// if that's intentionaly by posting a bug on the GitHubs, but GitHub is like a neckbeard
	// black forest, I don't understand fucking anything on there.
	AnimationStateData * stateData = AnimationStateData_create(skeletonData);
	
	if ( stateData == NULL )
    {
        return NULL;
    }	

	return new SkeletonDrawable(skeletonData, stateData );
}

///////////////////////////////////////////////////////////////////////////////
