/*
* Copyright (C) 2016 Incognito
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

#include <streamer/extended.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/variant.hpp>

#include <Eigen/Core>

STREAMER_BEGIN_NS

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
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_OBJECT) == core->getData()->objects.size()) {
		return 0;
	}

	int objectID = Item::Object::identifier.get();
	Item::SharedObject object(new Item::Object);
	//object->amx = amx;
	object->objectID = objectID;
	object->inverseAreaChecking = false;
	object->noCameraCollision = false;
	object->originalComparableStreamDistance = -1.0f;
	object->positionOffset = Eigen::Vector3f::Zero();
	object->modelID = modelid;
	object->position = Eigen::Vector3f(x, y, z);
	object->rotation = Eigen::Vector3f(rx, ry, rz);
	object->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	object->streamDistance = streamDistance;
	object->drawDistance = drawDistance;

	worlds = object->worlds;
	interiors = object->interiors;
	players = object->players;
	areas = object->areas;

	object->priority = priority;
	core->getGrid()->addObject(object);
	core->getData()->objects.insert(std::make_pair(objectID, object));
	return objectID;
}

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
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_PICKUP) == core->getData()->pickups.size()) {
		return 0;
	}
	int pickupID = Item::Pickup::identifier.get();
	Item::SharedPickup pickup(new Item::Pickup);
	//pickup->amx = amx;
	pickup->pickupID = pickupID;
	pickup->inverseAreaChecking = false;
	pickup->originalComparableStreamDistance = -1.0f;
	pickup->modelID = modelid;
	pickup->positionOffset = Eigen::Vector3f::Zero();
	pickup->type = type;
	pickup->position = Eigen::Vector3f(x, y, z);
	pickup->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	pickup->streamDistance = streamDistance;

	worlds = pickup->worlds;
	interiors = pickup->interiors;
	players = pickup->players;
	areas = pickup->areas;

	pickup->priority = priority;
	core->getGrid()->addPickup(pickup);
	core->getData()->pickups.insert(std::make_pair(pickupID, pickup));
	return pickupID;
}

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
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_CP) == core->getData()->checkpoints.size()) {
		return 0;
	}
	int checkpointID = Item::Checkpoint::identifier.get();
	Item::SharedCheckpoint checkpoint(new Item::Checkpoint);
	//checkpoint->amx = amx;
	checkpoint->checkpointID = checkpointID;
	checkpoint->inverseAreaChecking = false;
	checkpoint->originalComparableStreamDistance = -1.0f;
	checkpoint->positionOffset = Eigen::Vector3f::Zero();
	checkpoint->position = Eigen::Vector3f(x, y, z);
	checkpoint->size = size;
	checkpoint->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	checkpoint->streamDistance = streamDistance;

	worlds = checkpoint->worlds;
	interiors = checkpoint->interiors;
	players = checkpoint->players;
	areas = checkpoint->areas;

	checkpoint->priority = priority;
	core->getGrid()->addCheckpoint(checkpoint);
	core->getData()->checkpoints.insert(std::make_pair(checkpointID, checkpoint));
	return checkpointID;
}

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
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_RACE_CP) == core->getData()->raceCheckpoints.size()) {
		return 0;
	}
	int raceCheckpointID = Item::RaceCheckpoint::identifier.get();
	Item::SharedRaceCheckpoint raceCheckpoint(new Item::RaceCheckpoint);
	//raceCheckpoint->amx = amx;
	raceCheckpoint->raceCheckpointID = raceCheckpointID;
	raceCheckpoint->inverseAreaChecking = false;
	raceCheckpoint->originalComparableStreamDistance = -1.0f;
	raceCheckpoint->positionOffset = Eigen::Vector3f::Zero();
	raceCheckpoint->type = type;
	raceCheckpoint->position = Eigen::Vector3f(x, y, z);
	raceCheckpoint->next = Eigen::Vector3f(nx, ny, nz);
	raceCheckpoint->size = size;
	raceCheckpoint->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	raceCheckpoint->streamDistance = streamDistance;

	worlds = raceCheckpoint->worlds;
	interiors = raceCheckpoint->interiors;
	players = raceCheckpoint->players;
	areas = raceCheckpoint->areas;

	raceCheckpoint->priority = priority;
	core->getGrid()->addRaceCheckpoint(raceCheckpoint);
	core->getData()->raceCheckpoints.insert(std::make_pair(raceCheckpointID, raceCheckpoint));
	return raceCheckpointID;
}

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
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_MAP_ICON) == core->getData()->mapIcons.size()) {
		return 0;
	}
	int mapIconID = Item::MapIcon::identifier.get();
	Item::SharedMapIcon mapIcon(new Item::MapIcon);
	//mapIcon->amx = amx;
	mapIcon->mapIconID = mapIconID;
	mapIcon->inverseAreaChecking = false;
	mapIcon->originalComparableStreamDistance = -1.0f;
	mapIcon->positionOffset = Eigen::Vector3f::Zero();
	mapIcon->position = Eigen::Vector3f(x, y, z);
	mapIcon->type = type;
	mapIcon->color = color;
	mapIcon->style = style;
	mapIcon->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	mapIcon->streamDistance = streamDistance;

	worlds = mapIcon->worlds;
	interiors = mapIcon->interiors;
	players = mapIcon->players;
	areas = mapIcon->areas;

	mapIcon->priority = priority;
	core->getGrid()->addMapIcon(mapIcon);
	core->getData()->mapIcons.insert(std::make_pair(mapIconID, mapIcon));
	return mapIconID;
}

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
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_3D_TEXT_LABEL) == core->getData()->textLabels.size()) {
		return 0;
	}
	int textLabelID = Item::TextLabel::identifier.get();
	Item::SharedTextLabel textLabel(new Item::TextLabel);
	//textLabel->amx = amx;
	textLabel->textLabelID = textLabelID;
	textLabel->inverseAreaChecking = false;
	textLabel->originalComparableStreamDistance = -1.0f;
	textLabel->positionOffset = Eigen::Vector3f::Zero();
	textLabel->text = text;
	textLabel->color = color;
	textLabel->position = Eigen::Vector3f(x, y, z);
	textLabel->drawDistance = drawDistance;
	if (attachedPlayer != INVALID_GENERIC_ID || attachedVehicle != INVALID_GENERIC_ID)
	{
		textLabel->attach = boost::intrusive_ptr<Item::TextLabel::Attach>(new Item::TextLabel::Attach);
		textLabel->attach->player = attachedPlayer;
		textLabel->attach->vehicle = attachedVehicle;
		if (textLabel->position.cwiseAbs().maxCoeff() > 50.0f)
		{
			textLabel->position.setZero();
		}
		core->getStreamer()->attachedTextLabels.insert(textLabel);
	}
	textLabel->testLOS = testlos;
	textLabel->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	textLabel->streamDistance = streamDistance;

	worlds = textLabel->worlds;
	interiors = textLabel->interiors;
	players = textLabel->players;
	areas = textLabel->areas;

	textLabel->priority = priority;
	core->getGrid()->addTextLabel(textLabel);
	core->getData()->textLabels.insert(std::make_pair(textLabelID, textLabel));
	return textLabelID;
}

int CreateDynamicCircleEx(
	float x,
	float y,
	float size,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	int priority
) {
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

	worlds = area->worlds;
	interiors = area->interiors;
	players = area->players;

	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

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
) {
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

	worlds = area->worlds;
	interiors = area->interiors;
	players = area->players;

	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int CreateDynamicSphereEx(
	float x,
	float y,
	float z,
	float size,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	int priority
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size()) {
		return 0;
	}
	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	//area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_SPHERE;
	area->position = Eigen::Vector3f(x, y, z);
	area->comparableSize = size * size;
	area->size = size;

	worlds = area->worlds;
	interiors = area->interiors;
	players = area->players;

	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int CreateDynamicRectangleEx(
	float minx,
	float miny,
	float maxx,
	float maxy,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	int priority
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size()) {
		return 0;
	}
	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	//area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_RECTANGLE;
	area->position = Box2D(Eigen::Vector2f(minx, miny), Eigen::Vector2f(maxx, maxy));
	boost::geometry::correct(boost::get<Box2D>(area->position));
	area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(boost::get<Box2D>(area->position).min_corner(), boost::get<Box2D>(area->position).max_corner()));
	area->size = static_cast<float>(boost::geometry::distance(boost::get<Box2D>(area->position).min_corner(), boost::get<Box2D>(area->position).max_corner()));

	worlds = area->worlds;
	interiors = area->interiors;
	players = area->players;

	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

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
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size()) {
		return 0;
	}
	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	//area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_CUBOID;
	area->position = Box3D(Eigen::Vector3f(minx, miny, minz), Eigen::Vector3f(maxx, maxy, maxz));
	boost::geometry::correct(boost::get<Box3D>(area->position));
	area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(boost::get<Box3D>(area->position).min_corner()[0], boost::get<Box3D>(area->position).min_corner()[1]), Eigen::Vector2f(boost::get<Box3D>(area->position).max_corner()[0], boost::get<Box3D>(area->position).max_corner()[1])));
	area->size = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(boost::get<Box3D>(area->position).min_corner()[0], boost::get<Box3D>(area->position).min_corner()[1]), Eigen::Vector2f(boost::get<Box3D>(area->position).max_corner()[0], boost::get<Box3D>(area->position).max_corner()[1])));
	worlds = area->worlds;
	interiors = area->interiors;
	players = area->players;
	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int CreateDynamicPolygonEx(
	std::vector<Eigen::Vector2f> points,
	float minz,
	float maxz,
	boost::unordered_set<int> &worlds,
	boost::unordered_set<int> &interiors,
	std::bitset<MAX_PLAYERS> &players,
	int priority
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size()) {
		return 0;
	}

	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	//area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_POLYGON;

	auto polygon = boost::get<Polygon2D>(area->position);
	boost::geometry::assign_points(polygon, points);
	boost::geometry::correct(polygon);

	area->height = Eigen::Vector2f(minz, maxz);
	Box2D box = boost::geometry::return_envelope<Box2D>(boost::get<Polygon2D>(area->position));
	area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(box.min_corner(), box.max_corner()));
	area->size = static_cast<float>(boost::geometry::distance(box.min_corner(), box.max_corner()));

	worlds = area->worlds;
	interiors = area->interiors;
	players = area->players;

	area->priority = priority;
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

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
) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_ACTOR) == core->getData()->actors.size()) {
		return 0;
	}
	int actorID = Item::Actor::identifier.get();
	Item::SharedActor actor(new Item::Actor);
	//actor->amx = amx;
	actor->actorID = actorID;
	actor->inverseAreaChecking = false;
	actor->originalComparableStreamDistance = -1.0f;
	actor->modelID = modelid;
	actor->position = Eigen::Vector3f(x, y, z);
	actor->rotation = r;
	actor->invulnerable = invulnerable;
	actor->health = health;
	actor->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	actor->streamDistance = streamDistance;

	worlds = actor->worlds;
	interiors = actor->interiors;
	players = actor->players;
	areas = actor->areas;

	actor->priority = priority;
	core->getGrid()->addActor(actor);
	core->getData()->actors.insert(std::make_pair(actorID, actor));
	return actorID;
}

STREAMER_END_NS
