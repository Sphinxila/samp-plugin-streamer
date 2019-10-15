#ifndef CHECKPOINTS_HPP
#define CHECKPOINTS_HPP

#pragma once

#include "config.hpp"

STREAMER_BEGIN_NS

int CreateDynamicCP(float x, float y, float z, float size, int worldid, int interiorid, int playerid, float stream, int areaid, int priority);
int DestroyDynamicCP(int id);
int GetPlayerVisibleDynamicCP(int id);
int IsPlayerInDynamicCP(int id, int checkpointId);
int IsValidDynamicCP(int id);

STREAMER_END_NS
#endif