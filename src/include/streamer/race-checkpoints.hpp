#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#pragma once

#include "config.hpp"

STREAMER_BEGIN_NS

int CreateDynamicRaceCP(int id, float x, float y, float z, float nx, float ny, float nz, float size, int worldid, int interiorid, int playerid, float streamDistance, int areaid, int priority);
int DestroyDynamicRaceCP(int id);
int IsValidDynamicRaceCP(int id);
int IsPlayerInDynamicRaceCP(int id, int checkpointid);
int GetPlayerVisibleDynamicRaceCP(int playerid);

STREAMER_END_NS
#endif