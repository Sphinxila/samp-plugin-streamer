#ifndef MAP_ICONS_HPP
#define MAP_ICONS_HPP

#pragma once

#include "config.hpp"

STREAMER_BEGIN_NS

int CreateDynamicMapIcon(float x, float y, float z, int type, int color, int worldid, int interiorid, int playerid, float streamDistance, int style, int areaid, int priority);
int DestroyDynamicMapIcon(int id);
int IsValidDynamicMapIcon(int id);

STREAMER_END_NS
#endif