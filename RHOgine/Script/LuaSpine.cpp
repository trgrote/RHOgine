#include "LuaSpine.h"

#include "../RHOgine.h"
#include "../Graphics/SpineAnimation.h"

using namespace rho;

#define CHECK_SKEL_DRAW( lua, index )\
	static_cast< spine::SkeletonDrawable *>( lua::checklightuserdata( lua, index ) )

///////////////////////////////////////////////////////////////////////////////

static int l_newSkeletonDrawable( lua_State * lua )
{
	// Needs two to three arguments
	char const * atlas_file_name = luaL_checkstring( lua, 1 );
	char const * skel_file_name = luaL_checkstring( lua, 2 );
	
	if ( lua_gettop( lua ) > 2 )
	{
		char const * mix_file_name = luaL_checkstring( lua, 3 );
		
		lua::pop_all( lua );
		
		spine::SkeletonDrawable * rval = cRHOgine::GetInstance().getSpine().createSkeletonDrawable(
			atlas_file_name,
			skel_file_name,
			mix_file_name );
			
		lua_pushlightuserdata( lua, rval );
	}
	else
	{
		lua::pop_all( lua );
		
		spine::SkeletonDrawable * rval = cRHOgine::GetInstance().getSpine().createSkeletonDrawable(
			atlas_file_name,
			skel_file_name );
			
		lua_pushlightuserdata( lua, rval );
	}
	
	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static int l_updateSkeleton( lua_State * lua )
{
	spine::SkeletonDrawable * rval = CHECK_SKEL_DRAW( lua, 1 );
	float deltaTime = luaL_checknumber( lua, 2 );
	lua::pop_all( lua );
	
	rval->update( deltaTime );
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_deleteSkeletonDrawable( lua_State * lua )
{
	spine::SkeletonDrawable * drawable = CHECK_SKEL_DRAW( lua, 1 );
	
	lua::pop_all( lua );
	
	delete drawable;
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_AnimationState_setAnimationByName( lua_State * lua )
{
	spine::SkeletonDrawable * drawable = CHECK_SKEL_DRAW( lua, 1 );
	
	int track_index 			= luaL_checkint( lua, 2 );
	char const * animationName 	= luaL_checkstring( lua, 3 );
	bool loop 					= lua::checkbool( lua, 4 );
	
	lua::pop_all( lua );
	
	AnimationState_setAnimationByName( 
		drawable->state, 
		track_index,
		animationName,
		loop 
	);
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_AnimationState_addAnimationByName( lua_State * lua )
{
	spine::SkeletonDrawable * drawable = CHECK_SKEL_DRAW( lua, 1 );
	
	int track_index 			= luaL_checkint( lua, 2 );
	char const * animationName 	= luaL_checkstring( lua, 3 );
	bool loop 					= lua::checkbool( lua, 4 );
	float delay					= luaL_checknumber( lua, 5 );
	
	lua::pop_all( lua );
	
	AnimationState_addAnimationByName( 
		drawable->state, 
		track_index,
		animationName,
		loop,
		delay
	);
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_Skeleton_setToSetupPose( lua_State * lua )
{
	spine::SkeletonDrawable * drawable = CHECK_SKEL_DRAW( lua, 1 );
	Skeleton_setToSetupPose(drawable->skeleton);	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_Skeleton_updateWorldTransform( lua_State * lua )
{
	spine::SkeletonDrawable * drawable = CHECK_SKEL_DRAW( lua, 1 );
	Skeleton_updateWorldTransform(drawable->skeleton);	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_Skeleton_setRootX( lua_State * lua )
{
	spine::SkeletonDrawable * drawable = CHECK_SKEL_DRAW( lua, 1 );
	
	drawable->skeleton->root->x = static_cast<float>( luaL_checknumber( lua, 2 ) );
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_Skeleton_setRootY( lua_State * lua )
{
	spine::SkeletonDrawable * drawable = CHECK_SKEL_DRAW( lua, 1 );
	
	drawable->skeleton->root->y = static_cast<float>( luaL_checknumber( lua, 2 ) );
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_Skeleton_setFlipX( lua_State * lua )
{
	spine::SkeletonDrawable * drawable = CHECK_SKEL_DRAW( lua, 1 );
	
	drawable->skeleton->flipX = lua::checkbool( lua, 2 );
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static int l_Skeleton_setFlipY( lua_State * lua )
{
	spine::SkeletonDrawable * drawable = CHECK_SKEL_DRAW( lua, 1 );
	
	drawable->skeleton->flipY = lua::checkbool( lua, 2 );
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

static luaL_Reg const spine_funcs [] =
{
	{ "newSkeletonDrawable", 			l_newSkeletonDrawable },
	{ "updateSkeleton", 				l_updateSkeleton },
	{ "deleteSkeletonDrawable",			l_deleteSkeletonDrawable },
	
	// Animation State Functions
	{ "AnimationState_setAnimationByName",			l_AnimationState_setAnimationByName },
	{ "AnimationState_addAnimationByName",			l_AnimationState_addAnimationByName },
	
	// Skeleton Nonsense
	{ "Skeleton_setToSetupPose"	,					l_Skeleton_setToSetupPose },
	{ "Skeleton_updateWorldTransform",				l_Skeleton_updateWorldTransform },
	{ "Skeleton_setRootX",							l_Skeleton_setRootX },
	{ "Skeleton_setRootY",							l_Skeleton_setRootY },
	{ "Skeleton_setFlipX",							l_Skeleton_setFlipX },
	{ "Skeleton_setFlipY",							l_Skeleton_setFlipY },
	
	
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////

void rho::lua::registerSpine( lua_State * lua )
{
	lua::register_funcs( lua, "Spine", spine_funcs );
}

///////////////////////////////////////////////////////////////////////////////
