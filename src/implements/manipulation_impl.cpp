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

#include <streamer/miscellaneous.hpp>

#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>

STREAMER_BEGIN_NS

int Streamer_GetUpperBound(int type) {
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		int objectId = 0;
		for (boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.begin(); o != core->getData()->objects.end(); ++o)
		{
			if (o->first > objectId)
			{
				objectId = o->first;
			}
		}
		return static_cast<cell>(objectId + 1);
	}
	case STREAMER_TYPE_PICKUP:
	{
		int pickupId = 0;
		for (boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.begin(); p != core->getData()->pickups.end(); ++p)
		{
			if (p->first > pickupId)
			{
				pickupId = p->first;
			}
		}
		return static_cast<cell>(pickupId + 1);
	}
	case STREAMER_TYPE_CP:
	{
		int checkpointId = 0;
		for (boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin(); c != core->getData()->checkpoints.end(); ++c)
		{
			if (c->first > checkpointId)
			{
				checkpointId = c->first;
			}
		}
		return static_cast<cell>(checkpointId + 1);
	}
	case STREAMER_TYPE_RACE_CP:
	{
		int raceCheckpointId = 0;
		for (boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin(); r != core->getData()->raceCheckpoints.end(); ++r)
		{
			if (r->first > raceCheckpointId)
			{
				raceCheckpointId = r->first;
			}
		}
		return static_cast<cell>(raceCheckpointId + 1);
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		int mapIconId = 0;
		for (boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.begin(); m != core->getData()->mapIcons.end(); ++m)
		{
			if (m->first > mapIconId)
			{
				mapIconId = m->first;
			}
		}
		return static_cast<cell>(mapIconId + 1);
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		int textLabelId = 0;
		for (boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.begin(); t != core->getData()->textLabels.end(); ++t)
		{
			if (t->first > textLabelId)
			{
				textLabelId = t->first;
			}
		}
		return static_cast<cell>(textLabelId + 1);
	}
	case STREAMER_TYPE_AREA:
	{
		int areaId = 0;
		for (boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
		{
			if (a->first > areaId)
			{
				areaId = a->first;
			}
		}
		return static_cast<cell>(areaId + 1);
	}
	case STREAMER_TYPE_ACTOR:
	{
		int actorId = 0;
		for (boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.begin(); a != core->getData()->actors.end(); ++a)
		{
			if (a->first > actorId)
			{
				actorId = a->first;
			}
		}
		return static_cast<cell>(actorId + 1);
	}
	default:
	{
		Utility::logError("Streamer_GetUpperBound: Invalid type specified.");
		return 0;
	}
	}
	return 0;
}

STREAMER_END_NS
