#ifndef AREAS_HPP
#define AREAS_HPP

#pragma once

#include "config.hpp"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/variant.hpp>

#include <Eigen/Core>

#include "../common.h"

STREAMER_BEGIN_NS

int AttachDynamicAreaToObject(int id, int objectid, int type, int playerid, float x, float y, float z);
int AttachDynamicAreaToPlayer(int id, int playerrid, float x, float y, float z);
int AttachDynamicAreaToVehicle(int id, int vehicleid, float x, float y, float z);
int CreateDynamicCircle(float x, float y, float size, int worldid, int interiorid, int playerid, int priority);
int CreateDynamicCuboid(float minx, float miny, float minz, float maxx, float maxy, float maxz, int worldid, int interiorid, int playerid, int priority);
int CreateDynamicCylinder(float x, float y, float minz, float maxz, float size, int worldid, int interiorid, int playerid, int priority);
int CreateDynamicPolygon(std::vector<Eigen::Vector2f> points, float minz, float maxz, int maxpoints, int worldid, int interiorid, int playerid, int priority);
int CreateDynamicRectangle(float minx, float miny, float maxx, float maxy, int worldid, int interiorid, int playerid, int priority);
int CreateDynamicSphere(float x, float y, float z, float size, int worldid, int interiorid, int playerid, int priority);
int DestroyDynamicArea(int id);
std::vector<int> GetDynamicAreasForLine(float x, float y, float z, float x2, float y2, float z2);
std::vector<int> GetDynamicAreasForPoint(float x, float y, float z);
int GetDynamicPolygonNumberPoints(int id);
Polygon2D GetDynamicPolygonPoints(int id, std::vector<Eigen::Vector2f> points, int maxpoints);
int GetNumberDynamicAreasForLine(float x, float y, float z, float x2, float y2, float z2);
int GetNumberDynamicAreasForPoint(float x, float y, float z);
std::vector<int> GetPlayerDynamicAreas(int id);
int GetPlayerNumberDynamicAreas(int id);
int IsAnyPlayerInAnyDynamicArea(bool recheck);
int IsAnyPlayerInDynamicArea(int id, bool recheck);
int IsLineInAnyDynamicArea(float x, float y, float z, float x2, float y2, float z2);
int IsLineInDynamicArea(int id, float x, float y, float z, float x2, float y2, float z2);
int IsPlayerInAnyDynamicArea(int id, bool recheck);
int IsPlayerInDynamicArea(int playerID, int areaID, bool recheck);
int IsPointInAnyDynamicArea(float x, float y, float z);
int IsPointInDynamicArea(int id, float x, float y, float z);
int IsValidDynamicArea(int id);

STREAMER_END_NS
#endif