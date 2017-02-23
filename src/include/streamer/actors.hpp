#ifndef ACTORS_HPP
#define ACTORS_HPP

#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <Eigen/Core>

#include "config.hpp"

STREAMER_BEGIN_NS

int ApplyDynamicActorAnimation(int id, std::string lib, std::string name, float delta, bool loop, bool lockx, bool locky, bool freeze, int time);
int ClearDynamicActorAnimations(int id);
int CreateDynamicActor(int modelid, float x, float y, float z, float r, bool invulnerable, float health, int worldid, int interiorid, int playerid, float streamdistance, int areaid, int priority);
int DestroyDynamicActor(int id);
int GetDynamicActorFacingAngle(int id, float r);
int GetDynamicActorHealth(int id, float health);
int GetDynamicActorPos(int id, float x, float y, float z);
int GetDynamicActorVirtualWorld(int id);
int GetPlayerCameraTargetDynActor(int playerID);
int GetPlayerTargetDynamicActor(int id);
int IsDynamicActorInvulnerable(int id);
int IsDynamicActorStreamedIn(int playerID, int actorID);
int IsValidDynamicActor(int id);
int SetDynamicActorFacingAngle(int id, float angle);
int SetDynamicActorHealth(int id, float health);
int SetDynamicActorInvulnerable(int id, bool invulnerable);
int SetDynamicActorPos(int id, float x, float y, float z);
int SetDynamicActorVirtualWorld(int id, int value);

STREAMER_END_NS
#endif