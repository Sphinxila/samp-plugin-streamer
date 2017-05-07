#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#pragma once

#include "config.hpp"
#include <boost/scoped_ptr.hpp>

STREAMER_BEGIN_NS

float Streamer_GetCellDistance();
float Streamer_GetCellSize();
size_t Streamer_GetChunkSize(int type);
bool Streamer_SetChunkSize(int type, std::size_t size);
int Streamer_GetChunkTickRate(std::size_t type, std::size_t player);
int Streamer_GetMaxItems(int type);
int Streamer_GetPlayerTickRate(int id);
float Streamer_GetRadiusMultiplier(int type, int playerid);
int Streamer_GetTickRate();
std::vector<int> Streamer_GetTypePriority();
size_t Streamer_GetVisibleItems(int type, int playerid);
bool Streamer_IsToggleErrorCallback();
bool Streamer_ToggleItemInvAreas(int type, int id);
int Streamer_IsToggleItemStatic(int type, int id);
void Streamer_SetCellDistance(float distance);
void Streamer_SetCellSize(float size);
bool Streamer_SetChunkSize(int type, std::size_t size);
bool Streamer_SetChunkTickRate(int type, std::size_t value, int playerid);
bool Streamer_SetMaxItems(int type, std::size_t value);
int Streamer_SetPlayerTickRate(int id, std::size_t size);
int Streamer_SetRadiusMultiplier(int type, float value, int playerid);
int Streamer_SetTickRate(std::size_t size);
void Streamer_SetTypePriority(std::vector<int> types);
int Streamer_SetVisibleItems(int type, std::size_t value, int playerid);
void Streamer_ToggleErrorCallback(bool toggle);
int Streamer_ToggleItemAntiAreas(int type, int id, bool toggle);
int Streamer_ToggleItemStatic(int type, int id, bool toggle);
int Streamer_ToggleItemCallbacks(int type, int itemid, bool streamCallbacks);
int Streamer_IsToggleItemCallbacks(int type, int itemid);
STREAMER_END_NS
#endif