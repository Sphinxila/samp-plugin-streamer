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
		int objectID = 0;
		for (boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.begin(); o != core->getData()->objects.end(); ++o)
		{
			if (o->first > objectID)
			{
				objectID = o->first;
			}
		}
		return static_cast<cell>(objectID + 1);
	}
	case STREAMER_TYPE_PICKUP:
	{
		int pickupID = 0;
		for (boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.begin(); p != core->getData()->pickups.end(); ++p)
		{
			if (p->first > pickupID)
			{
				pickupID = p->first;
			}
		}
		return static_cast<cell>(pickupID + 1);
	}
	case STREAMER_TYPE_CP:
	{
		int checkpointID = 0;
		for (boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin(); c != core->getData()->checkpoints.end(); ++c)
		{
			if (c->first > checkpointID)
			{
				checkpointID = c->first;
			}
		}
		return static_cast<cell>(checkpointID + 1);
	}
	case STREAMER_TYPE_RACE_CP:
	{
		int raceCheckpointID = 0;
		for (boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin(); r != core->getData()->raceCheckpoints.end(); ++r)
		{
			if (r->first > raceCheckpointID)
			{
				raceCheckpointID = r->first;
			}
		}
		return static_cast<cell>(raceCheckpointID + 1);
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		int mapIconID = 0;
		for (boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.begin(); m != core->getData()->mapIcons.end(); ++m)
		{
			if (m->first > mapIconID)
			{
				mapIconID = m->first;
			}
		}
		return static_cast<cell>(mapIconID + 1);
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		int textLabelID = 0;
		for (boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.begin(); t != core->getData()->textLabels.end(); ++t)
		{
			if (t->first > textLabelID)
			{
				textLabelID = t->first;
			}
		}
		return static_cast<cell>(textLabelID + 1);
	}
	case STREAMER_TYPE_AREA:
	{
		int areaID = 0;
		for (boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
		{
			if (a->first > areaID)
			{
				areaID = a->first;
			}
		}
		return static_cast<cell>(areaID + 1);
	}
	case STREAMER_TYPE_ACTOR:
	{
		int actorID = 0;
		for (boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.begin(); a != core->getData()->actors.end(); ++a)
		{
			if (a->first > actorID)
			{
				actorID = a->first;
			}
		}
		return static_cast<cell>(actorID + 1);
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
