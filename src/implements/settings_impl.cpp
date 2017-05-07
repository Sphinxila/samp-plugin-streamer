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

#include <streamer/settings.hpp>

#include <boost/scoped_ptr.hpp>

STREAMER_BEGIN_NS

int Streamer_GetTickRate() {
	return core->getStreamer()->getTickRate();
}

int Streamer_SetTickRate(std::size_t size) {
	return core->getStreamer()->setTickRate(size);
}

int Streamer_GetPlayerTickRate(int id) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(id);
	if (p != core->getData()->players.end()) {
		return p->second.tickRate;
	}
	return 0;
}

int Streamer_SetPlayerTickRate(int id, std::size_t size) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(id);
	if (p != core->getData()->players.end()) {
		p->second.tickRate = size;
		return 1;
	}
	return 0;
}

int Streamer_GetChunkTickRate(std::size_t type, std::size_t player) {
	return Utility::getChunkTickRate(type, player);
}

bool Streamer_SetChunkTickRate(int type, std::size_t value, int playerid) {
	return Utility::setChunkTickRate(type, value, playerid);
}

size_t Streamer_GetChunkSize(int type) {
	return core->getStreamer()->getChunkSize(type);
}

bool Streamer_SetChunkSize(int type, std::size_t size) {
	return core->getStreamer()->setChunkSize(type, size);
}

int Streamer_GetMaxItems(int type) {
	return core->getData()->getGlobalMaxItems(type);
}

bool Streamer_SetMaxItems(int type, std::size_t value) {
	return core->getData()->setGlobalMaxItems(type, value);
}

size_t Streamer_GetVisibleItems(int type, int playerid) {
	return Utility::getMaxVisibleItems(type, playerid);
}

int Streamer_SetVisibleItems(int type, std::size_t value, int playerid) {
	return Utility::setMaxVisibleItems(type, value, playerid);
}

float Streamer_GetRadiusMultiplier(int type, int playerid) {
	return Utility::getRadiusMultiplier(type, playerid);
}

int Streamer_SetRadiusMultiplier(int type, float value, int playerid) {
	return Utility::setRadiusMultiplier(type, value, playerid);
}

std::vector<int> Streamer_GetTypePriority() {
	return core->getData()->typePriority;
}

void Streamer_SetTypePriority(std::vector<int> types) {
	core->getData()->typePriority = types;
}

float Streamer_GetCellDistance() {
	return core->getGrid()->getCellDistance();
}

void Streamer_SetCellDistance(float distance) {
	core->getGrid()->setCellDistance(distance * distance);
	core->getGrid()->rebuildGrid();
}

float Streamer_GetCellSize() {
	return  core->getGrid()->getCellSize();
}

void Streamer_SetCellSize(float size) {
	core->getGrid()->setCellSize(size);
	core->getGrid()->rebuildGrid();
}

int Streamer_ToggleItemStatic(int type, int id, bool toggle) {
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
		if (o != core->getData()->objects.end()) {
			if (toggle) {
				if (o->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && o->second->originalComparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF) {
					o->second->originalComparableStreamDistance = o->second->comparableStreamDistance;
					o->second->comparableStreamDistance = -1.0f;
				}
			} else {
				if (o->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && o->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
					o->second->comparableStreamDistance = o->second->originalComparableStreamDistance;
					o->second->originalComparableStreamDistance = -1.0f;
				}
			}
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
		if (p != core->getData()->pickups.end()) {
			if (toggle) {
				if (p->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && p->second->originalComparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF) {
					p->second->originalComparableStreamDistance = p->second->comparableStreamDistance;
					p->second->comparableStreamDistance = -1.0f;
				}
			} else {
				if (p->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && p->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
					p->second->comparableStreamDistance = p->second->originalComparableStreamDistance;
					p->second->originalComparableStreamDistance = -1.0f;
				}
			}
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
		if (c != core->getData()->checkpoints.end()) {
			if (toggle) {
				if (c->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && c->second->originalComparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF) {
					c->second->originalComparableStreamDistance = c->second->comparableStreamDistance;
					c->second->comparableStreamDistance = -1.0f;
				}
			} else {
				if (c->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && c->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
					c->second->comparableStreamDistance = c->second->originalComparableStreamDistance;
					c->second->originalComparableStreamDistance = -1.0f;
				}
			}
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
		if (r != core->getData()->raceCheckpoints.end()) {
			if (toggle) {
				if (r->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && r->second->originalComparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF) {
					r->second->originalComparableStreamDistance = r->second->comparableStreamDistance;
					r->second->comparableStreamDistance = -1.0f;
				}
			} else {
				if (r->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && r->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
					r->second->comparableStreamDistance = r->second->originalComparableStreamDistance;
					r->second->originalComparableStreamDistance = -1.0f;
				}
			}
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
		if (m != core->getData()->mapIcons.end()) {
			if (toggle) {
				if (m->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && m->second->originalComparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF) {
					m->second->originalComparableStreamDistance = m->second->comparableStreamDistance;
					m->second->comparableStreamDistance = -1.0f;
				}
			} else {
				if (m->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && m->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
					m->second->comparableStreamDistance = m->second->originalComparableStreamDistance;
					m->second->originalComparableStreamDistance = -1.0f;
				}
			}
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end()) {
			if (toggle) {
				if (t->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && t->second->originalComparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF) {
					t->second->originalComparableStreamDistance = t->second->comparableStreamDistance;
					t->second->comparableStreamDistance = -1.0f;
				}
			} else {
				if (t->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && t->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
					t->second->comparableStreamDistance = t->second->originalComparableStreamDistance;
					t->second->originalComparableStreamDistance = -1.0f;
				}
			}
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
		if (a != core->getData()->actors.end()) {
			if (toggle) {
				if (a->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && a->second->originalComparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF) {
					a->second->originalComparableStreamDistance = a->second->comparableStreamDistance;
					a->second->comparableStreamDistance = -1.0f;
				}
			} else {
				if (a->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && a->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
					a->second->comparableStreamDistance = a->second->originalComparableStreamDistance;
					a->second->originalComparableStreamDistance = -1.0f;
				}
			}
			return 1;
		}
		break;
	}
	default:
	{
		Utility::logError("Streamer_ToggleItemStatic: Invalid type specified.");
		return 0;
	}
	}
	return 0;
}

int Streamer_IsToggleItemStatic(int type, int id) {
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
		if (o != core->getData()->objects.end()) {
			if (o->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && o->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
				return 1;
			}
		}
		return 0;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
		if (p != core->getData()->pickups.end()) {
			if (p->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && p->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
				return 1;
			}
		}
		return 0;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
		if (c != core->getData()->checkpoints.end()) {
			if (c->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && c->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
				return 1;
			}
		}
		return 0;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
		if (r != core->getData()->raceCheckpoints.end()) {
			if (r->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && r->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
				return 1;
			}
		}
		return 0;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
		if (m != core->getData()->mapIcons.end()) {
			if (m->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && m->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
				return 1;
			}
		}
		return 0;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end()) {
			if (t->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && t->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
				return 1;
			}
		}
		return 0;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
		if (a != core->getData()->actors.end()) {
			if (a->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && a->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
				return 1;
			}
		}
		return 0;
	}
	default:
	{
		Utility::logError("Streamer_IsToggleItemStatic: Invalid type specified.");
		return 0;
	}
	}
	return 0;
}

int Streamer_ToggleItemInvAreas(int type, int id, bool toggle)
{
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
		if (o != core->getData()->objects.end()) {
			o->second->inverseAreaChecking = toggle;
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
		if (p != core->getData()->pickups.end()) {
			p->second->inverseAreaChecking = toggle;
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
		if (c != core->getData()->checkpoints.end()) {
			c->second->inverseAreaChecking = toggle;
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
		if (r != core->getData()->raceCheckpoints.end()) {
			r->second->inverseAreaChecking = toggle;
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
		if (m != core->getData()->mapIcons.end()) {
			m->second->inverseAreaChecking = toggle;
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end()) {
			t->second->inverseAreaChecking = toggle;
			return 1;
		}
		break;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
		if (a != core->getData()->actors.end()) {
			a->second->inverseAreaChecking = toggle;
			return 1;
		}
		break;
	}
	default:
	{
		Utility::logError("Streamer_Toggle: Invalid type specified.");
		return 0;
	}
	}
	return 0;
}

bool Streamer_IsToggleItemAntiAreas(int type, int id) {
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
		if (o != core->getData()->objects.end()) {
			return o->second->inverseAreaChecking;
		}
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
		if (p != core->getData()->pickups.end()) {
			return p->second->inverseAreaChecking;
		}
		break;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
		if (c != core->getData()->checkpoints.end()) {
			return c->second->inverseAreaChecking;
		}
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
		if (r != core->getData()->raceCheckpoints.end()) {
			return r->second->inverseAreaChecking;
		}
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
		if (m != core->getData()->mapIcons.end()) {
			return m->second->inverseAreaChecking;
		}
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end()) {
			return t->second->inverseAreaChecking;
		}
		break;
	}
	case STREAMER_TYPE_ACTOR:
	{
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
		if (a != core->getData()->actors.end()) {
			return a->second->inverseAreaChecking;
		}
		break;
	}
	default:
	{
		Utility::logError("Streamer_IsToggleItemAntiAreas: Invalid type specified");
		return false;
	}
	}
	return false;
}

void Streamer_ToggleErrorCallback(bool toggle) {
	core->getData()->errorCallbackEnabled = toggle;
}

bool Streamer_IsToggleErrorCallback() {
	return core->getData()->errorCallbackEnabled;
}


int Streamer_ToggleItemCallbacks(int type, int itemid, bool streamCallbacks) {
	switch (type)
	{
		case STREAMER_TYPE_OBJECT:
		{
			boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(itemid);
			if (o != core->getData()->objects.end()) {
				o->second->streamCallbacks = streamCallbacks;
				return 1;
			}
			break;
		}
		case STREAMER_TYPE_PICKUP:
		{
			boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(itemid);
			if (p != core->getData()->pickups.end()) {
				p->second->streamCallbacks = streamCallbacks;
				return 1;
			}
			break;
		}
		case STREAMER_TYPE_CP:
		{
			boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(itemid);
			if (c != core->getData()->checkpoints.end()) {
				c->second->streamCallbacks = streamCallbacks;
				return 1;
			}
			break;
		}
		case STREAMER_TYPE_RACE_CP:
		{
			boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(itemid);
			if (r != core->getData()->raceCheckpoints.end()) {
				r->second->streamCallbacks = streamCallbacks;
				return 1;
			}
			break;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
			boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(itemid);
			if (m != core->getData()->mapIcons.end()) {
				m->second->streamCallbacks = streamCallbacks;
				return 1;
			}
			break;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(itemid);
			if (t != core->getData()->textLabels.end()) {
				t->second->streamCallbacks = streamCallbacks;
				return 1;
			}
			break;
		}
		default:
		{
			Utility::logError("Streamer_ToggleItemCallbacks: Invalid type specified.");
			return 0;
		}
	}
	return 0;
}

int Streamer_IsToggleItemCallbacks(int type, int itemid) {
	switch (type)
	{
		case STREAMER_TYPE_OBJECT:
		{
			boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(itemid);
			if (o != core->getData()->objects.end()) {
				return static_cast<cell>(o->second->streamCallbacks != 0);
			}
			break;
		}
		case STREAMER_TYPE_PICKUP:
		{
			boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(itemid);
			if (p != core->getData()->pickups.end()) {
				return static_cast<cell>(p->second->streamCallbacks != 0);
			}
			break;
		}
		case STREAMER_TYPE_CP:
		{
			boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(itemid);
			if (c != core->getData()->checkpoints.end()) {
				return static_cast<cell>(c->second->streamCallbacks != 0);
			}
			break;
		}
		case STREAMER_TYPE_RACE_CP:
		{
			boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(itemid);
			if (r != core->getData()->raceCheckpoints.end()) {
				return static_cast<cell>(r->second->streamCallbacks != 0);
			}
			break;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
			boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(itemid);
			if (m != core->getData()->mapIcons.end()) {
				return static_cast<cell>(m->second->streamCallbacks != 0);
			}
			break;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(itemid);
			if (t != core->getData()->textLabels.end()) {
				return static_cast<cell>(t->second->streamCallbacks != 0);
			}
			break;
		}
		default:
		{
			Utility::logError("Streamer_IsToggleItemCallbacks: Invalid type specified.");
			return 0;
		}
	}
	return 1;
}


STREAMER_END_NS