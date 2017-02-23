#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#pragma once

#include "config.hpp"

STREAMER_BEGIN_NS

int CreateDynamicPickup(int id, int type, float x, float y, float z, int worldid, int interiorid, int playerid, float streamDistance, int areaid, int priority);
int DestroyDynamicPickup(int id);
int IsValidDynamicPickup(int id);

STREAMER_END_NS
#endif