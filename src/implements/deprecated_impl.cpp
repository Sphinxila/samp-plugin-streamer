/*
* Copyright (C) 2017 Incognito (Edited by ProMetheus)
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include <streamer/config.hpp>
#include "../natives.h"
#include "../core.h"

#include <streamer/deprecated.hpp>

#include <boost/scoped_ptr.hpp>

#include <streamer/miscellaneous.hpp>

STREAMER_BEGIN_NS

int Streamer_CallbackHook() {
	return 0;
}

int DestroyAllDynamicObjects(bool serverWide) {
	return Streamer_DestroyAllItems(STREAMER_TYPE_OBJECT, serverWide);
}

int CountDynamicObjects(bool serverWide) {
	return Streamer_CountItems(STREAMER_TYPE_OBJECT, serverWide);
}

int DestroyAllDynamicPickups(bool serverWide) {
	return Streamer_DestroyAllItems(STREAMER_TYPE_PICKUP, serverWide);
}

int CountDynamicPickups(bool serverWide) {
	return Streamer_CountItems(STREAMER_TYPE_PICKUP, serverWide);
}

int DestroyAllDynamicCPs(bool serverWide) {
	return Streamer_DestroyAllItems(STREAMER_TYPE_CP, serverWide);
}

int CountDynamicCPs(bool serverWide) {
	return Streamer_CountItems(STREAMER_TYPE_CP, serverWide);
}

int DestroyAllDynamicRaceCPs(bool serverWide) {
	return Streamer_DestroyAllItems(STREAMER_TYPE_RACE_CP, serverWide);
}

int CountDynamicRaceCPs(bool serverWide) {
	return Streamer_CountItems(STREAMER_TYPE_RACE_CP, serverWide);
}

int DestroyAllDynamicMapIcons(bool serverWide) {
	return Streamer_DestroyAllItems(STREAMER_TYPE_MAP_ICON, serverWide);
}

int CountDynamicMapIcons(bool serverWide) {
	return Streamer_CountItems(STREAMER_TYPE_MAP_ICON, serverWide);
}

int DestroyAllDynamic3DTextLabels(bool serverWide) {
	return Streamer_DestroyAllItems(STREAMER_TYPE_3D_TEXT_LABEL, serverWide);
}

int CountDynamic3DTextLabels(bool serverWide) {
	return Streamer_CountItems(STREAMER_TYPE_3D_TEXT_LABEL, serverWide);
}

int DestroyAllDynamicAreas(bool serverWide) {
	return Streamer_DestroyAllItems(STREAMER_TYPE_AREA, serverWide);
}

int CountDynamicAreas(bool serverWide) {
	return Streamer_CountItems(STREAMER_TYPE_AREA, serverWide);
}

int TogglePlayerDynamicCP(int playerid, int checkpointid, bool toggle)
{
	return Streamer_ToggleItem(playerid, STREAMER_TYPE_CP, checkpointid, toggle);
}

int TogglePlayerAllDynamicCPs(int playerid, bool toggle)
{
	boost::unordered_set<int> exceptions;
	return Streamer_ToggleAllItems(playerid, STREAMER_TYPE_CP, toggle, exceptions);
}

int TogglePlayerDynamicRaceCP(int playerid, int raceid, bool toggle)
{
	return Streamer_ToggleItem(playerid, STREAMER_TYPE_RACE_CP, raceid, toggle);
}

int TogglePlayerAllDynamicRaceCPs(int playerid, bool toggle)
{
	boost::unordered_set<int> exceptions;
	return Streamer_ToggleAllItems(playerid, STREAMER_TYPE_RACE_CP, toggle, exceptions);
}

int TogglePlayerDynamicArea(int playerid, int areaid, bool toggle)
{
	return Streamer_ToggleItem(playerid, STREAMER_TYPE_AREA, areaid, toggle);
}

int TogglePlayerAllDynamicAreas(int playerid, bool toggle)
{
	boost::unordered_set<int> exceptions;
	return Streamer_ToggleAllItems(playerid, STREAMER_TYPE_AREA, toggle, exceptions);
}

STREAMER_END_NS