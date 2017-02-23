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
#include <streamer/config.hpp>
#include "../natives.h"

#include "../core.h"
#include "../utility.h"

#include <streamer/race-checkpoints.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include <Eigen/Core>

STREAMER_BEGIN_NS

int CreateDynamicRaceCP(int id, float x, float y, float z, float nx, float ny, float nz, float size, int worldid, int interiorid, int playerid, float streamDistance, int areaid, int priority) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_RACE_CP) == core->getData()->raceCheckpoints.size()) {
		return 0;
	}
	int raceCheckpointID = Item::RaceCheckpoint::identifier.get();
	Item::SharedRaceCheckpoint raceCheckpoint(new Item::RaceCheckpoint);
	//raceCheckpoint->amx = amx;
	raceCheckpoint->raceCheckpointID = raceCheckpointID;
	raceCheckpoint->inverseAreaChecking = false;
	raceCheckpoint->originalComparableStreamDistance = -1.0f;
	raceCheckpoint->positionOffset = Eigen::Vector3f::Zero();
	raceCheckpoint->type = id;
	raceCheckpoint->position = Eigen::Vector3f(x, y, z);
	raceCheckpoint->next = Eigen::Vector3f(nx, ny, nz);
	raceCheckpoint->size = size;

	Utility::addToContainer(raceCheckpoint->worlds, worldid);
	Utility::addToContainer(raceCheckpoint->interiors, interiorid);
	Utility::addToContainer(raceCheckpoint->players, playerid);

	raceCheckpoint->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	raceCheckpoint->streamDistance = streamDistance;
	Utility::addToContainer(raceCheckpoint->areas, areaid);
	raceCheckpoint->priority = priority;
	core->getGrid()->addRaceCheckpoint(raceCheckpoint);
	core->getData()->raceCheckpoints.insert(std::make_pair(raceCheckpointID, raceCheckpoint));
	return raceCheckpointID;
}

int DestroyDynamicRaceCP(int id) {
	boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
	if (r != core->getData()->raceCheckpoints.end()) {
		Utility::destroyRaceCheckpoint(r);
		return 1;
	}
	return 0;
}

int IsValidDynamicRaceCP(int id) {
	boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
	if (r != core->getData()->raceCheckpoints.end()) {
		return 1;
	}
	return 0;
}

int IsPlayerInDynamicRaceCP(int id, int checkpointid) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(id);
	if (p != core->getData()->players.end()) {
		if (p->second.activeRaceCheckpoint == checkpointid) {
			return 1;
		}
	}
	return 0;
}

int GetPlayerVisibleDynamicRaceCP(int playerid) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		return p->second.visibleRaceCheckpoint;
	}
	return 0;
}

STREAMER_END_NS