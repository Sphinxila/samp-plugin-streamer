#ifndef DEPRECATED_HPP
#define DEPRECATED_HPP

#pragma once

#include "config.hpp"

STREAMER_BEGIN_NS

int Streamer_CallbackHook();
int DestroyAllDynamicObjects(bool serverWide);
int CountDynamicObjects(bool serverWide);
int DestroyAllDynamicPickups(bool serverWide);
int CountDynamicPickups(bool serverWide);
int DestroyAllDynamicCPs(bool serverWide);
int CountDynamicCPs(bool serverWide);
int DestroyAllDynamicRaceCPs(bool serverWide);
int CountDynamicRaceCPs(bool serverWide);
int DestroyAllDynamicMapIcons(bool serverWide);
int CountDynamicMapIcons(bool serverWide);
int DestroyAllDynamic3DTextLabels(bool serverWide);
int CountDynamic3DTextLabels(bool serverWide);
int DestroyAllDynamicAreas(bool serverWide);
int CountDynamicAreas(bool serverWide);
int TogglePlayerDynamicCP(int playerid, int checkpointid, bool toggle);
int TogglePlayerAllDynamicCPs(int playerid, bool toggle);
int TogglePlayerDynamicRaceCP(int playerid, int raceid, bool toggle);
int TogglePlayerAllDynamicRaceCPs(int playerid, bool toggle);
int TogglePlayerDynamicArea(int playerid, int areaid, bool toggle);
int TogglePlayerAllDynamicAreas(int playerid, bool toggle);

STREAMER_END_NS
#endif