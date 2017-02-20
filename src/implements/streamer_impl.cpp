#include "../include/config.hpp"

#include "../core.h"

#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <set>

STREAMER_BEGIN_NS

namespace event {
	bool OnPlayerConnect(int playerid) {
		if (playerid >= 0 && playerid < MAX_PLAYERS) {
			boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
			if (p == core->getData()->players.end()) {
				Player player(playerid);
				core->getData()->players.insert(std::make_pair(playerid, player));
			}
		}
		return true;
	}

	bool OnPlayerDisconnect(int playerid, int reason) {
		core->getData()->players.erase(playerid);
		return true;
	}

	bool OnPlayerSpawn(int playerid) {
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end()) {
			p->second.requestingClass = false;
		}
		return true;
	}

	bool OnPlayerRequestClass(int playerid, int classid) {
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end()) {
			p->second.requestingClass = true;
		}
		return true;
	}

	bool OnPlayerEnterCheckpoint(int playerid) {
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end()) {
			if (p->second.activeCheckpoint != p->second.visibleCheckpoint) {
				int checkpointid = p->second.visibleCheckpoint;
				p->second.activeCheckpoint = checkpointid;
				for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a) {
					int amxIndex = 0;
					if (!amx_FindPublic(*a, "OnPlayerEnterDynamicCP", &amxIndex)) {
						amx_Push(*a, static_cast<cell>(checkpointid));
						amx_Push(*a, static_cast<cell>(playerid));
						amx_Exec(*a, NULL, amxIndex);
					}
				}
			}
		}
		return true;
	}

	bool OnPlayerLeaveCheckpoint(int playerid) {
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end()) {
			if (p->second.activeCheckpoint == p->second.visibleCheckpoint) {
				int checkpointid = p->second.activeCheckpoint;
				p->second.activeCheckpoint = 0;
				for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a) {
					int amxIndex = 0;
					if (!amx_FindPublic(*a, "OnPlayerLeaveDynamicCP", &amxIndex)) {
						amx_Push(*a, static_cast<cell>(checkpointid));
						amx_Push(*a, static_cast<cell>(playerid));
						amx_Exec(*a, NULL, amxIndex);
					}
				}
			}
		}
		return true;
	}

	bool OnPlayerEnterRaceCheckpoint(int playerid) {
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end()) {
			if (p->second.activeRaceCheckpoint != p->second.visibleRaceCheckpoint) {
				int checkpointid = p->second.visibleRaceCheckpoint;
				p->second.activeRaceCheckpoint = checkpointid;
				for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a) {
					int amxIndex = 0;
					if (!amx_FindPublic(*a, "OnPlayerEnterDynamicRaceCP", &amxIndex)) {
						amx_Push(*a, static_cast<cell>(checkpointid));
						amx_Push(*a, static_cast<cell>(playerid));
						amx_Exec(*a, NULL, amxIndex);
					}
				}
			}
		}
		return true;
	}

	bool OnPlayerLeaveRaceCheckpoint(int playerid) {
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end()) {
			if (p->second.activeRaceCheckpoint == p->second.visibleRaceCheckpoint) {
				int checkpointid = p->second.activeRaceCheckpoint;
				p->second.activeRaceCheckpoint = 0;
				for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a) {
					int amxIndex = 0;
					if (!amx_FindPublic(*a, "OnPlayerLeaveDynamicRaceCP", &amxIndex)) {
						amx_Push(*a, static_cast<cell>(checkpointid));
						amx_Push(*a, static_cast<cell>(playerid));
						amx_Exec(*a, NULL, amxIndex);
					}
				}
			}
		}
		return true;
	}

	bool OnPlayerPickUpPickup(int playerid, int pickupid) {
		for (boost::unordered_map<int, int>::iterator i = core->getData()->internalPickups.begin(); i != core->getData()->internalPickups.end(); ++i) {
			if (i->second == pickupid) {
				int pickupid = i->first;
				for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a) {
					int amxIndex = 0;
					if (!amx_FindPublic(*a, "OnPlayerPickUpDynamicPickup", &amxIndex)) {
						amx_Push(*a, static_cast<cell>(pickupid));
						amx_Push(*a, static_cast<cell>(playerid));
						amx_Exec(*a, NULL, amxIndex);
					}
				}
				break;
			}
		}
		return true;
	}

	bool OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ) {
		if (playerobject) {
			boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
			if (p != core->getData()->players.end()) {
				for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i) {
					if (i->second == objectid) {
						int objectid = i->first;
						for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a) {
							int amxIndex = 0;
							if (!amx_FindPublic(*a, "OnPlayerEditDynamicObject", &amxIndex)) {
								amx_Push(*a, amx_ftoc(fRotZ));
								amx_Push(*a, amx_ftoc(fRotY));
								amx_Push(*a, amx_ftoc(fRotX));
								amx_Push(*a, amx_ftoc(fZ));
								amx_Push(*a, amx_ftoc(fY));
								amx_Push(*a, amx_ftoc(fX));
								amx_Push(*a, static_cast<cell>(response));
								amx_Push(*a, static_cast<cell>(objectid));
								amx_Push(*a, static_cast<cell>(playerid));
								amx_Exec(*a, NULL, amxIndex);
							}
						}
						break;
					}
				}
			}
		}
		return false;
	}

	bool OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float x, float y, float z) {
		if (type == SELECT_OBJECT_PLAYER_OBJECT) {
			boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
			if (p != core->getData()->players.end()) {
				for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i) {
					if (i->second == objectid) {
						int objectid = i->first;
						for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a) {
							int amxIndex = 0;
							if (!amx_FindPublic(*a, "OnPlayerSelectDynamicObject", &amxIndex)) {
								amx_Push(*a, amx_ftoc(z));
								amx_Push(*a, amx_ftoc(y));
								amx_Push(*a, amx_ftoc(x));
								amx_Push(*a, static_cast<cell>(modelid));
								amx_Push(*a, static_cast<cell>(objectid));
								amx_Push(*a, static_cast<cell>(playerid));
								amx_Exec(*a, NULL, amxIndex);
							}
						}
						break;
					}
				}
			}
		}
		return false;
	}

	bool OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float x, float y, float z) {
		bool retVal = true;
		if (hittype == BULLET_HIT_TYPE_PLAYER_OBJECT) {
			boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
			if (p != core->getData()->players.end()) {
				for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i) {
					if (i->second == hitid) {
						int objectid = i->first;
						for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a) {
							int amxIndex = 0;
							cell amxRetVal = 0;
							if (!amx_FindPublic(*a, "OnPlayerShootDynamicObject", &amxIndex)) {
								amx_Push(*a, amx_ftoc(z));
								amx_Push(*a, amx_ftoc(y));
								amx_Push(*a, amx_ftoc(x));
								amx_Push(*a, static_cast<cell>(objectid));
								amx_Push(*a, static_cast<cell>(weaponid));
								amx_Push(*a, static_cast<cell>(playerid));
								amx_Exec(*a, &amxRetVal, amxIndex);
								if (!amxRetVal) {
									retVal = false;
								}
							}
						}
						break;
					}
				}
			}
		}
		return retVal;
	}

	bool OnPlayerGiveDamageActor(int playerid, int actorid, float amount, int weaponid, int bodypart) {
		for (boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i) {
			if (i->second == actorid) {
				int actorid = i->first;
				for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a) {
					int amxIndex = 0;
					if (!amx_FindPublic(*a, "OnPlayerGiveDamageDynamicActor", &amxIndex)) {
						amx_Push(*a, static_cast<cell>(bodypart));
						amx_Push(*a, static_cast<cell>(weaponid));
						amx_Push(*a, amx_ftoc(amount));
						amx_Push(*a, static_cast<cell>(actorid));
						amx_Push(*a, static_cast<cell>(playerid));
						amx_Exec(*a, NULL, amxIndex);
					}
				}
				break;
			}
		}
		return true;
	}

	bool OnActorStreamIn(int actorid, int forplayerid) {
		for (boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i) {
			if (i->second == actorid) {
				int actorid = i->first;
				for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a) {
					int amxIndex = 0;
					if (!amx_FindPublic(*a, "OnDynamicActorStreamIn", &amxIndex)) {
						amx_Push(*a, static_cast<cell>(forplayerid));
						amx_Push(*a, static_cast<cell>(actorid));
						amx_Exec(*a, NULL, amxIndex);
					}
				}
				break;
			}
		}
		return true;
	}

	bool OnActorStreamOut(int actorid, int forplayerid) {
		for (boost::unordered_map<int, int>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i) {
			if (i->second == actorid) {
				int actorid = i->first;
				for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a) {
					int amxIndex = 0;
					if (!amx_FindPublic(*a, "OnDynamicActorStreamOut", &amxIndex)) {
						amx_Push(*a, static_cast<cell>(forplayerid));
						amx_Push(*a, static_cast<cell>(actorid));
						amx_Exec(*a, NULL, amxIndex);
					}
				}
				break;
			}
		}
		return true;
	}
};

STREAMER_END_NS