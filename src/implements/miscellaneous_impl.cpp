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

#include <streamer/miscellaneous.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/variant.hpp>

#include <Eigen/Core>

STREAMER_BEGIN_NS

int Streamer_GetDistanceToItem(float x, float y, float z, int type, int id, float &distance, int dimensions) {
	Eigen::Vector3f position = Eigen::Vector3f::Zero();
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
		if (o != core->getData()->objects.end()) {
			if (o->second->attach) {
				position = o->second->attach->position;
			} else {
				position = o->second->position;
			}
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
		if (p != core->getData()->pickups.end()) {
			position = p->second->position;
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
		if (c != core->getData()->checkpoints.end()) {
			position = c->second->position;
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
		if (r != core->getData()->raceCheckpoints.end()) {
			position = r->second->position;
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
		if (m != core->getData()->mapIcons.end()) {
			position = m->second->position;
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end()) {
			if (t->second->attach) {
				position = t->second->attach->position;
			} else {
				position = t->second->position;
			}
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_AREA:
	{
		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
		if (a != core->getData()->areas.end())
		{
			boost::variant<Polygon2D, Box2D, Box3D, Eigen::Vector2f, Eigen::Vector3f> areaPosition;
			if (a->second->attach) {
				areaPosition = a->second->position;
			} else {
				areaPosition = a->second->position;
			}

			switch (a->second->type)
			{
			case STREAMER_AREA_TYPE_CIRCLE:
			case STREAMER_AREA_TYPE_CYLINDER:
			{
				distance = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(x, y), boost::get<Eigen::Vector2f>(areaPosition)));
				return 1;
			}
			case STREAMER_AREA_TYPE_SPHERE:
			{
				position = boost::get<Eigen::Vector3f>(areaPosition);
				break;
			}
			case STREAMER_AREA_TYPE_RECTANGLE:
			{
				Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Box2D>(areaPosition));
				distance = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(x, y), centroid));
				return 1;
			}
			case STREAMER_AREA_TYPE_CUBOID:
			{
				Eigen::Vector3f centroid = boost::geometry::return_centroid<Eigen::Vector3f>(boost::get<Box3D>(areaPosition));
				distance = static_cast<float>(boost::geometry::distance(Eigen::Vector3f(x, y, z), centroid));
				return 1;
			}
			case STREAMER_AREA_TYPE_POLYGON:
			{
				Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Polygon2D>(areaPosition));
				distance = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(x, y), centroid));
				return 1;
			}
			}
		}
		return 0;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
		if (a != core->getData()->actors.end()) {
			position = a->second->position;
			break;
		}
		return 0;
	}
	default:
	{
		Utility::logError("Streamer_GetDistanceToItem: Invalid type specified");
		return 0;
	}
	}
	switch (dimensions)
	{
		case 2:
		{
			distance = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(x, y), Eigen::Vector2f(position[0], position[1])));
			return 1;
		}
		case 3:
		{
			distance = static_cast<float>(boost::geometry::distance(Eigen::Vector3f(x, y, z), position));
			return 1;
		}
		default:
		{
			Utility::logError("Streamer_GetDistanceToItem: Invalid number of dimensions specified (outside range of 2-3).");
			return 0;
		}
	}
	return 0;
}

int Streamer_ToggleItem(int playerid, int type, int id, bool toggle) {
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
		if (o != core->getData()->objects.end()) {
			if (!toggle) {
				return static_cast<cell>(Utility::removeFromContainer(o->second->players, playerid));
			} else {
				return static_cast<cell>(Utility::addToContainer(o->second->players, playerid));
			}
		}
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
		if (p != core->getData()->pickups.end()) {
			if (!toggle) {
				return static_cast<cell>(Utility::removeFromContainer(p->second->players, playerid));
			} else {
				return static_cast<cell>(Utility::addToContainer(p->second->players, playerid));
			}
		}
		break;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
		if (c != core->getData()->checkpoints.end()) {
			if (!toggle) {
				return static_cast<cell>(Utility::removeFromContainer(c->second->players, playerid));
			} else {
				return static_cast<cell>(Utility::addToContainer(c->second->players, playerid));
			}
		}
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
		if (r != core->getData()->raceCheckpoints.end()) {
			if (!toggle) {
				return static_cast<cell>(Utility::removeFromContainer(r->second->players, playerid));
			} else {
				return static_cast<cell>(Utility::addToContainer(r->second->players, playerid));
			}
		}
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
		if (m != core->getData()->mapIcons.end()) {
			if (!toggle) {
				return static_cast<cell>(Utility::removeFromContainer(m->second->players, playerid));
			} else {
				return static_cast<cell>(Utility::addToContainer(m->second->players, playerid));
			}
		}
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end()) {
			if (!toggle) {
				return static_cast<cell>(Utility::removeFromContainer(t->second->players, playerid));
			} else {
				return static_cast<cell>(Utility::addToContainer(t->second->players, playerid));
			}
		}
		break;
	}
	case STREAMER_TYPE_AREA:
	{
		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
		if (a != core->getData()->areas.end()) {
			if (!toggle) {
				return static_cast<cell>(Utility::removeFromContainer(a->second->players, playerid));
			} else {
				return static_cast<cell>(Utility::addToContainer(a->second->players, playerid));
			}
		}
		break;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
		if (a != core->getData()->actors.end()) {
			if (!toggle) {
				return static_cast<cell>(Utility::removeFromContainer(a->second->players, playerid));
			} else {
				return static_cast<cell>(Utility::addToContainer(a->second->players, playerid));
			}
		}
		break;
	}
	default:
	{
		Utility::logError("Streamer_ToggleItem: Invalid type specified.");
		return 0;
	}
	}
	return 0;
}

int Streamer_IsToggleItem(int playerid, int type, int id) {
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
		if (o != core->getData()->objects.end()) {
			return static_cast<cell>(Utility::isInContainer(o->second->players, playerid));
		}
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
		if (p != core->getData()->pickups.end()) {
			return static_cast<cell>(Utility::isInContainer(p->second->players, playerid));
		}
		break;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
		if (c != core->getData()->checkpoints.end()) {
			return static_cast<cell>(Utility::isInContainer(c->second->players, playerid));
		}
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
		if (r != core->getData()->raceCheckpoints.end()) {
			return static_cast<cell>(Utility::isInContainer(r->second->players, playerid));
		}
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
		if (m != core->getData()->mapIcons.end()) {
			return static_cast<cell>(Utility::isInContainer(m->second->players, playerid));
		}
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end()) {
			return static_cast<cell>(Utility::isInContainer(t->second->players, playerid));
		}
		break;
	}
	case STREAMER_TYPE_AREA:
	{
		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
		if (a != core->getData()->areas.end()) {
			return static_cast<cell>(Utility::isInContainer(a->second->players, playerid));
		}
		break;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
		if (a != core->getData()->actors.end()) {
			return static_cast<cell>(Utility::isInContainer(a->second->players, playerid));
		}
		break;
	}
	default:
	{
		Utility::logError("Streamer_IsToggleItem: Invalid type specified.");
		return 0;
	}
	}
	return 0;
}

int Streamer_ToggleAllItems(int playerid, int type, bool toggle, boost::unordered_set<int> exceptions) {
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		for (boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.begin(); o != core->getData()->objects.end(); ++o) {
			boost::unordered_set<int>::iterator e = exceptions.find(o->first);
			if (e == exceptions.end()) {
				if (!toggle) {
					Utility::removeFromContainer(o->second->players, playerid);
				} else {
					Utility::addToContainer(o->second->players, playerid);
				}
			}
		}
		return 1;
	}
	case STREAMER_TYPE_PICKUP:
	{
		for (boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.begin(); p != core->getData()->pickups.end(); ++p) {
			boost::unordered_set<int>::iterator e = exceptions.find(p->first);
			if (e == exceptions.end()) {
				if (!toggle) {
					Utility::removeFromContainer(p->second->players, playerid);
				} else {
					Utility::addToContainer(p->second->players, playerid);
				}
			}
		}
		return 1;
	}
	case STREAMER_TYPE_CP:
	{
		for (boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin(); c != core->getData()->checkpoints.end(); ++c) {
			boost::unordered_set<int>::iterator e = exceptions.find(c->first);
			if (e == exceptions.end()) {
				if (!toggle) {
					Utility::removeFromContainer(c->second->players, playerid);
				} else {
					Utility::addToContainer(c->second->players, playerid);
				}
			}
		}
		return 1;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		for (boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin(); r != core->getData()->raceCheckpoints.end(); ++r)
		{
			boost::unordered_set<int>::iterator e = exceptions.find(r->first);
			if (e == exceptions.end())
			{
				if (!toggle) {
					Utility::removeFromContainer(r->second->players, playerid);
				} else {
					Utility::addToContainer(r->second->players, playerid);
				}
			}
		}
		return 1;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		for (boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.begin(); m != core->getData()->mapIcons.end(); ++m) {
			boost::unordered_set<int>::iterator e = exceptions.find(m->first);
			if (e == exceptions.end()) {
				if (!toggle) { 
					Utility::removeFromContainer(m->second->players, playerid);
				} else {
					Utility::addToContainer(m->second->players, playerid);
				}
			}
		}
		return 1;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		for (boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.begin(); t != core->getData()->textLabels.end(); ++t) {
			boost::unordered_set<int>::iterator e = exceptions.find(t->first);
			if (e == exceptions.end()) {
				if (!toggle) {
					Utility::removeFromContainer(t->second->players, playerid);
				} else {
					Utility::addToContainer(t->second->players, playerid);
				}
			}
		}
		return 1;
	}
	case STREAMER_TYPE_AREA:
	{
		for (boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a) {
			boost::unordered_set<int>::iterator e = exceptions.find(a->first);
			if (e == exceptions.end()) {
				if (!toggle) {
					Utility::removeFromContainer(a->second->players, playerid);
				} else {
					Utility::addToContainer(a->second->players, playerid);
				}
			}
		}
		return 1;
	}
	case STREAMER_TYPE_ACTOR:
	{
		for (boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.begin(); a != core->getData()->actors.end(); ++a) {
			boost::unordered_set<int>::iterator e = exceptions.find(a->first);
			if (e == exceptions.end()) {
				if (!toggle) {
					Utility::removeFromContainer(a->second->players, playerid);
				} else {
					Utility::addToContainer(a->second->players, playerid);
				}
			}
		}
		return 1;
	}
	default:
	{
		Utility::logError("Streamer_ToggleAllItems: Invalid type specified.");
		return 0;
	}
	}
	return 0;
}

int Streamer_GetItemInternalID(int playerid, int type, int streamerid) {
	switch (type)
	{
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, int>::iterator i = core->getData()->internalPickups.find(streamerid);
		if (i != core->getData()->internalPickups.end())
		{
			return static_cast<cell>(i->second);
		}
		return 0;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(streamerid);
		if (i != core->getData()->internalActors.end())
		{
			return static_cast<cell>(i->second);
		}
		return 0;
	}
	}
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		switch (type)
		{
		case STREAMER_TYPE_OBJECT:
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(playerid);
			if (i != p->second.internalObjects.end()) {
				return static_cast<cell>(i->second);
			}
			return 0;
		}
		case STREAMER_TYPE_CP:
		{
			if (p->second.visibleCheckpoint == streamerid) {
				return 1;
			}
			return 0;
		}
		case STREAMER_TYPE_RACE_CP:
		{
			if (p->second.visibleRaceCheckpoint == streamerid) {
				return 1;
			}
			return 0;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.find(streamerid);
			if (i != p->second.internalMapIcons.end()) {
				return static_cast<cell>(i->second);
			}
			return 0;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(streamerid);
			if (i != p->second.internalTextLabels.end()) {
				return static_cast<cell>(i->second);
			}
		}
		case STREAMER_TYPE_AREA:
		{
			boost::unordered_set<int>::iterator i = p->second.internalAreas.find(streamerid);
			if (i != p->second.internalAreas.end()) {
				return *i;
			}
			return 0;
		}
		default:
		{
			Utility::logError("Streamer_GetItemInternalID: Invalid type specified.");
			return 0;
		}
		}
	}
	return 0;
}

int Streamer_GetItemStreamerID(int playerid, int type, int internalid) {
	switch (type)
	{
	case STREAMER_TYPE_PICKUP:
	{
		for (boost::unordered_map<int, int>::iterator i = core->getData()->internalPickups.begin(); i != core->getData()->internalPickups.end(); ++i) {
			if (i->second == internalid) {
				return i->first;
			}
		}
		return 0;
	}
	case STREAMER_TYPE_ACTOR:
	{
		for (boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i) {
			if (i->second == internalid) {
				return i->first;
			}
		}
		return 0;
	}
	}
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		switch (type)
		{
		case STREAMER_TYPE_OBJECT:
		{
			for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i) {
				if (i->second == internalid) {
					return i->first;
				}
			}
			return 0;
		}
		case STREAMER_TYPE_CP:
		{
			if (p->second.visibleCheckpoint == internalid) {
				return 1;
			}
			return 0;
		}
		case STREAMER_TYPE_RACE_CP:
		{
			if (p->second.visibleRaceCheckpoint == internalid) {
				return 1;
			}
			return 0;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
			for (boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.begin(); i != p->second.internalMapIcons.end(); ++i) {
				if (i->second == internalid) {
					return i->first;
				}
			}
			return 0;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			for (boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.begin(); i != p->second.internalTextLabels.end(); ++i) {
				if (i->second == internalid) {
					return i->first;
				}
			}
			return 0;
		}
		case STREAMER_TYPE_AREA:
		{
			boost::unordered_set<int>::iterator i = p->second.internalAreas.find(internalid);
			if (i != p->second.internalAreas.end()) {
				return *i;
			}
			return 0;
		}
		default:
		{
			Utility::logError("Streamer_GetItemStreamerID: Invalid type specified.");
			return 0;
		}
		}
	}
	return 0;
}

int Streamer_IsItemVisible(int playerid, int type, int id) {
	switch (type)
	{
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, int>::iterator i = core->getData()->internalPickups.find(id);
		if (i != core->getData()->internalPickups.end())
		{
			return 1;
		}
		return 0;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(id);
		if (i != core->getData()->internalActors.end()) {
			return 1;
		}
		return 0;
	}
	}
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		switch (type)
		{
		case STREAMER_TYPE_OBJECT:
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(id);
			if (i != p->second.internalObjects.end()) {
				return 1;
			}
			return 0;
		}
		case STREAMER_TYPE_CP:
		{
			if (p->second.visibleCheckpoint == id) {
				return 1;
			}
			return 0;
		}
		case STREAMER_TYPE_RACE_CP:
		{
			if (p->second.visibleRaceCheckpoint == id) {
				return 1;
			}
			return 0;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.find(id);
			if (i != p->second.internalMapIcons.end()) {
				return 1;
			}
			return 0;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(id);
			if (i != p->second.internalTextLabels.end()) {
				return 1;
			}
		}
		case STREAMER_TYPE_AREA:
		{
			boost::unordered_set<int>::iterator i = p->second.internalAreas.find(id);
			if (i != p->second.internalAreas.end()) {
				return 1;
			}
			return 0;
		}
		default:
		{
			Utility::logError("Streamer_IsItemVisible: Invalid type specified.");
			return 0;
		}
		}
	}
	return 0;
}

int Streamer_DestroyAllVisibleItems(int playerid, int type, bool serverWide) {
	// TODO: <sphinx> amx
	AMX* amx = nullptr;

	switch (type)
	{
		case STREAMER_TYPE_PICKUP:
		{
			boost::unordered_map<int, int>::iterator i = core->getData()->internalPickups.begin();
			while (i != core->getData()->internalPickups.end())
			{
				boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(i->first);
				if (serverWide || (p != core->getData()->pickups.end() && p->second->amx == amx))
				{
					sampgdk::DestroyPickup(i->second);
					i = core->getData()->internalPickups.erase(i);
				}
				else
				{
					++i;
				}
			}
			return 1;
		}
		case STREAMER_TYPE_ACTOR:
		{
			boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.begin();
			while (i != core->getData()->internalActors.end())
			{
				boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(i->first);
				if (serverWide || (a != core->getData()->actors.end() && a->second->amx == amx))
				{
					sampgdk::DestroyActor(i->second);
					i = core->getData()->internalActors.erase(i);
				}
				else
				{
					++i;
				}
			}
			return 1;
		}
	}
	
	
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		switch (type)
		{
		case STREAMER_TYPE_OBJECT:
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin();
			while (i != p->second.internalObjects.end())
			{
				boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(i->first);
				if (serverWide || (o != core->getData()->objects.end() && o->second->amx == amx))
				{
					sampgdk::DestroyPlayerObject(p->first, i->second);
					i = p->second.internalObjects.erase(i);
				}
				else
				{
					++i;
				}
			}
			return 1;
		}
		case STREAMER_TYPE_CP:
		{
			if (p->second.visibleCheckpoint)
			{
				boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(p->second.visibleCheckpoint);
				if (serverWide || (c != core->getData()->checkpoints.end() && c->second->amx == amx)) {
					sampgdk::DisablePlayerCheckpoint(p->first);
					p->second.activeCheckpoint = 0;
					p->second.visibleCheckpoint = 0;
					return 1;
				}
			}
			return 0;
		}
		case STREAMER_TYPE_RACE_CP:
		{
			if (p->second.visibleRaceCheckpoint) {
				boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(p->second.visibleRaceCheckpoint);
				if (serverWide || (r != core->getData()->raceCheckpoints.end() && r->second->amx == amx)) {
					sampgdk::DisablePlayerRaceCheckpoint(p->first);
					p->second.activeRaceCheckpoint = 0;
					p->second.visibleRaceCheckpoint = 0;
					return 1;
				}
			}
			return 0;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.begin();
			while (i != p->second.internalMapIcons.end())
			{
				boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(i->first);
				if (serverWide || (m != core->getData()->mapIcons.end() && m->second->amx == amx))
				{
					sampgdk::RemovePlayerMapIcon(p->first, i->second);
					i = p->second.internalMapIcons.erase(i);
				}
				else
				{
					++i;
				}
			}
			return 1;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.begin();
			while (i != p->second.internalTextLabels.end())
			{
				boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(i->first);
				if (serverWide || (t != core->getData()->textLabels.end() && t->second->amx == amx))
				{
					sampgdk::DeletePlayer3DTextLabel(p->first, i->second);
					i = p->second.internalTextLabels.erase(i);
				}
				else
				{
					++i;
				}
			}
			return 1;
		}
		case STREAMER_TYPE_AREA:
		{
			boost::unordered_set<int>::iterator i = p->second.internalAreas.begin();
			while (i != p->second.internalAreas.end())
			{
				boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(*i);
				if (serverWide || (a != core->getData()->areas.end() && a->second->amx == amx))
				{
					i = p->second.internalAreas.erase(i);
				}
				else
				{
					++i;
				}
			}
			return 1;
		}
		default:
		{
			Utility::logError("Streamer_DestroyAllVisibleItems: Invalid type specified.");
			return 0;
		}
		}
	}
	return 0;
}

int Streamer_CountVisibleItems(int playerid, int type, bool serverWide) {
	// TODO: <sphinx> amx
	AMX* amx = nullptr;

	switch (type)
	{
	case STREAMER_TYPE_PICKUP:
	{
		return static_cast<cell>(core->getData()->internalPickups.size());
	}
	case STREAMER_TYPE_ACTOR:
	{
		return static_cast<cell>(core->getData()->internalActors.size());
	}
	}
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		switch (type)
		{
		case STREAMER_TYPE_OBJECT:
		{
			if (serverWide) {
				return static_cast<cell>(p->second.internalObjects.size());
			} else {
				int count = 0;
				for (boost::unordered_map<int, int>::iterator o = p->second.internalObjects.begin(); o != p->second.internalObjects.end(); ++o) {
					boost::unordered_map<int, Item::SharedObject>::iterator q = core->getData()->objects.find(o->first);
					if (q != core->getData()->objects.end() && q->second->amx == amx) {
						++count;
					}
				}
				return count;
			}
		}
		case STREAMER_TYPE_CP:
		{
			if (p->second.visibleCheckpoint)
			{
				boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(p->second.visibleCheckpoint);
				if (serverWide || (c != core->getData()->checkpoints.end() && c->second->amx == amx))
				{
					return 1;
				}
				return 0;
			}
		}
		case STREAMER_TYPE_RACE_CP:
		{
			if (p->second.visibleRaceCheckpoint)
			{
				boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(p->second.visibleRaceCheckpoint);
				if (serverWide || (r != core->getData()->raceCheckpoints.end() && r->second->amx == amx))
				{
					return 1;
				}
			}
			return 0;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
			if (serverWide) {
				return p->second.internalMapIcons.size();
			} else {
				int count = 0;
				for (boost::unordered_map<int, int>::iterator m = p->second.internalMapIcons.begin(); m != p->second.internalObjects.end(); ++m) {
					boost::unordered_map<int, Item::SharedMapIcon>::iterator n = core->getData()->mapIcons.find(m->first);
					if (n != core->getData()->mapIcons.end() && n->second->amx == amx) {
						++count;
					}
				}
				return count;
			}
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			if (serverWide)
			{
				return static_cast<cell>(p->second.internalTextLabels.size());
			}
			else
			{
				int count = 0;
				for (boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.begin(); i != p->second.internalTextLabels.end(); ++i) {
					boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(i->first);
					if (t != core->getData()->textLabels.end() && t->second->amx == amx) {
						++count;
					}
				}
				return count;
			}
		}
		case STREAMER_TYPE_AREA:
		{
			if (serverWide) {
				return p->second.internalAreas.size();
			} else {
				int count = 0;
				for (boost::unordered_set<int>::iterator i = p->second.internalAreas.begin(); i != p->second.internalAreas.end(); ++i) {
					boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(*i);
					if (a != core->getData()->areas.end() && a->second->amx == amx) {
						++count;
					}
				}
				return count;
			}
		}
		default:
		{
			Utility::logError("Streamer_CountVisibleItems: Invalid type specified.");
			return 0;
		}
		}
	}
	return 0;
}

int Streamer_DestroyAllItems(int type, bool serverWide) {
	// TODO: <sphinx> amx
	AMX* amx = nullptr;

	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.begin();
		while (o != core->getData()->objects.end())
		{
			if (serverWide || o->second->amx == amx) {
				o = Utility::destroyObject(o);
			} else {
				++o;
			}
		}
		return 1;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.begin();
		while (p != core->getData()->pickups.end())
		{
			if (serverWide || p->second->amx == amx) {
				p = Utility::destroyPickup(p);
			} else {
				++p;
			}
		}
		return 1;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin();
		while (c != core->getData()->checkpoints.end())
		{
			if (serverWide || c->second->amx == amx) {
				c = Utility::destroyCheckpoint(c);
			} else {
				++c;
			}
		}
		return 1;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin();
		while (r != core->getData()->raceCheckpoints.end())
		{
			if (serverWide || r->second->amx == amx) {
				r = Utility::destroyRaceCheckpoint(r);
			} else {
				++r;
			}
		}
		return 1;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.begin();
		while (m != core->getData()->mapIcons.end())
		{
			if (serverWide || m->second->amx == amx) {
				m = Utility::destroyMapIcon(m);
			} else {
				++m;
			}
		}
		return 1;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.begin();
		while (t != core->getData()->textLabels.end())
		{
			if (serverWide || t->second->amx == amx) {
				t = Utility::destroyTextLabel(t);
			} else {
				++t;
			}
		}
		return 1;
	}
	case STREAMER_TYPE_AREA:
	{
		Utility::executeFinalAreaCallbacksForAllAreas(amx, serverWide);
		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin();
		while (a != core->getData()->areas.end())
		{
			if (serverWide || a->second->amx == amx) {
				a = Utility::destroyArea(a);
			} else {
				++a;
			}
		}
		return 1;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.begin();
		while (a != core->getData()->actors.end())
		{
			if (serverWide || a->second->amx == amx) {
				a = Utility::destroyActor(a);
			} else {
				++a;
			}
		}
		return 1;
	}
	default:
	{
		Utility::logError("Streamer_DestroyAllItems: Invalid type specified");
		return 0;
	}
	}
	return 0;
}

int Streamer_CountItems(int type, bool serverWide) {
	// TODO: <sphinx> amx
	AMX* amx = nullptr;

	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		if (serverWide) {
			return static_cast<cell>(core->getData()->objects.size());
		} else {
			int count = 0;
			for (boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.begin(); o != core->getData()->objects.end(); ++o)
			{
				if (o->second->amx == amx) {
					++count;
				}
			}
			return count;
		}
	}
	case STREAMER_TYPE_PICKUP:
	{
		if (serverWide) {
			return static_cast<cell>(core->getData()->pickups.size());
		} else {
			int count = 0;
			for (boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.begin(); p != core->getData()->pickups.end(); ++p)
			{
				if (p->second->amx == amx)
				{
					++count;
				}
			}
			return count;
		}
	}
	case STREAMER_TYPE_CP:
	{
		if (serverWide) {
			return static_cast<cell>(core->getData()->checkpoints.size());
		} else {
			int count = 0;
			for (boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin(); c != core->getData()->checkpoints.end(); ++c)
			{
				if (c->second->amx == amx)
				{
					++count;
				}
			}
			return count;
		}
	}
	case STREAMER_TYPE_RACE_CP:
	{
		if (serverWide) {
			return static_cast<cell>(core->getData()->raceCheckpoints.size());
		} else {
			int count = 0;
			for (boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin(); r != core->getData()->raceCheckpoints.end(); ++r)
			{
				if (r->second->amx == amx)
				{
					++count;
				}
			}
			return count;
		}
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		if (serverWide) {
			return static_cast<cell>(core->getData()->mapIcons.size());
		} else {
			int count = 0;
			for (boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.begin(); m != core->getData()->mapIcons.end(); ++m)
			{
				if (m->second->amx == amx)
				{
					++count;
				}
			}
			return count;
		}
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		if (serverWide) {
			return static_cast<cell>(core->getData()->textLabels.size());
		} else {
			int count = 0;
			for (boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.begin(); t != core->getData()->textLabels.end(); ++t)
			{
				if (t->second->amx == amx)
				{
					++count;
				}
			}
			return count;
		}
	}
	case STREAMER_TYPE_AREA:
	{
		if (serverWide) {
			return static_cast<cell>(core->getData()->areas.size());
		} else {
			int count = 0;
			for (boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
			{
				if (a->second->amx == amx)
				{
					++count;
				}
			}
			return count;
		}
	}
	case STREAMER_TYPE_ACTOR:
	{
		if (serverWide) {
			return static_cast<cell>(core->getData()->actors.size());
		} else{
			int count = 0;
			for (boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.begin(); a != core->getData()->actors.end(); ++a)
			{
				if (a->second->amx == amx)
				{
					++count;
				}
			}
			return count;
		}
	}
	default:
	{
		Utility::logError("Streamer_CountItems: Invalid type specified.");
		return 0;
	}
	}
	return 0;
}

std::vector<int> Streamer_GetNearbyItems(float x, float y, float z, int type, float prange) {
	Eigen::Vector2f position2D = Eigen::Vector2f(x, y);
	Eigen::Vector3f position3D = Eigen::Vector3f(x, y, z);
	
	float range = prange*prange;

	std::multimap<float, int> orderedItems;
	std::vector<SharedCell> pointCells;
	core->getGrid()->findMinimalCellsForPoint(position2D, pointCells);

	switch (type)
	{
		case STREAMER_TYPE_OBJECT:
		{
			for (std::vector<SharedCell>::const_iterator p = pointCells.begin(); p != pointCells.end(); ++p) {
				for (boost::unordered_map<int, Item::SharedObject>::const_iterator o = (*p)->objects.begin(); o != (*p)->objects.end(); ++o) {
					float distance = 0.0f;
					if (o->second->attach) {
						distance = static_cast<float>(boost::geometry::comparable_distance(position3D, o->second->attach->position));
					} else {
						distance = static_cast<float>(boost::geometry::comparable_distance(position3D, o->second->position));
					}

					if (distance < range) {
						orderedItems.insert(std::pair<float, int>(distance, o->first));
					}
				}
			}
			break;
		}
		case STREAMER_TYPE_PICKUP:
		{
			for (std::vector<SharedCell>::const_iterator p = pointCells.begin(); p != pointCells.end(); ++p) {
				for (boost::unordered_map<int, Item::SharedPickup>::const_iterator q = (*p)->pickups.begin(); q != (*p)->pickups.end(); ++q) {
					float distance = static_cast<float>(boost::geometry::comparable_distance(position3D, q->second->position));
					if (distance < range) {
						orderedItems.insert(std::pair<float, int>(distance, q->first));
					}
				}
			}
			break;
		}
		case STREAMER_TYPE_CP:
		{
			for (std::vector<SharedCell>::const_iterator p = pointCells.begin(); p != pointCells.end(); ++p) {
				for (boost::unordered_map<int, Item::SharedCheckpoint>::const_iterator c = (*p)->checkpoints.begin(); c != (*p)->checkpoints.end(); ++c) {
					float distance = static_cast<float>(boost::geometry::comparable_distance(position3D, c->second->position));
					if (distance < range) {
						orderedItems.insert(std::pair<float, int>(distance, c->first));
					}
				}
			}
			break;
		}
		case STREAMER_TYPE_RACE_CP:
		{
			for (std::vector<SharedCell>::const_iterator p = pointCells.begin(); p != pointCells.end(); ++p) {
				for (boost::unordered_map<int, Item::SharedRaceCheckpoint>::const_iterator r = (*p)->raceCheckpoints.begin(); r != (*p)->raceCheckpoints.end(); ++r) {
					float distance = static_cast<float>(boost::geometry::comparable_distance(position3D, r->second->position));
					if (distance < range) {
						orderedItems.insert(std::pair<float, int>(distance, r->first));
					}
				}
			}
			break;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
			for (std::vector<SharedCell>::const_iterator p = pointCells.begin(); p != pointCells.end(); ++p) {
				for (boost::unordered_map<int, Item::SharedMapIcon>::const_iterator m = (*p)->mapIcons.begin(); m != (*p)->mapIcons.end(); ++m) {
					float distance = static_cast<float>(boost::geometry::comparable_distance(position3D, m->second->position));
					if (distance < range) {
						orderedItems.insert(std::pair<float, int>(distance, m->first));
					}
				}
			}
			break;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			for (std::vector<SharedCell>::const_iterator p = pointCells.begin(); p != pointCells.end(); ++p) {
				for (boost::unordered_map<int, Item::SharedTextLabel>::const_iterator t = (*p)->textLabels.begin(); t != (*p)->textLabels.end(); ++t) {
					float distance = static_cast<float>(boost::geometry::comparable_distance(position3D, t->second->position));
					if (distance < range) {
						orderedItems.insert(std::pair<float, int>(distance, t->first));
					}
				}
			}
			break;
		}
		case STREAMER_TYPE_AREA:
		{
			for (std::vector<SharedCell>::const_iterator p = pointCells.begin(); p != pointCells.end(); ++p) {
				for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = (*p)->areas.begin(); a != (*p)->areas.end(); ++a) {
					boost::variant<Polygon2D, Box2D, Box3D, Eigen::Vector2f, Eigen::Vector3f> position;
					if (a->second->attach) {
						position = a->second->position;
					} else {
						position = a->second->position;
					}

					float distance = 0.0f;
					switch (a->second->type)
					{
						case STREAMER_AREA_TYPE_CIRCLE:
						case STREAMER_AREA_TYPE_CYLINDER:
						{
							distance = static_cast<float>(boost::geometry::distance(position2D, boost::get<Eigen::Vector2f>(position)));
							break;
						}
						case STREAMER_AREA_TYPE_SPHERE:
						{
							distance = static_cast<float>(boost::geometry::comparable_distance(position3D, boost::get<Eigen::Vector3f>(position)));
							break;
						}
						case STREAMER_AREA_TYPE_RECTANGLE:
						{
							Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Box2D>(position));
							distance = static_cast<float>(boost::geometry::comparable_distance(position2D, centroid));
							break;
						}
						case STREAMER_AREA_TYPE_CUBOID:
						{
							Eigen::Vector3f centroid = boost::geometry::return_centroid<Eigen::Vector3f>(boost::get<Box3D>(position));
							distance = static_cast<float>(boost::geometry::comparable_distance(position3D, centroid));
							break;
						}
						case STREAMER_AREA_TYPE_POLYGON:
						{
							Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Polygon2D>(position));
							distance = static_cast<float>(boost::geometry::comparable_distance(position2D, centroid));
							break;
						}
					}
					if (distance < range)
					{
						orderedItems.insert(std::pair<float, int>(distance, a->first));
					}
				}
			}
			break;
		}
		case STREAMER_TYPE_ACTOR:
		{
			for (std::vector<SharedCell>::const_iterator p = pointCells.begin(); p != pointCells.end(); ++p) {
				for (boost::unordered_map<int, Item::SharedActor>::const_iterator a = (*p)->actors.begin(); a != (*p)->actors.end(); ++a) {
					float distance = static_cast<float>(boost::geometry::comparable_distance(position3D, a->second->position));
					if (distance < range) {
						orderedItems.insert(std::pair<float, int>(distance, a->first));
					}
				}
			}
			break;
		}
		default:
		{
			Utility::logError("Streamer_GetNearbyItems: Invalid type specified.");
			return 0;
		}
	}
	std::vector<int> finalItems;
	for (std::multimap<float, int>::iterator i = orderedItems.begin(); i != orderedItems.end(); ++i) {
		finalItems.push_back(i->second);
	}

	return finalItems;
}

int Streamer_GetItemOffset(int type, int id, float &x, float &y, float &z) {
	Eigen::Vector3f positionOffset = Eigen::Vector3f::Zero();
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
		if (o != core->getData()->objects.end()) {
			positionOffset = o->second->positionOffset;
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
		if (p != core->getData()->pickups.end()) {
			positionOffset = p->second->positionOffset;
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
		if (c != core->getData()->checkpoints.end()) {
			positionOffset = c->second->positionOffset;
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
		if (r != core->getData()->raceCheckpoints.end()) {
			positionOffset = r->second->positionOffset;
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
		if (m != core->getData()->mapIcons.end()) {
			positionOffset = m->second->positionOffset;
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end()) {
			positionOffset = t->second->positionOffset;
			break;
		}
		return 0;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
		if (a != core->getData()->actors.end()) {
			positionOffset = a->second->positionOffset;
			break;
		}
		return 0;
	}
	default:
	{
		Utility::logError("Streamer_GetItemPosOffset: Invalid type specified.");
		return 0;
	}
	}
	x = positionOffset[0];
	y = positionOffset[1];
	z = positionOffset[2];
	return 1;
}

int Streamer_SetItemOffset(int type, int id, float x, float y, float z) {
	Eigen::Vector3f positionOffset = Eigen::Vector3f::Zero();
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
		if (o != core->getData()->objects.end())
		{
			o->second->positionOffset = Eigen::Vector3f(x, y, z);
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
		if (p != core->getData()->pickups.end())
		{
			p->second->positionOffset = Eigen::Vector3f(x, y, z);
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
		if (c != core->getData()->checkpoints.end())
		{
			c->second->positionOffset = Eigen::Vector3f(x, y, z);
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
		if (r != core->getData()->raceCheckpoints.end())
		{
			r->second->positionOffset = Eigen::Vector3f(x, y, z);
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
		if (m != core->getData()->mapIcons.end())
		{
			m->second->positionOffset = Eigen::Vector3f(x, y, z);
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end())
		{
			t->second->positionOffset = Eigen::Vector3f(x, y, z);
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
		if (a != core->getData()->actors.end())
		{
			a->second->positionOffset = Eigen::Vector3f(x, y, z);
			return 1;
		}
		break;
	}
	default:
	{
		Utility::logError("Streamer_SetItemPosOffset: Invalid type specified.");
		return 0;
	}
	}
	return 0;
}

std::vector<int> Streamer_GetAllVisibleItems(int playerid, int type) {
	std::multimap<float, int> orderedItems;
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		switch (type) {
			case STREAMER_TYPE_OBJECT:
			{
				for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i) {
					boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(i->first);
					if (o != core->getData()->objects.end()) {
						float distance = 0.0f;
						if (o->second->attach) {
							distance = static_cast<float>(boost::geometry::comparable_distance(p->second.position, o->second->attach->position));
						} else {
							distance = static_cast<float>(boost::geometry::comparable_distance(p->second.position, o->second->position));
						}
						orderedItems.insert(std::pair<float, int>(distance, o->first));
					}
				}
				break;
			}
			case STREAMER_TYPE_PICKUP:
			{
				for (boost::unordered_map<int, int>::iterator i = core->getData()->internalPickups.begin(); i != core->getData()->internalPickups.end(); ++i) {
					boost::unordered_map<int, Item::SharedPickup>::iterator q = core->getData()->pickups.find(i->first);
					if (q != core->getData()->pickups.end()) {
						float distance = static_cast<float>(boost::geometry::comparable_distance(p->second.position, q->second->position));
						orderedItems.insert(std::pair<float, int>(distance, q->first));
					}
				}
				break;
			}
			case STREAMER_TYPE_MAP_ICON:
			{
				for (boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.begin(); i != p->second.internalMapIcons.end(); ++i) {
					boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(i->first);
					if (m != core->getData()->mapIcons.end()) {
						float distance = static_cast<float>(boost::geometry::comparable_distance(p->second.position, m->second->position));
						orderedItems.insert(std::pair<float, int>(distance, m->first));
					}
				}
				break;
			}
			case STREAMER_TYPE_3D_TEXT_LABEL:
			{
				for (boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.begin(); i != p->second.internalTextLabels.end(); ++i) {
					boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(i->first);
					if (t != core->getData()->textLabels.end()) {
						float distance = 0.0f;
						if (t->second->attach) {
							distance = static_cast<float>(boost::geometry::comparable_distance(p->second.position, t->second->attach->position));
						} else {
							distance = static_cast<float>(boost::geometry::comparable_distance(p->second.position, t->second->position));
						}
						orderedItems.insert(std::pair<float, int>(distance, t->first));
					}
				}
				break;
			}
			case STREAMER_TYPE_ACTOR:
			{
				for (boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i) {
					boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(i->first);
					if (a != core->getData()->actors.end()) {
						float distance = static_cast<float>(boost::geometry::comparable_distance(p->second.position, a->second->position));
						orderedItems.insert(std::pair<float, int>(distance, a->first));
					}
				}
				break;
			}
		}
	}

	std::vector<int> finalItems;
	for (std::multimap<float, int>::iterator i = orderedItems.begin(); i != orderedItems.end(); ++i) {
		finalItems.push_back(i->second);
	}
	return finalItems;
}

STREAMER_END_NS