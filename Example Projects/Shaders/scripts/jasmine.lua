-- Currently Supported Spine functions:
-- Creation/updated/delete
--	newSkeletonDrawable
--	updateSkeleton
--	deleteSkeletonDrawable
--
-- Animation State Functions
--	AnimationState_setAnimationByName
--	AnimationState_addAnimationByName
--
-- Skeleton Functions
--	Skeleton_setToSetupPose
--	Skeleton_updateWorldTransform
--	Skeleton_setRootX
--	Skeleton_setRootY
--	Skeleton_setFlipX
--	Skeleton_setFlipY


Jasmine = Jasmine or {}		-- Set up class table

-- Sets up the Animations
-- Change this functions in the script
-- Save
-- Then, in the console type 
--	"dofile scripts/jasmine.lua"
-- Press Space bar and VOILA!
-- YOU GET A NEW SET OF FUCKING ANIMATIONS DONT NEED TO WASTE CALARIES ESCAPEING AND UPDAETING SCRIPT DO IT RUN TIME RUN THEM STREETS WHO REALLY RUNS THE SOUTH?!?!
function Jasmine.setUpAnimations( jas_obj )
	local obj_data = GameObject.getData( jas_obj )
	
	-- Set Initial Animation
	Spine.AnimationState_setAnimationByName( 
		obj_data.skel_drawable, 			-- Skeleton Drawable
		0, 									-- Track Index
		"walk", 							-- Animation Name
		true								-- Looping
	)		
	
	-- Setup queue for animations to play after initial
	Spine.AnimationState_addAnimationByName( 
		obj_data.skel_drawable, 			-- Skeleton Drawable
		0, 									-- Track Index
		"jump", 							-- Animation Name
		false, 								-- Looping
		0									-- Delay
	)
	
	Spine.AnimationState_addAnimationByName( obj_data.skel_drawable, 0, "walk", true,  0) 
	Spine.AnimationState_addAnimationByName( obj_data.skel_drawable, 0, "jump", false, 0)
end

function Jasmine.loadAndPose( jas_obj )
	local obj_data = GameObject.getData( jas_obj )	
	
	if obj_data.skel_drawable ~= nil then
		Spine.deleteSkeletonDrawable( obj_data.skel_drawable )
		obj_data.skel_drawable = nil
	end
	
	obj_data.skel_drawable = Spine.newSkeletonDrawable(
		"resources/spineboy.atlas",				-- Atlas File
		"resources/spineboy.json",				-- Skeleton Data File
		"resources/spineboy_mix.json"			-- Custom Mix File ( Optional )
	)
	
	-- Setting the Root Bones Values
	Spine.Skeleton_setRootX( obj_data.skel_drawable, 320 )
	Spine.Skeleton_setRootY( obj_data.skel_drawable, 420 )
	Spine.Skeleton_updateWorldTransform( obj_data.skel_drawable )	-- Updating World Position	
end

-- Flying Object Constructor
function Jasmine.init( jas_obj, args )

	local obj_data = GameObject.getData( jas_obj )
		
	-- Load Animation and Set Initial Pose
	Jasmine.loadAndPose( jas_obj )
	
	-- Setup animations
	Jasmine.setUpAnimations( jas_obj )
	
	-- Register to Events
	GameObject.addEventToRegister( jas_obj, "KeyReleased" )
	
	-- Load Shaders
	-- obj_data.shader = Shader.newShader( "inverse.frag", ShaderType.Fragment )
	-- obj_data.shader = Shader.newShader( "fragment.frag", ShaderType.Fragment )
	obj_data.shader = Shader.newShader( "sepia.frag", ShaderType.Fragment )
	
	if obj_data.shader == nil then
		print "Shader Failed to load"
		Game.endGame()
	else
		Shader.setParameterCurrentTexture( obj_data.shader, "texture" )
	end
	
	
	obj_data.background = Sprite.newSprite ( "TitleScreenBG.png" )
	
end

function Jasmine.update( jas_obj, dt )
	local obj_data = GameObject.getData( jas_obj )
	
	Spine.updateSkeleton( obj_data.skel_drawable, dt )	
end

function Jasmine.draw( jas_obj, render_target )
	local obj_data = GameObject.getData( jas_obj )
	
	if obj_data.shader ~= nil then	
		RenderTarget.draw( 
			render_target, 
			obj_data.background, 
			BlendMode.None, 
			obj_data.shader 
		)
		
		RenderTarget.draw( 
			render_target, 
			obj_data.skel_drawable, 
			BlendMode.None, 
			obj_data.shader 
		)
	end
	
end

function Jasmine.handleEvent( jas_obj, event )
	-- Handle Events
	local obj_data = GameObject.getData( jas_obj )	
	if Event.getEventType(event) == "KeyReleased" then
		if Event.getArg( event, "key_code" ) == 36 then
			Game.endGame()			
		end
		if Event.getArg( event, "key_code" ) == 17 then
			Jasmine.loadAndPose( jas_obj )			-- R key == Reload character
		end
		if Event.getArg( event, "key_code" ) == 57 then
			Jasmine.loadAndPose( jas_obj )
			Jasmine.setUpAnimations( jas_obj )		-- Space bar == Repose Character
		end
	end
end

function Jasmine.shutdown( jas_obj )
	local obj_data = GameObject.getData( jas_obj )	
	Spine.deleteSkeletonDrawable( obj_data.skel_drawable )
	
	if ( obj_data.shader ~= nil ) then
		Shader.deleteShader( obj_data.shader )
	end
	
	if obj_data.background ~= nil then
		Sprite.deleteSprite( obj_data.background )
	end
end

print "Jasmine Loaded"
