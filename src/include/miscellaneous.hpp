#ifndef MISCELLANEOUS_HPP
#define MISCELLANEOUS_HPP

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

STREAMER_BEGIN_NS

int Streamer_CountItems(int type, bool serverWide);
int Streamer_CountVisibleItems(int playerid, int type, bool serverWide);
int Streamer_DestroyAllItems(int type, bool serverWide);
int Streamer_DestroyAllVisibleItems(int playerid, int type, bool serverWide);
int Streamer_GetDistanceToItem(float x, float y, float z, int type, int id, float &distance, int dimensions);
int Streamer_GetItemInternalID(int playerid, int type, int streamerid);
int Streamer_GetItemOffset(int type, int id, float &x, float &y, float &z);
int Streamer_GetItemStreamerID(int playerid, int type, int internalid);
int Streamer_IsItemVisible(int playerid, int type, int id);
int Streamer_IsToggleItem(int playerid, int type, int id);
int Streamer_SetItemOffset(int type, int id, float x, float y, float z);
int Streamer_ToggleAllItems(int playerid, int type, bool toggle, boost::unordered_set<int> exceptions);
int Streamer_ToggleItem(int playerid, int type, int id, bool toggle);
std::vector<int> Streamer_GetNearbyItems(float x, float y, float z, int type, float prange);

STREAMER_END_NS
#endif