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
#include "../main.h"
#include "../utility.h"

#include <streamer/areas.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/variant.hpp>

#include <Eigen/Core>
#include <vector>

STREAMER_BEGIN_NS

int CreateDynamicCircle(float x, float y, float size, int worldid, int interiorid, int playerid, int priority) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size()) {
		return 0;
	}

	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	//area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_CIRCLE;
	area->position = Eigen::Vector2f(x, y);
	area->comparableSize = size * size;
	area->size = size;

	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int CreateDynamicCylinder(float x, float y, float minz, float maxz, float size, int worldid, int interiorid, int playerid, int priority) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size()) {
		return 0;
	}

	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	//area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_CYLINDER;
	area->position = Eigen::Vector2f(x, y);
	area->height = Eigen::Vector2f(minz, maxz);
	area->comparableSize = size * size;
	area->size = size;

	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players,  playerid);
	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int CreateDynamicSphere(float x, float y, float z, float size, int worldid, int interiorid, int playerid, int priority) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size()) {
		return 0;
	}

	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	//area->amx = amx;
	area->areaID = areaID;
	area->spectateMode = true;
	area->type = STREAMER_AREA_TYPE_SPHERE;
	area->position = Eigen::Vector3f(x, y, z);
	area->comparableSize = size * size;
	area->size = size;

	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int CreateDynamicRectangle(float minx, float miny, float maxx, float maxy, int worldid, int interiorid, int playerid, int priority) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size()) {
		return 0;
	}

	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	//area->amx = amx;
	area->areaID = areaID;
	area->spectateMode = true;
	area->type = STREAMER_AREA_TYPE_RECTANGLE;
	area->position = Box2D(Eigen::Vector2f(minx, miny), Eigen::Vector2f(maxx, maxy));
	boost::geometry::correct(boost::get<Box2D>(area->position));
	area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(boost::get<Box2D>(area->position).min_corner(), boost::get<Box2D>(area->position).max_corner()));
	area->size = static_cast<float>(boost::geometry::distance(boost::get<Box2D>(area->position).min_corner(), boost::get<Box2D>(area->position).max_corner()));

	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int CreateDynamicCuboid(float minx, float miny, float minz, float maxx, float maxy, float maxz, int worldid, int interiorid, int playerid, int priority) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size()) {
		return 0;
	}

	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	//area->amx = amx;
	area->areaID = areaID;
	area->spectateMode = true;
	area->type = STREAMER_AREA_TYPE_CUBOID;
	area->position = Box3D(Eigen::Vector3f(minx, miny, minz), Eigen::Vector3f(maxx, maxy, maxz));
	boost::geometry::correct(boost::get<Box3D>(area->position));
	area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(boost::get<Box3D>(area->position).min_corner()[0], boost::get<Box3D>(area->position).min_corner()[1]), Eigen::Vector2f(boost::get<Box3D>(area->position).max_corner()[0], boost::get<Box3D>(area->position).max_corner()[1])));
	area->size = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(boost::get<Box3D>(area->position).min_corner()[0], boost::get<Box3D>(area->position).min_corner()[1]), Eigen::Vector2f(boost::get<Box3D>(area->position).max_corner()[0], boost::get<Box3D>(area->position).max_corner()[1])));

	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int CreateDynamicPolygon(std::vector<Eigen::Vector2f> points, float minz, float maxz, int maxpoints, int worldid, int interiorid, int playerid, int priority) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size()) {
		return 0;
	}

	if (static_cast<int>(maxpoints >= 2 && static_cast<int>(maxpoints) % 2)) {
		Utility::logError("CreateDynamicPolygon: Number of points must be divisible by two");
		return 0;
	}

	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	//area->amx = amx;
	area->areaID = areaID;
	area->spectateMode = true;
	area->type = STREAMER_AREA_TYPE_POLYGON;

	Polygon2D pol = boost::get<Polygon2D>(area->position);
	boost::geometry::assign_points(pol, points);
	boost::geometry::correct(pol);

	area->height = Eigen::Vector2f(minz, maxz);
	Box2D box = boost::geometry::return_envelope<Box2D>(boost::get<Polygon2D>(area->position));
	area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(box.min_corner(), box.max_corner()));
	area->size = static_cast<float>(boost::geometry::distance(box.min_corner(), box.max_corner()));
	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int DestroyDynamicArea(int id)
{
	Utility::executeFinalAreaCallbacks(id);
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
	if (a != core->getData()->areas.end()) {
		Utility::destroyArea(a);
		return 1;
	}
	return 0;
}

int IsValidDynamicArea(int id) {
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
	if (a != core->getData()->areas.end()) {
		return 1;
	}
	return 0;
}

// TODO: <sphinx> Check this out
Polygon2D GetDynamicPolygonPoints(int id, std::vector<Eigen::Vector2f> points, int maxpoints) {
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
	if (a != core->getData()->areas.end()) {
		return boost::get<Polygon2D>(a->second->position);
	}
	return Polygon2D();
}

int GetDynamicPolygonNumberPoints(int id) {
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
	if (a != core->getData()->areas.end()) {
		return boost::get<Polygon2D>(a->second->position).outer().size();
	}
	return 0;
}

int IsPlayerInDynamicArea(int playerID, int areaID, bool recheck) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerID);
	if (p != core->getData()->players.end()) {
		if (!recheck) {
			boost::unordered_set<int>::iterator i = p->second.internalAreas.find(areaID);
			if (i != p->second.internalAreas.end()) {
				return 1;
			}
		} else {
			boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaID);
			if (a != core->getData()->areas.end()) {
				return Utility::isPointInArea(p->second.position, a->second);
			}
		}
	}
	return 0;
}

int IsPlayerInAnyDynamicArea(int id, bool recheck) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(id);
	if (p != core->getData()->players.end()) {
		if (!recheck) {
			if (!p->second.internalAreas.empty()) {
				return 1;
			}
		} else {
			for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a) {
				if (Utility::isPointInArea(p->second.position, a->second)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int IsAnyPlayerInDynamicArea(int id, bool recheck) {
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p) {
		if (!recheck) {
			boost::unordered_set<int>::iterator i = p->second.internalAreas.find(id);
			if (i != p->second.internalAreas.end())
			{
				return 1;
			}
		} else {
			boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
			if (a != core->getData()->areas.end()) {
				return Utility::isPointInArea(p->second.position, a->second);
			}
		}
	}
	return 0;
}

int IsAnyPlayerInAnyDynamicArea(bool recheck) {
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p) {
		if (!recheck) {
			if (!p->second.internalAreas.empty())
			{
				return 1;
			}
		} else {
			for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a) {
				if (Utility::isPointInArea(p->second.position, a->second)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int IsPointInDynamicArea(int id, float x, float y, float z) {
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
	if (a != core->getData()->areas.end()) {
		return Utility::isPointInArea(Eigen::Vector3f(x, y, z), a->second);
	}
	return 0;
}

int IsPointInAnyDynamicArea(float x, float y, float z) {
	for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a) {
		if (Utility::isPointInArea(Eigen::Vector3f(x, y, z), a->second)) {
			return 1;
		}
	}
	return 0;
}

int IsLineInDynamicArea(int id, float x, float y, float z, float x2, float y2, float z2) {
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
	if (a != core->getData()->areas.end()) {
		return Utility::doesLineSegmentIntersectArea(Eigen::Vector3f(x, y, z), Eigen::Vector3f(x2, y2, z2), a->second);
	}
	return 0;
}

int IsLineInAnyDynamicArea(float x, float y, float z, float x2, float y2, float z2) {
	for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a) {
		if (Utility::doesLineSegmentIntersectArea(Eigen::Vector3f(x, y, z), Eigen::Vector3f(x2, y2, z2), a->second)) {
			return 1;
		}
	}
	return 0;
}

std::vector<int> GetPlayerDynamicAreas(int id) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(id);
	std::vector<int> finalAreas;
	if (p != core->getData()->players.end()) {
		std::multimap<float, int> orderedAreas;
		for (boost::unordered_set<int>::iterator i = p->second.internalAreas.begin(); i != p->second.internalAreas.end(); ++i) {
			boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(*i);
			if (a != core->getData()->areas.end()) {

				boost::variant<Polygon2D, Box2D, Box3D, Eigen::Vector2f, Eigen::Vector3f> position;
				if (a->second->attach) {
					position = a->second->position;
				} else {
					position = a->second->position;
				}

				float distance = 0.0f;
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_CIRCLE:
				case STREAMER_AREA_TYPE_CYLINDER:
				{
					distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(p->second.position[0], p->second.position[1]), boost::get<Eigen::Vector2f>(position)));
					break;
				}
				case STREAMER_AREA_TYPE_SPHERE:
				{
					distance = static_cast<float>(boost::geometry::comparable_distance(p->second.position, boost::get<Eigen::Vector3f>(position)));
					break;
				}
				case STREAMER_AREA_TYPE_RECTANGLE:
				{
					Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Box2D>(position));
					distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(p->second.position[0], p->second.position[1]), centroid));
					break;
				}
				case STREAMER_AREA_TYPE_CUBOID:
				{
					Eigen::Vector3f centroid = boost::geometry::return_centroid<Eigen::Vector3f>(boost::get<Box3D>(position));
					distance = static_cast<float>(boost::geometry::comparable_distance(p->second.position, centroid));
					break;
				
				}
				case STREAMER_AREA_TYPE_POLYGON:
				{
					Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Polygon2D>(position));
					distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(p->second.position[0], p->second.position[1]), centroid));
					break;
				}
				}
				orderedAreas.insert(std::pair<float, int>(distance, a->first));
			}
		}
		for (std::map<float, int>::iterator i = orderedAreas.begin(); i != orderedAreas.end(); ++i) {
			finalAreas.push_back(i->second);
		}
		return finalAreas;
	}
	return finalAreas;
}

int GetPlayerNumberDynamicAreas(int id) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(id);
	if (p != core->getData()->players.end()) {
		return static_cast<cell>(p->second.internalAreas.size());
	}
	return 0;
}

std::vector<int> GetDynamicAreasForPoint(float x, float y, float z) {
	std::multimap<float, int> orderedAreas;
	std::vector<SharedCell> pointCells;
	std::vector<int> finalAreas;
	core->getGrid()->findMinimalCellsForPoint(Eigen::Vector2f(x, y), pointCells);
	for (std::vector<SharedCell>::const_iterator p = pointCells.begin(); p != pointCells.end(); ++p) {
		for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = (*p)->areas.begin(); a != (*p)->areas.end(); ++a) {
			if (Utility::isPointInArea(Eigen::Vector3f(x, y, z), a->second)) {

				boost::variant<Polygon2D, Box2D, Box3D, Eigen::Vector2f, Eigen::Vector3f> position;
				if (a->second->attach) {
					position = a->second->position;
				} else {
					position = a->second->position;
				}

				float distance = 0.0f;
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_CIRCLE:
				case STREAMER_AREA_TYPE_CYLINDER:
				{
					distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(x, y), boost::get<Eigen::Vector2f>(position)));
					break;
				}
				case STREAMER_AREA_TYPE_SPHERE:
				{
					distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector3f(x, y, z), boost::get<Eigen::Vector3f>(position)));
					break;
				}
				case STREAMER_AREA_TYPE_RECTANGLE:
				{
					Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Box2D>(position));
					distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(x, y), centroid));
					break;
				}
				case STREAMER_AREA_TYPE_CUBOID:
				{
					Eigen::Vector3f centroid = boost::geometry::return_centroid<Eigen::Vector3f>(boost::get<Box3D>(position));
					distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector3f(x, y, z), centroid));
					break;

				}
				case STREAMER_AREA_TYPE_POLYGON:
				{
					Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Polygon2D>(position));
					distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(x, y), centroid));
					break;
				}
				}
				orderedAreas.insert(std::pair<float, int>(distance, a->first));
			}
		}
	}
	for (std::map<float, int>::iterator i = orderedAreas.begin(); i != orderedAreas.end(); ++i) {
		finalAreas.push_back(i->second);
	}
	return finalAreas;
}

int GetNumberDynamicAreasForPoint(float x, float y, float z) {
	int areaCount = 0;
	std::vector<SharedCell> pointCells;
	core->getGrid()->findMinimalCellsForPoint(Eigen::Vector2f(x, y), pointCells);
	for (std::vector<SharedCell>::const_iterator p = pointCells.begin(); p != pointCells.end(); ++p) {
		for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = (*p)->areas.begin(); a != (*p)->areas.end(); ++a) {
			if (Utility::isPointInArea(Eigen::Vector3f(x, y, z), a->second)) {
				++areaCount;
			}
		}
	}
	return areaCount;
}

std::vector<int> GetDynamicAreasForLine(float x, float y, float z, float x2, float y2, float z2) {
	std::multimap<float, int> orderedAreas;
	for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a) {
		if (Utility::doesLineSegmentIntersectArea(Eigen::Vector3f(x, y, z), Eigen::Vector3f(x2, y2, z2), a->second)) {

			boost::variant<Polygon2D, Box2D, Box3D, Eigen::Vector2f, Eigen::Vector3f> position;
			if (a->second->attach) {
				position = a->second->position;
			} else {
				position = a->second->position;
			}

			float distance = 0.0f;
			switch (a->second->type)
			{
			case STREAMER_AREA_TYPE_CIRCLE:
			case STREAMER_AREA_TYPE_CYLINDER:
			{
				distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(x, y), boost::get<Eigen::Vector2f>(position)));
				break;
			}
			case STREAMER_AREA_TYPE_SPHERE:
			{
				distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector3f(x, y, z), boost::get<Eigen::Vector3f>(position)));
				break;
			}
			case STREAMER_AREA_TYPE_RECTANGLE:
			{
				Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Box2D>(position));
				distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(x, y), centroid));
				break;
			}
			case STREAMER_AREA_TYPE_CUBOID:
			{
				Eigen::Vector3f centroid = boost::geometry::return_centroid<Eigen::Vector3f>(boost::get<Box3D>(position));
				distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector3f(x, y, z), centroid));
				break;

			}
			case STREAMER_AREA_TYPE_POLYGON:
			{
				Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Polygon2D>(position));
				distance = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(x, y), centroid));
				break;
			}
			}
			orderedAreas.insert(std::pair<float, int>(distance, a->first));
		}
	}
	std::vector<int> finalAreas;
	for (std::map<float, int>::iterator i = orderedAreas.begin(); i != orderedAreas.end(); ++i) {
		finalAreas.push_back(i->second);
	}
	return finalAreas;
}

int GetNumberDynamicAreasForLine(float x, float y, float z, float x2, float y2, float z2) {
	int areaCount = 0;
	for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a) {
		if (Utility::doesLineSegmentIntersectArea(Eigen::Vector3f(x, y, z), Eigen::Vector3f(x2, y2, z2), a->second)) {
			++areaCount;
		}
	}
	return static_cast<cell>(areaCount);
}

int AttachDynamicAreaToObject(int id, int objectid, int type, int playerid, float x, float y, float z) {
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
	if (a != core->getData()->areas.end()) {
		/*if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE) {
			Utility::logError("AttachDynamicAreaToObject: Only circles and spheres may be attached to objects");
			return 0;
		}*/

		if ((objectid != INVALID_GENERIC_ID && type != STREAMER_OBJECT_TYPE_DYNAMIC) || (objectid != INVALID_STREAMER_ID && type == STREAMER_OBJECT_TYPE_DYNAMIC)) {
			a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
			a->second->attach->player = INVALID_GENERIC_ID;
			a->second->attach->vehicle = INVALID_GENERIC_ID;
			a->second->attach->position = a->second->position;
			a->second->attach->object = boost::make_tuple(objectid, type, playerid);
			a->second->attach->positionOffset = Eigen::Vector3f(x, y, z);
			core->getStreamer()->attachedAreas.insert(a->second);
		} else {
			if (a->second->attach) {
				if ((a->second->attach->object.get<0>() != INVALID_GENERIC_ID && a->second->attach->object.get<1>() != STREAMER_OBJECT_TYPE_DYNAMIC) || (a->second->attach->object.get<0>() != INVALID_STREAMER_ID && a->second->attach->object.get<1>() == STREAMER_OBJECT_TYPE_DYNAMIC)) {
					a->second->attach.reset();
					core->getStreamer()->attachedAreas.erase(a->second);
					core->getGrid()->removeArea(a->second, true);
				}
			}
		}
		return 1;
	}
	return 0;
}

int AttachDynamicAreaToPlayer(int id, int playerrid, float x, float y, float z) {
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
	if (a != core->getData()->areas.end()) {
		/*if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE) {
			Utility::logError("AttachDynamicAreaToPlayer: Only circles and spheres may be attached to players");
			return 0;
		}*/

		if (playerrid != INVALID_GENERIC_ID) {
			a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
			a->second->attach->object = boost::make_tuple(INVALID_STREAMER_ID, STREAMER_OBJECT_TYPE_DYNAMIC, INVALID_PLAYER_ID);
			a->second->attach->vehicle = INVALID_GENERIC_ID;
			a->second->attach->position = a->second->position;
			a->second->attach->player = playerrid;
			a->second->attach->positionOffset = Eigen::Vector3f(x, y, z);
			core->getStreamer()->attachedAreas.insert(a->second);
		} else {
			if (a->second->attach) {
				if (a->second->attach->player != INVALID_GENERIC_ID) {
					a->second->attach.reset();
					core->getStreamer()->attachedAreas.erase(a->second);
					core->getGrid()->removeArea(a->second, true);
				}
			}
		}
		return 1;
	}
	return 0;
}

int AttachDynamicAreaToVehicle(int id, int vehicleid, float x, float y, float z) {
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
	if (a != core->getData()->areas.end()) {
		/*if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE) {
			Utility::logError("AttachDynamicAreaToVehicle: Only circles and spheres may be attached to vehicles");
			return 0;
		}*/

		if (vehicleid != INVALID_GENERIC_ID) {
			a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
			a->second->attach->object = boost::make_tuple(INVALID_STREAMER_ID, STREAMER_OBJECT_TYPE_DYNAMIC, INVALID_PLAYER_ID);
			a->second->attach->player = INVALID_GENERIC_ID;
			a->second->attach->position = a->second->position;
			a->second->attach->vehicle = vehicleid;
			a->second->attach->positionOffset = a->second->attach->positionOffset = Eigen::Vector3f(x, y, z);
			core->getStreamer()->attachedAreas.insert(a->second);
		} else {
			if (a->second->attach) {
				if (a->second->attach->vehicle != INVALID_GENERIC_ID) {
					a->second->attach.reset();
					core->getStreamer()->attachedAreas.erase(a->second);
					core->getGrid()->removeArea(a->second, true);
				}
			}
		}
		return 1;
	}
	return 0;
}

int ToggleDynAreaSpectateMode(int areaid, int spectator) { 
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end()) {
		a->second->spectateMode = spectator != 0;
		return 1;
	}
	return 0;
}

int IsToggleDynAreaSpectateMode(int areaid) {
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end()) {
		return static_cast<cell>(a->second->spectateMode != 0);
	}
	return 0;
}

STREAMER_END_NS