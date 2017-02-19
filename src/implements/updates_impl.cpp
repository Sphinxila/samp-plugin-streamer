/*
* Copyright (C) 2016 Incognito
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

#include "../include/config.hpp"
#include "../natives.h"

#include "../core.h"

#include "../include/updates.hpp"

#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <Eigen/Core>

#include <bitset>

STREAMER_BEGIN_NS

void Streamer_ProcessActiveItems() {
	core->getStreamer()->processActiveItems();
}

int Streamer_ToggleIdleUpdate(int playerid, bool toggle) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		p->second.updateWhenIdle = toggle;
		return 1;
	}
	return 0;
}

bool Streamer_IsToggleIdleUpdate(int playerid) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()){
		return p->second.updateWhenIdle;
	}
	return false;
}

int Streamer_ToggleCameraUpdate(int playerid, bool toggle) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		p->second.updateUsingCameraPosition = toggle;
		return 1;
	}
	return 0;
}

bool Streamer_IsToggleCameraUpdate(int playerid) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		return p->second.updateUsingCameraPosition;
	}
	return false;
}

int Streamer_ToggleItemUpdate(int playerid, size_t type, bool toggle) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		if (type >= 0 && type < STREAMER_MAX_TYPES) {
			p->second.enabledItems.set(type, toggle);
			return 1;
		}
	}
	return 0;
}

bool Streamer_IsToggleItemUpdate(int playerid, size_t type) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		if (type >= 0 && type < STREAMER_MAX_TYPES) {
			return p->second.enabledItems.test(type);
		}
	}
	return false;
}

float Streamer_GetLastUpdateTime() {
	return core->getStreamer()->getLastUpdateTime();
}

int Streamer_Update(int playerid, int type) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		p->second.interiorID = sampgdk::GetPlayerInterior(p->first);
		p->second.worldID = sampgdk::GetPlayerVirtualWorld(p->first);
		sampgdk::GetPlayerPos(p->first, &p->second.position[0], &p->second.position[1], &p->second.position[2]);
		core->getStreamer()->startManualUpdate(p->second, type);
		return 1;
	}
	return 0;
}

int Streamer_UpdateEx(int playerid, float x, float y, float z, int worldid, int interiorid, int type, int compensatedtime) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		p->second.position = Eigen::Vector3f(x, y, z);
		if (worldid >= 0) {
			p->second.worldID = worldid;
		} else {
			p->second.worldID = sampgdk::GetPlayerVirtualWorld(p->first);
		}
		if (interiorid >= 0) {
			p->second.interiorID = interiorid;
		} else {
			p->second.interiorID = sampgdk::GetPlayerInterior(p->first);
		}
		if (compensatedtime >= 0) {
			sampgdk::SetPlayerPos(p->first, p->second.position[0], p->second.position[1], p->second.position[2]);
			sampgdk::TogglePlayerControllable(p->first, false);
			p->second.delayedUpdate = true;
			p->second.delayedUpdateType = type;
			p->second.delayedUpdateTime = boost::chrono::steady_clock::now() + boost::chrono::milliseconds(compensatedtime);
		}
		core->getStreamer()->startManualUpdate(p->second, type);
		return 1;
	}
	return 0;
}

STREAMER_END_NS