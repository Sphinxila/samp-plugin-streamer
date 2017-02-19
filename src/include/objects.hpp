#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#pragma once

#include "config.hpp"

#include <boost/chrono.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>

#include <Eigen/Core>

#include <limits>
#include <string>

STREAMER_BEGIN_NS

int CreateDynamicObject(int modelid, float x, float y, float z,
	float rx, float ry, float rz, int worldid, int interiorid, int playerid,
	float streamDistance, float drawDistance, int areaid, int priority);
int DestroyDynamicObject(int id);
int IsValidDynamicObject(int id);
int SetDynamicObjectPos(int id, float x, float y, float z);
int GetDynamicObjectPos(int id, float &x, float &y, float &z);
int SetDynamicObjectRot(int id, float rx, float ry, float rz);
int GetDynamicObjectRot(int id, float &rx, float &ry, float &rz);
int SetDynamicObjectNoCameraCol(int id);
int GetDynamicObjectNoCameraCol(int id);
int MoveDynamicObject(int id, float x, float y, float z, float speed, float rx, float ry, float rz);
int StopDynamicObject(int id);
int IsDynamicObjectMoving(int id);
int AttachCameraToDynamicObject(int playerid, int objectid);
int AttachDynamicObjectToObject(int objectid, int attachtoid, float x, float y, float z, float rx, float ry, float rz, bool sync);
int AttachDynamicObjectToPlayer(int objectid, int playerid, float x, float y, float z, float rx, float ry, float rz);
int AttachDynamicObjectToVehicle(int objectid, int vehicleid, float x, float y, float z, float rx, float ry, float rz);
int EditDynamicObject(int playerid, int objectid);
int IsDynamicObjectMaterialUsed(int objectid, int materialindex);
int GetDynamicObjectMaterial(int objectid, int materialindex, int &modelid, std::string &txdname, std::string &texturename, int &materialcolor);
int SetDynamicObjectMaterial(int id, int materialindex, int modelid, std::string txdname, std::string texturename, int materialcolor);
int IsDynamicObjectMaterialTextUsed(int id, int materialindex);
int GetDynamicObjectMaterialText(int id, int materialindex, std::string &text, int &materialSize, std::string &fontface, int &fontsize, bool &bold, int &fontcolor, int &backcolor, int &textalignment);
int SetDynamicObjectMaterialText(int id, int materialindex, std::string text, int materialsize, int fontface, int fontsize, bool bold, int fontcolor, int backcolor, int textalignment);
int GetPlayerCameraTargetDynObject(int playerrid);

STREAMER_END_NS
#endif