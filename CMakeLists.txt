cmake_minimum_required(VERSION 2.8)

project(RHOgine)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Wall to get more warnings
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# detect the OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX 1)
else()
    message(FATAL_ERROR "Unsupported operating system")
    return()
endif()

set( SFML_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-bin" CACHE STRING "SFML Build Directory" )

# Build a Test EXe that is just a window
option( BUILD_TEST_EXECUTABLE "Build a Test Executable that doesn't do anything" ON )

# Grab the OpenGL Shit that we need, son
find_package( OpenGL REQUIRED )
# find_package( GLU REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS}  )
# include_directories( ${GLU_INCLUDE_DIRS} )

#include RHOgine
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

# RHOgine Source Files
file(GLOB rho_source_files
    "${CMAKE_CURRENT_SOURCE_DIR}/RHOgine/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RHOgine/Audio/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RHOgine/Console/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RHOgine/EventManager/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RHOgine/GameObjectFactory/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RHOgine/GameObjects/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RHOgine/Graphics/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RHOgine/Input/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RHOgine/Logic/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RHOgine/Scene/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RHOgine/Script/*.cpp"
)

# JSONcpp Source Directory
set( JSON_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/JSON" CACHE STRING "JsonCpp Source Directory( expecting 'include' and 'src' to be subdirs)" )

# Include JSON
include_directories(
	"${JSON_SRC_DIR}/include"
)

# JSON Source Files
file(GLOB json_source_files
	"${JSON_SRC_DIR}/src/*.cpp"
	"${JSON_SRC_DIR}/src/*.inl"
)

# Lua Source Directory
set( LUA_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Lua" CACHE STRING "Lua Source Directory( expecting 'include' and 'src' to be subdirs)" )

# Include Lua
include_directories(
	"${LUA_SRC_DIR}/include"
)

# Lua Source Files
file(GLOB lua_source_files
	"${LUA_SRC_DIR}/src/*.cpp"
	"${LUA_SRC_DIR}/src/*.c"
	"${LUA_SRC_DIR}/src/*.h"
)

# Spine Source Directory
set( SPINE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Spine" CACHE STRING "Spine Source Directory( expecting 'Include' and 'Source' to be subdirs)" )

# Include Spine
include_directories(
	"${SPINE_SRC_DIR}/Include"
)

# Spine Source Files
file(GLOB spine_source_files
	"${SPINE_SRC_DIR}/Source/*.cpp"
	"${SPINE_SRC_DIR}/Source/*.c"
	"${SPINE_SRC_DIR}/Source/*.h"
)

# SFML Source Directory
set( SFML_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML" CACHE STRING "SFML Source Directory( expecting 'include' subdir)" )

# Include SFML
include_directories(
	"${SFML_SRC_DIR}/include"
)

LINK_DIRECTORIES( ${CMAKE_BINARY_DIR} )

LINK_DIRECTORIES( ${SFML_BUILD_DIR}/lib )

# add_subdirectory( Lua )
# add_subdirectory( JSON )
# add_subdirectory( Spine )

add_library( RHOgine STATIC
	${rho_source_files}
	${json_source_files}
	${lua_source_files}
	${spine_source_files}
)

if ( ${BUILD_TEST_EXECUTABLE} )

	add_executable( RHOgineTest
		"${CMAKE_CURRENT_SOURCE_DIR}/main-test.cpp"
	)
	
	add_custom_command(TARGET RHOgineTest POST_BUILD        # Adds a post-build event to RHOgineTest
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  		# which executes "cmake - E copy_if_different..."
			"${CMAKE_SOURCE_DIR}/sansation.ttf"      	# <--this is in-file
			"${CMAKE_BINARY_DIR}/sansation.ttf")              # <--this is out-file path
			
	add_definitions( -DSFML_STATIC )
	add_definitions( -DDEBUG )

	if ( WINDOWS )
		add_custom_command(TARGET RHOgineTest POST_BUILD        # Adds a post-build event to RHOgineTest
			COMMAND ${CMAKE_COMMAND} -E copy_if_different  		# which executes "cmake - E copy_if_different..."
				"${SFML_SRC_DIR}/extlibs/bin/x86/libsndfile-1.dll"     # <--this is in-file
				"${CMAKE_BINARY_DIR}/libsndfile-1.dll")         # <--this is out-file path
				
		add_custom_command(TARGET RHOgineTest POST_BUILD        # Adds a post-build event to RHOgineTest
			COMMAND ${CMAKE_COMMAND} -E copy_if_different  		# which executes "cmake - E copy_if_different..."
				"${SFML_SRC_DIR}/extlibs/bin/x86/openal32.dll"      	# <--this is in-file
				"${CMAKE_BINARY_DIR}/openal32.dll")             # <--this is out-file path
				
		target_link_libraries(RHOgineTest
			RHOgine
			sfml-network-s-d
			sfml-graphics-s-d
			sfml-audio-s-d
			sfml-window-s-d
			sfml-system-s-d
			sfml-main-d
			${OPENGL_LIBRARIES}
		)
	endif( WINDOWS )

	if ( LINUX )
        find_package(X11 REQUIRED)
        include_directories(${X11_INCLUDE_DIR})

        # add_definitions( -pthread )
        target_link_libraries(RHOgineTest
			RHOgine
			sfml-network-s-d
			sfml-graphics-s-d
			sfml-audio-s-d
			sfml-window-s-d
			sfml-system-s-d
			sndfile
			${X11_LIBRARIES}
			Xrandr
			openal
			jpeg
			freetype
			pthread
			GLEW
			${OPENGL_LIBRARIES}
		)
	endif ( LINUX )


endif( ${BUILD_TEST_EXECUTABLE} )

# target_link_libraries( RHOgine Lua )
# target_link_libraries( RHOgine JSON )
# target_link_libraries( RHOgine Spine )
