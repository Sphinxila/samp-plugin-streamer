#ifndef STREAMER_HPP
#define STREAMER_HPP

#pragma once

#include "config.hpp"
#include "actors.hpp"
#include "areas.hpp"
#include "checkpoints.hpp"
#include "deprecated.hpp"
#include "extended.hpp"
#include "manipulation.hpp"
#include "map-icons.hpp"
#include "miscellaneous.hpp"
#include "objects.hpp"
#include "pickups.hpp"
#include "race-checkpoints.hpp"
#include "settings.hpp"
#include "settings.hpp"
#include "text-labels.hpp"
#include "updates.hpp"

STREAMER_BEGIN_NS

namespace AmxEvent {
	unsigned int Supports();
	bool Load(void **ppData);
	void Unload();
	int AmxLoad(AMX *amx);
	int AmxUnload(AMX *amx);
	void ProcessTick();
};

namespace event {
	// SampGDK
	bool OnPlayerConnect(int playerid);
	bool OnPlayerDisconnect(int playerid, int reason);
	bool OnPlayerSpawn(int playerid);
	bool OnPlayerRequestClass(int playerid, int classid);
	bool OnPlayerEnterCheckpoint(int playerid);
	bool OnPlayerLeaveCheckpoint(int playerid);
	bool OnPlayerEnterRaceCheckpoint(int playerid);
	bool OnPlayerLeaveRaceCheckpoint(int playerid);
	bool OnPlayerPickUpPickup(int playerid, int pickupid);
	bool OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ);
	bool OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float x, float y, float z);
	bool OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float x, float y, float z);
	bool OnPlayerGiveDamageActor(int playerid, int actorid, float amount, int weaponid, int bodypart);
	bool OnActorStreamIn(int actorid, int forplayerid);
	bool OnActorStreamOut(int actorid, int forplayerid);
};

STREAMER_END_NS
#endif