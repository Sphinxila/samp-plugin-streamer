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

#include <streamer/pickups.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <Eigen/Core>

STREAMER_BEGIN_NS

int CreateDynamicPickup(int id, int type, float x, float y, float z, int worldid, int interiorid, int playerid, float streamDistance, int areaid, int priority) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_PICKUP) == core->getData()->pickups.size()) {
		return 0;
	}

	int pickupID = Item::Pickup::identifier.get();
	Item::SharedPickup pickup(new Item::Pickup);
	//pickup->amx = amx;
	pickup->pickupID = pickupID;
	pickup->inverseAreaChecking = false;
	pickup->originalComparableStreamDistance = -1.0f;
	pickup->positionOffset = Eigen::Vector3f::Zero();
	pickup->streamCallbacks = false;
	pickup->modelID = id;
	pickup->type = type;
	pickup->position = Eigen::Vector3f(x, y, z);

	Utility::addToContainer(pickup->worlds, worldid);
	Utility::addToContainer(pickup->interiors, interiorid);
	Utility::addToContainer(pickup->players, playerid);

	pickup->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	pickup->streamDistance = streamDistance;
	Utility::addToContainer(pickup->areas, areaid);
	pickup->priority = priority;
	core->getGrid()->addPickup(pickup);
	core->getData()->pickups.insert(std::make_pair(pickupID, pickup));
	return static_cast<cell>(pickupID);
}

int DestroyDynamicPickup(int id) {
	boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
	if (p != core->getData()->pickups.end()) {
		Utility::destroyPickup(p);
		return 1;
	}
	return 0;
}

int IsValidDynamicPickup(int id) {
	boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
	if (p != core->getData()->pickups.end()) {
		return 1;
	}
	return 0;
}

STREAMER_END_NS