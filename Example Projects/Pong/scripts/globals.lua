-- PONG LOGIC GLOBALS

-- Defines Screen Size
screenBoundary =
{
	left 	= 6,
	right 	= 1024,
	top		= 95,
	bottom	= 760
}

Player1String = "PLAYER1"
Player2String = "PLAYER2"

-- Player Scores
playerOneScore = 0
playerTwoScore = 0

-- Object Types
objectTypes =
{
	BALL	= "BALL",
	PADDLE	= "PADDLE"
}

-- Event Names
scoreChangedEventName 	= "ScoreChanged"	-- Event Triggered when the score changes
roundResetEventName 	= "RoundReset"		-- Event Triggered when the Round is reset
ballDeathEventName		= "BallDeath"		-- Event Triggered when Ball hits one of the side

-- End of Game Globals

-- GAME OBJECTS

-- Pong Scene
scene 	= nil

-- Camera
camera 	= nil

-- Global Game Objects
gameLogicObject		= nil
uiObject 			= nil
backgroundObject 	= nil
player1PaddleObj 	= nil
player2PaddleObj 	= nil
ballObj 			= nil

mainMenuObj			= nil		-- Main Menu Object

-- END OF GAME OBJECTS

-- END OF GLOBALS

-- Change the Score of the Player Given
-- Args:
--	player: "PLAYER1" or "PLAYER2"
function changeScore( player, amount )
	-- update Score
	if player == Player1String then
		playerOneScore = playerOneScore + amount
	elseif player == Player2String then
		playerTwoScore = playerTwoScore + amount
	end

	-- Shoot off Event
	local event = Event.newEvent( scoreChangedEventName )
	Event.setArgHashString( event, "player", player )
	Event.setArgInt( event, "amount", amount )
	EventManager.queueEvent( event )
	Event.deleteEvent( event )			-- YOU ARE ATAHERE!
end

-- Reset the Pong Board, placing the ball next to the player who won last round
-- Args:
--	player: "PLAYER1" or "PLAYER2", whoever the ball should start next to
function resetRound( player )

	if player1PaddleObj ~= nil then
		GameObject.setAlive( player1PaddleObj, false )
	end

	if player2PaddleObj ~= nil then
		GameObject.setAlive( player2PaddleObj, false )
	end

	if ballObj ~= nil then
		GameObject.setAlive( ballObj, false )
	end

	-- Create new Ball Object
	-- Reset Player Paddles ( or make new ones )
	-- Load Paddles
	player1PaddleObj = Scene.createGameObject( scene, "Paddle", { player = Player1String } )
	player2PaddleObj = Scene.createGameObject( scene, "Paddle", { player = Player2String } )
	-- Paddle.setPlayer( player2PaddleObj, 2 )

	-- Load Ball(s)
	ballObj = Scene.createGameObject( scene, "Ball" )

	-- Shoot off Event
	local event = Event.newEvent( roundResetEventName )
	Event.setArgHashString( event, "player", player )
	EventManager.queueEvent( event )
	Event.deleteEvent( event )			-- YOU ARE ATAHERE!
end

function fireBallDeathEvent( ball_obj, player_side )
	-- Shoot off Event
	local event = Event.newEvent( ballDeathEventName )
	Event.setArgHashString( event, "player_side", player_side )
	EventManager.fireEvent( event )
	Event.deleteEvent( event )			-- Fire Event
end

-- Set Up Scene for THE PANG!
function setUpPongScene()
	-- if scene ~= nil then
		-- Scene.deleteScene( scene )
		-- scene = nil
	-- end
	
	-- scene = Scene.newScene()	-- Make New Scene
	
	-- Load Game Logic
	gameLogicObject = Scene.createGameObject( scene, "GameLogic" )

	-- 	Load UI Object
	uiObject = Scene.createGameObject( scene, "UI" )

	-- Load Background
	backgroundObject = Scene.createGameObject( scene, "Background" )

	-- Set up Court
	resetRound( Player1String )
	
end

function setUpMenuScreen()
	
	-- if scene ~= nil then
		-- Scene.deleteScene( scene )
		-- scene = nil
	-- end
	
	-- scene = Scene.newScene()	-- Make New Scene
	
	mainMenuObj = Scene.createGameObject( scene, "MainMenu" )
	
end