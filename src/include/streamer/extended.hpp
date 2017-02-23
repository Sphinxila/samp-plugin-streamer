#ifndef EXTENDED_HPP
#define EXTENDED_HPP

#include "../../common.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/variant.hpp>

#include <Eigen/Core>

#pragma once

#include "config.hpp"

STREAMER_BEGIN_NS

int CreateDynamicActorEx(
	int modelid,
	float x,
	float y,
	float z,
	float r,
	bool invulnerable,
	float health,
	float streamDistance,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	boost::unordered_set<int> &areas,
	int priority
);

int CreateDynamicPolygonEx(
	std::vector<Eigen::Vector2f> points,
	float minz,
	float maxz,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	int priority
);

int CreateDynamicCuboidEx(
	float minx,
	float miny,
	float minz,
	float maxx,
	float maxy,
	float maxz,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	int priority
);

int CreateDynamicRectangleEx(
	float minx,
	float miny,
	float maxx,
	float maxy,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	int priority
);

int CreateDynamicSphereEx(
	float x,
	float y,
	float z,
	float size,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	int priority
);

int CreateDynamicCylinderEx(
	float x,
	float y,
	float minz,
	float maxz,
	float size,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	int priority
);

int CreateDynamicCircleEx(
	float x,
	float y,
	float size,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	int priority
);

int CreateDynamic3DTextLabelEx(
	std::string text,
	int color,
	float x,
	float y,
	float z,
	float drawDistance,
	int attachedPlayer,
	int attachedVehicle,
	bool testlos,
	float streamDistance,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	boost::unordered_set<int> &areas,
	int priority
);

int CreateDynamicMapIconEx(
	float x,
	float y,
	float z,
	int type,
	int color,
	int style,
	float streamDistance,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	boost::unordered_set<int> &areas,
	int priority
);


int CreateDynamicRaceCPEx(
	int type,
	float x,
	float y,
	float z,
	float nx,
	float ny,
	float nz,
	float size,
	float streamDistance,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	boost::unordered_set<int> &areas,
	int priority
);

int CreateDynamicCPEx(
	float x,
	float y,
	float z,
	float size,
	float streamDistance,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	boost::unordered_set<int> &areas,
	int priority
);

int CreateDynamicPickupEx(
	int modelid,
	int type,
	float x,
	float y,
	float z,
	float streamDistance,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	boost::unordered_set<int> &areas,
	int priority
);

int CreateDynamicObjectEx(
	int modelid,
	float x,
	float y,
	float z,
	float rx,
	float ry,
	float rz,
	float streamDistance,
	float drawDistance,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	boost::unordered_set<int> &areas,
	int priority
);

STREAMER_END_NS
#endif