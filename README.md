RHOgine
=======

RHO Industries C++ Game Engine

How to Build
====

1. Run cmake on the SFML library. The destination directory doesn't matter, but is used later.  Change the following values from their default:
	* BUILD_SHARED_LIBS to FALSE
	* CMAKE_BUILD_TYPE to "Debug"
	* SFML_USE_STATIC_STD_LIBS to TRUE
2. Generate and Build SFML.  The result should the creation of the following libraries in the lib/ directory of your cmake project folder:
	* libsfml-audio-s-d.a
	* libsfml-graphics-s-d.a
	* libsfml-main-d.a
	* libsfml-network-s-d.a
	* libsfml-system-s-d.a
	* libsfml-window-s-d.a
3. Configure the RHOgine project using cmake. 
	* Make sure sure the SFML_BUILD_DIR points to the location of the folder you told cmake to build your SFML project. This folder must have a lib directory with the above built libraries in it. It defaults to RHOgine/SFML-bin but that may not be the case.
	* Also change SFML_EXTERNAL_LIBS_DIR to point to the libs-<compiler> you are using.  It defaults to mingw.
4. After Generation, Build the RHOgine project.
5. After you build, you should find the following files:
	* libsndfile-1.dll
	* openal32.dll
	* RHOgineTest.exe
	* sansation.ttf
6. These files need to be in the same directory for the RHOgine game to run.
7. The game will look for a main.lua script, load the script, and run any global functions like:
	* init()
	* update(dt)
	* draw(render_target)
	* shutdown()
8. Look at the included example projects to see to make a game entirley in lua. To run these, simply copy the files mentioned above into the directory with main.lua and run RHOgineTest.exe.

Games Built with RHOgine
====
**Candy Pong**
![Alt text](http://3.bp.blogspot.com/-YdviePCg7ZI/VOp3aMRX1wI/AAAAAAAAAAg/y6VMV7yMLXM/s1600/screenshot.png "Saga Panga")
Simple Pong Game made for the Candy Jam 2014
[Link](https://dl.dropboxusercontent.com/u/41080683/OldGames/PongCandy.zip)

**RGBlaster**
![Alt text](http://4.bp.blogspot.com/-GnnL-a2p9mU/VOp3etV1UzI/AAAAAAAAAAo/E5Q8OfGw1KY/s1600/screenshot.png "Saga Panga")
Game built at the Games++ 2014 Game Jam:
[Link](https://dl.dropboxusercontent.com/u/41080683/OldGames/GamesPP.zip)

External Libraries Used
====

* SFML 2.2 https://github.com/LaurentGomila/SFML/tree/2.2
* Spine https://github.com/EsotericSoftware/spine-runtimes
* JsonCpp http://jsoncpp.sourceforge.net/
* lua 5.2 http://www.lua.org/
