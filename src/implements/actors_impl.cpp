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

#include <streamer/actors.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <Eigen/Core>

STREAMER_BEGIN_NS

int CreateDynamicActor(int modelid, float x, float y, float z, float r, bool invulnerable,
	float health, int worldid, int interiorid, int playerid, float streamdistance, int areaid, int priority) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_ACTOR) == core->getData()->actors.size()) {
		return 0;
	}

	int actorID = Item::Actor::identifier.get();
	Item::SharedActor actor(new Item::Actor);
	//actor->amx = amx;
	actor->actorID = actorID;
	actor->inverseAreaChecking = false;
	actor->originalComparableStreamDistance = -1.0f;
	actor->positionOffset = Eigen::Vector3f::Zero();
	actor->modelID = modelid;
	actor->position = Eigen::Vector3f(x, y, z);
	actor->rotation = r;
	actor->invulnerable = invulnerable;
	actor->health = health;

	Utility::addToContainer(actor->worlds, worldid);
	Utility::addToContainer(actor->interiors, interiorid);
	Utility::addToContainer(actor->players, playerid);

	actor->comparableStreamDistance = streamdistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamdistance : streamdistance * streamdistance;
	actor->streamDistance = streamdistance;

	Utility::addToContainer(actor->areas, areaid);
	actor->priority = priority;

	core->getGrid()->addActor(actor);
	core->getData()->actors.insert(std::make_pair(actorID, actor));
	return actorID;
}

int DestroyDynamicActor(int id) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		Utility::destroyActor(a);
		return 1;
	}
	return 0;
}

int IsValidDynamicActor(int id) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		return 1;
	}
	return 0;
}

int ApplyDynamicActorAnimation(int id, std::string lib, std::string name, float delta, bool loop, bool lockx, bool locky, bool freeze, int time) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		a->second->anim = boost::intrusive_ptr<Item::Actor::Anim>(new Item::Actor::Anim);
		a->second->anim->lib = lib;
		a->second->anim->name = name;
		a->second->anim->delta = delta;
		a->second->anim->loop = loop;
		a->second->anim->lockx = lockx;
		a->second->anim->locky = locky;
		a->second->anim->freeze = freeze;
		a->second->anim->time = time;

		boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(a->first);
		if (i != core->getData()->internalActors.end()) {
			sampgdk::ApplyActorAnimation(i->second, a->second->anim->lib.c_str(), a->second->anim->name.c_str(), a->second->anim->delta, a->second->anim->loop, a->second->anim->lockx, a->second->anim->locky, a->second->anim->freeze, a->second->anim->time);
		}
		return 1;
	}
	return 0;
}

int ClearDynamicActorAnimations(int id) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		a->second->anim = NULL;

		boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(a->first);
		if (i != core->getData()->internalActors.end()) {
			sampgdk::ClearActorAnimations(i->second);
		}
		return 1;
	}
	return 0;
}

int GetPlayerTargetDynamicActor(int id) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(id);
	if (p != core->getData()->players.end()) {
		int actorid = sampgdk::GetPlayerTargetActor(p->second.playerID);
		for (boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i) {
			if (i->second == actorid) {
				return i->first;
			}
		}
	}
	return 0;
}

int SetDynamicActorFacingAngle(int id, float angle) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		a->second->rotation = angle;

		boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(a->first);
		if (i != core->getData()->internalActors.end()) {
			sampgdk::DestroyActor(i->second);
			i->second = sampgdk::CreateActor(a->second->modelID, a->second->position[0], a->second->position[1], a->second->position[2], a->second->rotation);
			sampgdk::SetActorInvulnerable(i->second, a->second->invulnerable);
			sampgdk::SetActorHealth(i->second, a->second->health);
			sampgdk::SetActorVirtualWorld(i->second, a->second->worldID);

			if (a->second->anim) {
				sampgdk::ApplyActorAnimation(i->second, a->second->anim->lib.c_str(), a->second->anim->name.c_str(), a->second->anim->delta, a->second->anim->loop, a->second->anim->lockx, a->second->anim->locky, a->second->anim->freeze, a->second->anim->time);
			}
		}
		return 1;
	}
	return 0;
}

int SetDynamicActorHealth(int id, float health) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		a->second->health = health;

		boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(a->first);
		if (i != core->getData()->internalActors.end()) {
			sampgdk::SetActorHealth(i->second, a->second->health);
		}
		return 1;
	}
	return 0;
}

int SetDynamicActorInvulnerable(int id, bool invulnerable) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		a->second->invulnerable = invulnerable;

		boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(a->first);
		if (i != core->getData()->internalActors.end()) {
			sampgdk::DestroyActor(i->second);
			i->second = sampgdk::CreateActor(a->second->modelID, a->second->position[0], a->second->position[1], a->second->position[2], a->second->rotation);
			sampgdk::SetActorInvulnerable(i->second, a->second->invulnerable);
			sampgdk::SetActorHealth(i->second, a->second->health);
			sampgdk::SetActorVirtualWorld(i->second, a->second->worldID);

			if (a->second->anim) {
				sampgdk::ApplyActorAnimation(i->second, a->second->anim->lib.c_str(), a->second->anim->name.c_str(), a->second->anim->delta, a->second->anim->loop, a->second->anim->lockx, a->second->anim->locky, a->second->anim->freeze, a->second->anim->time);
			}
		}
		return 1;
	}
	return 0;
}

int SetDynamicActorPos(int id, float x, float y, float z) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		a->second->position[0] = x;
		a->second->position[1] = y;
		a->second->position[2] = z;

		boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(a->first);
		if (i != core->getData()->internalActors.end()) {
			core->getGrid()->removeActor(a->second, true);
			sampgdk::SetActorPos(i->second, a->second->position[0], a->second->position[1], a->second->position[2]);
		}
		return 1;
	}
	return 0;
}

int SetDynamicActorVirtualWorld(int id, int value) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		Utility::setFirstValueInContainer(a->second->worlds, value);
		boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(a->first);
		if (i != core->getData()->internalActors.end()) {
			sampgdk::SetActorVirtualWorld(i->second, a->second->worldID);
		}
		return 1;
	}
	return 0;
}

int GetDynamicActorFacingAngle(int id, float r) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		a->second->rotation = r;
		return 1;
	}
	return 0;
}

int GetDynamicActorHealth(int id, float health)
{
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		a->second->health = health;
		return 1;
	}
	return 0;
}

int GetDynamicActorPos(int id, float x, float y, float z) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		a->second->position[0] = x;
		a->second->position[1] = y;
		a->second->position[2] = z;
		return 1;
	}
	return 0;
}

int GetDynamicActorVirtualWorld(int id)
{
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		return Utility::getFirstValueInContainer(a->second->worlds);
	}
	return 0;
}

int IsDynamicActorInvulnerable(int id)
{
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
	if (a != core->getData()->actors.end()) {
		return a->second->invulnerable;
	}
	return 0;
}

int IsDynamicActorStreamedIn(int playerID, int actorID)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerID);
	if (p != core->getData()->players.end()) {
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(actorID);

		if (a != core->getData()->actors.end()) {
			boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(a->first);
			if (i != core->getData()->internalActors.end()) {
				return sampgdk::IsActorStreamedIn(i->second, p->first);
			}
		}
	}
	return 0;
}

int GetPlayerCameraTargetDynActor(int playerID)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerID);
	if (p != core->getData()->players.end()) {
		int actorid = sampgdk::GetPlayerCameraTargetActor(p->second.playerID);
		if (actorid != INVALID_ACTOR_ID) {
			for (boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i) {
				if (i->second == actorid) {
					return i->first;
				}
			}
		}
	}
	return 0;
}

int IsDynamicActorStreamedIn(int playerid, int actorid) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(actorid);
		if (a != core->getData()->actors.end()) {
			boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(a->first);
			if (i != core->getData()->internalActors.end()) {
				return sampgdk::IsActorStreamedIn(i->second, p->first);
			}
		}
	}
	return 0;
}

int GetDynamicActorVirtualWorld(int actorid) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(actorid);
	if (a != core->getData()->actors.end()) {
		return Utility::getFirstValueInContainer(a->second->worlds);
	}
	return 0;
}

int SetDynamicActorVirtualWorld(int actorid, int worldid) {
	boost::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(actorid);
	if (a != core->getData()->actors.end()) {
		Utility::setFirstValueInContainer(a->second->worlds, worldid);
		boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.find(a->first);
		if (i != core->getData()->internalActors.end()) {
			a->second->worldID = !a->second->worlds.empty() ? worldid : 0;
			sampgdk::SetActorVirtualWorld(i->second, a->second->worldID);
		}
		return 1;
	}
	return 0;
}

STREAMER_END_NS