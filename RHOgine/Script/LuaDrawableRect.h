/// \file Loads the SFML sf::RectangleShape and gives functionality for lua to
/// create, delete, modify, and draw primitive rectangles.

#ifndef LUA_DRAWABLE_RECT_H
#define LUA_DRAWABLE_RECT_H

#include "lua_helpers.h"

namespace rho
{

namespace lua
{

void registerDrawableRect( lua_State * lua );

}

}

#endif
