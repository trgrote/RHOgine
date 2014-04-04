#ifndef SPINE_ANIMATION_H
#define SPINE_ANIMATION_H

#include "../defines.h"
#include "../HashString.h"

#include <spine/spine-sfml.h>

#include <map>
#include <list>
#include <memory>
using std::unique_ptr;

namespace rho
{

/// \brief Manages all Spine Skeleton and Animation resources and, also, all SkeletonDrawbable Instances.
class SpineManager
{
protected:

	struct MixInfo
	{
		HashString to;
		HashString from;
		float duration;

		MixInfo( char const * arg_to, char const * arg_from, float const & arg_duration )
		:	to( arg_to ),
			from( arg_from ),
			duration( arg_duration )
		{
		}
	};

	typedef std::vector< MixInfo > MixInfoArray;

    static bool getMixInfoFromJSON( char const * json_file, SpineManager::MixInfoArray & mix_array );

	/// \brief Sorry for this, it's a class whose purpose is only to describe
	///	how to delete these two Spine Datatypes.
	class SpineDestructor
	{
	public:
		void operator()( SkeletonData * skel_data )
		{
			SkeletonData_dispose( skel_data );
		}
		void operator()( Atlas * atlas )
		{
			Atlas_dispose( atlas );
		}
		void operator()( AnimationStateData * animation_state_data )
		{
			spAnimationStateData_dispose( animation_state_data );
		}
	};

	typedef std::unique_ptr< Atlas, SpineDestructor > 				pAtlasData;
	typedef std::unique_ptr< SkeletonData, SpineDestructor > 		pSkeletonData;
	typedef std::unique_ptr< AnimationStateData, SpineDestructor > 	pAnimStateData;

	typedef std::map< HashString, pAtlasData > 		AtlasMap;
	typedef std::map< HashString, pSkeletonData > 	SkeletonDataMap;
	typedef std::map< HashString, MixInfoArray > 	MixInfoMap;

	/// \brief Map of all loaded Atlas Info
	AtlasMap 			m_atlasMap;
	
	/// \brief Map of all loaded SkeletonData
	SkeletonDataMap 	m_skelDataMap;
	
	/// \brief Map of all loaded mixing information ( not tied to specific skeleton data )
	MixInfoMap			m_mixInfoMap;

	/// \brief Will either load atlas file and , or if already loaded, return
	/// \arg atlas_file Atlas File to load
	/// \return Atlas ptr, if loaded, else NULL
	Atlas * getAtlas( char const * atlas_file );

	/// \brief Will either load skeleton file and save it for later, or if already loaded, return
	/// \arg atlas Already loaded atlas info, used to create SkeletonData
	/// \arg skeleton_file Skeleton JSON file to load
	/// \return SkeletonData ptr, if loaded, else NULL
	SkeletonData * getSkeletonData(
		Atlas * atlas,
		char const * skeleton_file );

	/// \brief Will either load the mix JSON file, or return the already loaded mix array
	/// \arg mix_file JSON mix file to load.
	/// \arg mix_array by ref mix array that holds all mixing relationships
	/// \return true if the file was succesfully loaded and mix_array now contains the mix info, or false.
	bool getMixInfo(
		char const * mix_file,
		SpineManager::MixInfoArray & mix_array );

	/// \brief Returns new AnimationStateData from the skeletonData given and 
	/// populated with the mix information from mix_file.
	/// \arg skeletonData Loaded SkeletonData that the AnimationStateData will associate to
	/// \arg mix_file Mix JSON File that contains the information for how to mix the animations of this skeleton
	/// \return AnimationStateData ptr if loaded and successfully created, false otherwise.
	AnimationStateData * getMixData(
		SkeletonData * skeletonData,
		char const * mix_file );

public:
	
	/// \brief Creates new SkeletonDrawable Object based off these Spine Files
	/// \arg atlas_file atlas file
	/// \arg skeleton_file skeleton_file
	/// \arg mix_file mix_file
	/// \return new SkeletonDrawable, or if loading fails, null
	/// \note These files will not be loaded multiple times, if already loaded, will use already loaded atlas, skeletondata or mix data.
	spine::SkeletonDrawable * createSkeletonDrawable(
		char const * atlas_file,
		char const * skeleton_file,
		char const * mix_file );

	/// \brief Creates new SkeletonDrawable Object based off these Spine Files
	/// \arg atlas_file atlas file
	/// \arg skeleton_file skeleton_file
	/// \return new SkeletonDrawable, or if loading fails, null
	/// \note These files will not be loaded multiple times, if already loaded, will use already loaded atlas, and skeletondata.
	spine::SkeletonDrawable * createSkeletonDrawable(
		char const * atlas_file,
		char const * skeleton_file );
};

}

#endif
