#ifndef TEXTLABELS_HPP
#define TEXTLABELS_HPP

#pragma once

#include "config.hpp"

STREAMER_BEGIN_NS

void Streamer_ProcessActiveItems();
int Streamer_ToggleIdleUpdate(int playerid, bool toggle);
bool Streamer_IsToggleIdleUpdate(int playerid);
int Streamer_ToggleCameraUpdate(int playerid, bool toggle);
bool Streamer_IsToggleCameraUpdate(int playerid);
int Streamer_ToggleItemUpdate(int playerid, size_t type, bool toggle);
bool Streamer_IsToggleItemUpdate(int playerid, size_t type);
float Streamer_GetLastUpdateTime();
int Streamer_Update(int playerid, int type);
int Streamer_UpdateEx(int playerid, float x, float y, float z, int worldid, int interiorid, int type, int compensatedtime);

STREAMER_END_NS
#endif