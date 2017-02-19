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
#include "../utility.h"

#include "../include/map-icons.hpp"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include <Eigen/Core>

STREAMER_BEGIN_NS

int CreateDynamicMapIcon(
	float x,
	float y,
	float z,
	int type,
	int color,
	int worldid,
	int interiorid,
	int playerid,
	float streamDistance,
	int style,
	int areaid,
	int priority
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_MAP_ICON) == core->getData()->mapIcons.size()) {
		return 0;
	}
	int mapIconID = Item::MapIcon::identifier.get();
	Item::SharedMapIcon mapIcon(new Item::MapIcon);
	//mapIcon->amx = amx;
	mapIcon->mapIconID = mapIconID;
	mapIcon->inverseAreaChecking = false;
	mapIcon->originalComparableStreamDistance = -1.0f;
	mapIcon->positionOffset = Eigen::Vector3f::Zero();
	mapIcon->position = Eigen::Vector3f(x, y, z);
	mapIcon->type = type;
	mapIcon->color = color;

	Utility::addToContainer(mapIcon->worlds, worldid);
	Utility::addToContainer(mapIcon->interiors, interiorid);
	Utility::addToContainer(mapIcon->players, playerid);

	mapIcon->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	mapIcon->streamDistance = streamDistance;
	mapIcon->style = style;
	Utility::addToContainer(mapIcon->areas, areaid);
	mapIcon->priority = priority;
	core->getGrid()->addMapIcon(mapIcon);
	core->getData()->mapIcons.insert(std::make_pair(mapIconID, mapIcon));
	return static_cast<cell>(mapIconID);
}

int DestroyDynamicMapIcon(int id) {
	boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
	if (m != core->getData()->mapIcons.end()) {
		Utility::destroyMapIcon(m);
		return 1;
	}
	return 0;
}

int IsValidDynamicMapIcon(int id) {
	boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
	if (m != core->getData()->mapIcons.end()) {
		return 1;
	}
	return 0;
}

STREAMER_END_NS
