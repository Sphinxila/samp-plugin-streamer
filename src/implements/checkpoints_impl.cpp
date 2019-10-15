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
#include "../utility.h"

#include <streamer/checkpoints.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include <Eigen/Core>

STREAMER_BEGIN_NS

int CreateDynamicCP(float x, float y, float z, float size, int worldid, int interiorid, int playerid, float stream, int areaid, int priority) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_CP) == core->getData()->checkpoints.size()) {
		return 0;
	}
	int checkpointId = Item::Checkpoint::identifier.get();
	Item::SharedCheckpoint checkpoint(new Item::Checkpoint);
	//checkpoint->amx = amx;
	checkpoint->checkpointId = checkpointId;
	checkpoint->inverseAreaChecking = false;
	checkpoint->originalComparableStreamDistance = -1.0f;
	checkpoint->positionOffset = Eigen::Vector3f::Zero();
	checkpoint->streamCallbacks = false;
	checkpoint->position = Eigen::Vector3f(x, y, z);
	checkpoint->size = size;

	Utility::addToContainer(checkpoint->worlds, worldid);
	Utility::addToContainer(checkpoint->interiors, interiorid);
	Utility::addToContainer(checkpoint->players, playerid);
	checkpoint->comparableStreamDistance = stream < STREAMER_STATIC_DISTANCE_CUTOFF ? stream : stream * stream;
	checkpoint->streamDistance = stream;
	Utility::addToContainer(checkpoint->areas, areaid);
	checkpoint->priority = priority;
	core->getGrid()->addCheckpoint(checkpoint);
	core->getData()->checkpoints.insert(std::make_pair(checkpointId, checkpoint));
	return checkpointId;
}

int DestroyDynamicCP(int id)
{
	boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
	if (c != core->getData()->checkpoints.end()) {
		Utility::destroyCheckpoint(c);
		return 1;
	}
	return 0;
}

int IsValidDynamicCP(int id) {
	boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
	if (c != core->getData()->checkpoints.end()) {
		return 1;
	}
	return 0;
}

int IsPlayerInDynamicCP(int id, int checkpointId)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(id);
	if (p != core->getData()->players.end()) {
		if (p->second.activeCheckpoint == checkpointId) {
			return 1;
		}
	}
	return 0;
}

int GetPlayerVisibleDynamicCP(int id)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(id);
	if (p != core->getData()->players.end()) {
		return p->second.visibleCheckpoint;
	}
	return 0;
}

STREAMER_END_NS
