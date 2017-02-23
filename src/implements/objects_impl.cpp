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

#include <streamer/objects.hpp>

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

int CreateDynamicObject( int modelid, float x, float y,  float z,
	float rx, float ry, float rz, int worldid, int interiorid, int playerid,
	float streamDistance, float drawDistance, int areaid,  int priority) {
	if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_OBJECT) == core->getData()->objects.size()) {
		return 0;
	}
	int objectID = Item::Object::identifier.get();
	Item::SharedObject object(new Item::Object);
	//object->amx = amx;
	object->objectID = objectID;
	object->noCameraCollision = false;
	object->inverseAreaChecking = false;
	object->originalComparableStreamDistance = -1.0f;
	object->positionOffset = Eigen::Vector3f::Zero();
	object->modelID = modelid;
	object->position = Eigen::Vector3f(x, y, z);
	object->rotation = Eigen::Vector3f(rx, ry, rz);

	Utility::addToContainer(object->worlds, worldid);
	Utility::addToContainer(object->interiors, interiorid);
	Utility::addToContainer(object->players, playerid);

	object->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	object->streamDistance = streamDistance;
	object->drawDistance = drawDistance;

	Utility::addToContainer(object->areas, areaid);

	object->priority = priority;
	core->getGrid()->addObject(object);
	core->getData()->objects.insert(std::make_pair(objectID, object));
	return objectID;
}

int DestroyDynamicObject(int id) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		Utility::destroyObject(o);
		return 1;
	}
	return 0;
}

int IsValidDynamicObject(int id) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		return 1;
	}
	return 0;
}

int SetDynamicObjectPos(int id, float x, float y, float z) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		Eigen::Vector3f position = o->second->position;
		o->second->position = Eigen::Vector3f(x, y, z);
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p) {
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				sampgdk::SetPlayerObjectPos(p->first, i->second, o->second->position[0], o->second->position[1], o->second->position[2]);
			}
		}
		if (position[0] != o->second->position[0] || position[1] != o->second->position[1]) {
			if (o->second->cell)
			{
				core->getGrid()->removeObject(o->second, true);
			}
		}
		if (o->second->move) {
			o->second->move->duration = static_cast<int>((static_cast<float>(boost::geometry::distance(o->second->move->position.get<0>(), o->second->position) / o->second->move->speed) * 1000.0f));
			o->second->move->position.get<1>() = o->second->position;
			o->second->move->position.get<2>() = (o->second->move->position.get<0>() - o->second->position) / static_cast<float>(o->second->move->duration);
			if ((o->second->move->rotation.get<0>().maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
			{
				o->second->move->rotation.get<1>() = o->second->rotation;
				o->second->move->rotation.get<2>() = (o->second->move->rotation.get<0>() - o->second->rotation) / static_cast<float>(o->second->move->duration);
			}
			o->second->move->time = boost::chrono::steady_clock::now();
		}
		return 1;
	}
	return 0;
}

int GetDynamicObjectPos(int id, float &x, float &y, float &z) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		if (o->second->move) {
			core->getStreamer()->processActiveItems();
		}
		x = o->second->position[0];
		y = o->second->position[1];
		z = o->second->position[2];
		return 1;
	}
	return 0;
}

int SetDynamicObjectRot(int id, float rx, float ry, float rz) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		o->second->rotation = Eigen::Vector3f(rx, ry, rz);
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p) {
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				sampgdk::SetPlayerObjectRot(p->first, i->second, o->second->rotation[0], o->second->rotation[1], o->second->rotation[2]);
			}
		}
		if (o->second->move) {
			if ((o->second->move->rotation.get<0>().maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon()) {
				o->second->move->rotation.get<1>() = o->second->rotation;
				o->second->move->rotation.get<2>() = (o->second->move->rotation.get<0>() - o->second->rotation) / static_cast<float>(o->second->move->duration);
			}
		}
		return 1;
	}
	return 0;
}

int GetDynamicObjectRot(int id, float &rx, float &ry, float &rz) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		if (o->second->move) {
			core->getStreamer()->processActiveItems();
		}
		rx = o->second->rotation[0];
		ry = o->second->rotation[1];
		rz = o->second->rotation[2];
		return 1;
	}
	return 0;
}

int SetDynamicObjectNoCameraCol(int id) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		o->second->noCameraCollision = true;
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p) {
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end()) {
				sampgdk::SetPlayerObjectNoCameraCol(p->first, i->second);
			}
		}
		return 1;
	}
	return 0;
}

int GetDynamicObjectNoCameraCol(int id) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		return o->second->noCameraCollision != 0;
	}
	return 0;
}

int MoveDynamicObject(int id, float x, float y, float z, float speed, float rx, float ry, float rz) {
	if (!rx) {
		return 0;
	}

	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		if (o->second->attach) {
			Utility::logError("MoveDynamicObject: Object is currently attached and cannot be moved");
			return 0;
		}
		Eigen::Vector3f position(x, y, z);
		Eigen::Vector3f rotation(rx, ry, rz);
		o->second->move = boost::intrusive_ptr<Item::Object::Move>(new Item::Object::Move);
		o->second->move->duration = static_cast<int>((static_cast<float>(boost::geometry::distance(position, o->second->position) / speed * 1000.0f)));
		o->second->move->position.get<0>() = position;
		o->second->move->position.get<1>() = o->second->position;
		o->second->move->position.get<2>() = (position - o->second->position) / static_cast<float>(o->second->move->duration);
		o->second->move->rotation.get<0>() = rotation;
		if ((o->second->move->rotation.get<0>().maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
		{
			o->second->move->rotation.get<1>() = o->second->rotation;
			o->second->move->rotation.get<2>() = (rotation - o->second->rotation) / static_cast<float>(o->second->move->duration);
		}
		o->second->move->speed = speed;
		o->second->move->time = boost::chrono::steady_clock::now();
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				sampgdk::StopPlayerObject(p->first, i->second);
				sampgdk::MovePlayerObject(p->first, i->second, o->second->move->position.get<0>()[0], o->second->move->position.get<0>()[1], o->second->move->position.get<0>()[2], o->second->move->speed, o->second->move->rotation.get<0>()[0], o->second->move->rotation.get<0>()[1], o->second->move->rotation.get<0>()[2]);
			}
		}
		core->getStreamer()->movingObjects.insert(o->second);
		return static_cast<cell>(o->second->move->duration);
	}
	return 0;
}

int StopDynamicObject(int id) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		if (o->second->move) {
			for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p) {
				boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
				if (i != p->second.internalObjects.end()) {
					sampgdk::StopPlayerObject(p->first, i->second);
				}
			}
			o->second->move.reset();
			core->getStreamer()->movingObjects.erase(o->second);
			return 1;
		}
	}
	return 0;
}

int IsDynamicObjectMoving(int id) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		if (o->second->move) {
			return 1;
		}
	}
	return 0;
}

int AttachCameraToDynamicObject(int playerid, int objectid) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		int internalID = INVALID_GENERIC_ID;
		boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectid);
		if (i == p->second.internalObjects.end()) {
			boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
			if (o != core->getData()->objects.end())
			{
				p->second.position = Eigen::Vector3f(o->second->position[0], o->second->position[1], o->second->position[2]);
				core->getStreamer()->startManualUpdate(p->second, STREAMER_TYPE_OBJECT);
			}
			boost::unordered_map<int, int>::iterator j = p->second.internalObjects.find(objectid);
			if (j != p->second.internalObjects.end())
			{
				internalID = j->second;
			}
		} else {
			internalID = i->second;
		}
		if (internalID != INVALID_GENERIC_ID) {
			sampgdk::AttachCameraToPlayerObject(p->first, internalID);
			return 1;
		}
	}
	return 0;
}

int AttachDynamicObjectToObject(int objectid, int attachtoid, float x, float y, float z, float rx, float ry, float rz, bool sync) {
	if (sampgdk::FindNative("SetPlayerGravity") == NULL) {
		Utility::logError("AttachDynamicObjectToObject: YSF plugin must be loaded to attach objects to objects");
		return 0;
	}
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end()) {
		if (o->second->move) {
			Utility::logError("AttachDynamicObjectToObject: Object is currently moving and must be stopped first");
			return 0;
		}
		o->second->attach = boost::intrusive_ptr<Item::Object::Attach>(new Item::Object::Attach);
		o->second->attach->player = INVALID_GENERIC_ID;
		o->second->attach->vehicle = INVALID_GENERIC_ID;
		o->second->attach->object = attachtoid;
		o->second->attach->positionOffset = Eigen::Vector3f(x, y, z);
		o->second->attach->rotation = Eigen::Vector3f(rx, ry, rz);
		o->second->attach->syncRotation = sync;
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p) {
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end()) {
				boost::unordered_map<int, int>::iterator j = p->second.internalObjects.find(o->second->attach->object);
				if (j != p->second.internalObjects.end()) {
					AMX_NATIVE native = sampgdk::FindNative("AttachPlayerObjectToObject");
					if (native != NULL) {
						sampgdk::InvokeNative(native, "dddffffffb", p->first, i->second, j->second, o->second->attach->positionOffset[0], o->second->attach->positionOffset[1], o->second->attach->positionOffset[2], o->second->attach->rotation[0], o->second->attach->rotation[1], o->second->attach->rotation[2], o->second->attach->syncRotation);
					}
					for (boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.begin(); m != o->second->materials.end(); ++m) {
						if (m->second.main) {
							sampgdk::SetPlayerObjectMaterial(p->first, i->second, m->first, m->second.main->modelID, m->second.main->txdFileName.c_str(), m->second.main->textureName.c_str(), m->second.main->materialColor);
						} else if (m->second.text) {
							sampgdk::SetPlayerObjectMaterialText(p->first, i->second, m->second.text->materialText.c_str(), m->first, m->second.text->materialSize, m->second.text->fontFace.c_str(), m->second.text->fontSize, m->second.text->bold, m->second.text->fontColor, m->second.text->backColor, m->second.text->textAlignment);
						}
					}
				}
			}
		}
		if (attachtoid != INVALID_STREAMER_ID) {
			boost::unordered_map<int, Item::SharedObject>::iterator p = core->getData()->objects.find(attachtoid);
			if (p != core->getData()->objects.end())
			{
				if (o->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && p->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF)
				{
					o->second->originalComparableStreamDistance = o->second->comparableStreamDistance;
					o->second->comparableStreamDistance = p->second->comparableStreamDistance + static_cast<float>(boost::geometry::comparable_distance(o->second->position, p->second->position));
				}
			}
			core->getStreamer()->attachedObjects.insert(o->second);
		} else {
			if (o->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF && o->second->comparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF) {
				o->second->comparableStreamDistance = o->second->originalComparableStreamDistance;
			}
			o->second->attach.reset();
			core->getStreamer()->attachedObjects.erase(o->second);
			core->getGrid()->removeObject(o->second, true);
		}
		return 1;
	}
	return 0;
}

int AttachDynamicObjectToPlayer(int objectid, int playerid, float x, float y, float z, float rx, float ry, float rz) {
	if (sampgdk::FindNative("SetPlayerGravity") == NULL) {
		Utility::logError("AttachDynamicObjectToPlayer: YSF plugin must be loaded to attach objects to players");
		return 0;
	}
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end()) {
		if (o->second->move) {
			Utility::logError("AttachDynamicObjectToPlayer: Object is currently moving and must be stopped first");
			return 0;
		}
		o->second->attach = boost::intrusive_ptr<Item::Object::Attach>(new Item::Object::Attach);
		o->second->attach->object = INVALID_STREAMER_ID;
		o->second->attach->vehicle = INVALID_GENERIC_ID;
		o->second->attach->player = playerid;
		o->second->attach->positionOffset = Eigen::Vector3f(x,y,z);
		o->second->attach->rotation = Eigen::Vector3f(rx,ry,rz);
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p) {
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end()) {
				AMX_NATIVE native = sampgdk::FindNative("AttachPlayerObjectToPlayer");
				if (native != NULL) {
					sampgdk::InvokeNative(native, "dddffffffd", p->first, i->second, o->second->attach->player, o->second->attach->positionOffset[0], o->second->attach->positionOffset[1], o->second->attach->positionOffset[2], o->second->attach->rotation[0], o->second->attach->rotation[1], o->second->attach->rotation[2], 0);
				}
				for (boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.begin(); m != o->second->materials.end(); ++m) {
					if (m->second.main) {
						sampgdk::SetPlayerObjectMaterial(p->first, i->second, m->first, m->second.main->modelID, m->second.main->txdFileName.c_str(), m->second.main->textureName.c_str(), m->second.main->materialColor);
					} else if (m->second.text) {
						sampgdk::SetPlayerObjectMaterialText(p->first, i->second, m->second.text->materialText.c_str(), m->first, m->second.text->materialSize, m->second.text->fontFace.c_str(), m->second.text->fontSize, m->second.text->bold, m->second.text->fontColor, m->second.text->backColor, m->second.text->textAlignment);
					}
				}
			}
		}
		if (playerid != INVALID_GENERIC_ID) {
			core->getStreamer()->attachedObjects.insert(o->second);
		} else {
			o->second->attach.reset();
			core->getStreamer()->attachedObjects.erase(o->second);
			core->getGrid()->removeObject(o->second, true);
		}
		return 1;
	}
	return 0;
}

int AttachDynamicObjectToVehicle(int objectid, int vehicleid, float x, float y, float z, float rx, float ry, float rz) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end()) {
		if (o->second->move) {
			Utility::logError("AttachDynamicObjectToVehicle: Object is currently moving and must be stopped first");
			return 0;
		}
		o->second->attach = boost::intrusive_ptr<Item::Object::Attach>(new Item::Object::Attach);
		o->second->attach->object = INVALID_STREAMER_ID;
		o->second->attach->player = INVALID_GENERIC_ID;
		o->second->attach->vehicle = vehicleid;
		o->second->attach->positionOffset = Eigen::Vector3f(x, y, z);
		o->second->attach->rotation = Eigen::Vector3f(rx, ry, rz);
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p) {
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end()) {
				sampgdk::AttachPlayerObjectToVehicle(p->first, i->second, o->second->attach->vehicle, o->second->attach->positionOffset[0], o->second->attach->positionOffset[1], o->second->attach->positionOffset[2], o->second->attach->rotation[0], o->second->attach->rotation[1], o->second->attach->rotation[2]);
				for (boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.begin(); m != o->second->materials.end(); ++m) {
					if (m->second.main) {
						sampgdk::SetPlayerObjectMaterial(p->first, i->second, m->first, m->second.main->modelID, m->second.main->txdFileName.c_str(), m->second.main->textureName.c_str(), m->second.main->materialColor);
					} else if (m->second.text) {
						sampgdk::SetPlayerObjectMaterialText(p->first, i->second, m->second.text->materialText.c_str(), m->first, m->second.text->materialSize, m->second.text->fontFace.c_str(), m->second.text->fontSize, m->second.text->bold, m->second.text->fontColor, m->second.text->backColor, m->second.text->textAlignment);
					}
				}
			}
		}
		if (vehicleid != INVALID_GENERIC_ID) {
			core->getStreamer()->attachedObjects.insert(o->second);
		} else {
			o->second->attach.reset();
			core->getStreamer()->attachedObjects.erase(o->second);
			core->getGrid()->removeObject(o->second, true);
		}
		return 1;
	}
	return 0;
}

int EditDynamicObject(int playerid, int objectid) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end()) {
		int internalID = INVALID_GENERIC_ID;
		boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectid);
		if (i == p->second.internalObjects.end()) {
			boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
			if (o != core->getData()->objects.end()) {
				p->second.position = Eigen::Vector3f(o->second->position[0], o->second->position[1], o->second->position[2]);
				core->getStreamer()->startManualUpdate(p->second, STREAMER_TYPE_OBJECT);
			}
			boost::unordered_map<int, int>::iterator j = p->second.internalObjects.find(objectid);
			if (j != p->second.internalObjects.end()) {
				internalID = j->second;
			}
		} else {
			internalID = i->second;
		}
		if (internalID != INVALID_GENERIC_ID) {
			sampgdk::EditPlayerObject(p->first, internalID);
			return 1;
		}
	}
	return 0;
}

int IsDynamicObjectMaterialUsed(int objectid, int materialindex) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end()) {
		boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.find(materialindex);
		if (m != o->second->materials.end()) {
			if (m->second.main) {
				return 1;
			}
		}
	}
	return 0;
}

int GetDynamicObjectMaterial(int objectid, int materialindex, int &modelid, std::string &txdname, std::string &texturename, int &materialcolor) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end()) {
		boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.find(materialindex);
		if (m != o->second->materials.end()) {
			if (m->second.main) {
				modelid = m->second.main->modelID;
				txdname = m->second.main->txdFileName;
				texturename = m->second.main->textureName;
				materialcolor = m->second.main->materialColor;
				return 1;
			}
		}
	}
	return 0;
}

int SetDynamicObjectMaterial(int id, int materialindex, int modelid,  std::string txdname, std::string texturename, int materialcolor)  {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		int index = materialindex;
		o->second->materials[index].main = boost::intrusive_ptr<Item::Object::Material::Main>(new Item::Object::Material::Main);
		o->second->materials[index].main->modelID = modelid;
		o->second->materials[index].main->txdFileName = txdname;
		o->second->materials[index].main->textureName = texturename;
		o->second->materials[index].main->materialColor = materialcolor;
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				sampgdk::SetPlayerObjectMaterial(p->first, i->second, index, o->second->materials[index].main->modelID, o->second->materials[index].main->txdFileName.c_str(), o->second->materials[index].main->textureName.c_str(), o->second->materials[index].main->materialColor);
			}
		}
		o->second->materials[index].text.reset();
		return 1;
	}
	return 0;
}

int IsDynamicObjectMaterialTextUsed(int id, int materialindex) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.find(materialindex);
		if (m != o->second->materials.end()) {
			if (m->second.text) {
				return 1;
			}
		}
	}
	return 0;
}

int GetDynamicObjectMaterialText(int id, int materialindex, std::string &text, int &materialSize, std::string &fontface, int &fontsize, bool &bold, int &fontcolor, int &backcolor, int &textalignment) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.find(materialindex);
		if (m != o->second->materials.end()) {
			if (m->second.text) {
				text = m->second.text->materialText;
				materialSize = m->second.text->materialSize;
				fontface = m->second.text->fontFace;
				fontsize = m->second.text->fontSize;
				bold = m->second.text->bold;
				fontcolor = m->second.text->fontColor;
				backcolor = m->second.text->backColor;
				textalignment = m->second.text->textAlignment;
				return 1;
			}
		}
	}
	return 0;
}

int SetDynamicObjectMaterialText(int id, int materialindex, std::string text, int materialsize, int fontface, int fontsize, bool bold, int fontcolor, int backcolor, int textalignment) {
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
	if (o != core->getData()->objects.end()) {
		int index = materialindex;
		o->second->materials[index].text = boost::intrusive_ptr<Item::Object::Material::Text>(new Item::Object::Material::Text);
		o->second->materials[index].text->materialText = text;
		o->second->materials[index].text->materialSize = materialsize;
		o->second->materials[index].text->fontFace = fontface;
		o->second->materials[index].text->fontSize = fontsize;
		o->second->materials[index].text->bold = bold;
		o->second->materials[index].text->fontColor = fontcolor;
		o->second->materials[index].text->backColor = backcolor;
		o->second->materials[index].text->textAlignment = textalignment;
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p) {
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end()) {
				sampgdk::SetPlayerObjectMaterialText(p->first, i->second, o->second->materials[index].text->materialText.c_str(), index, o->second->materials[index].text->materialSize, o->second->materials[index].text->fontFace.c_str(), o->second->materials[index].text->fontSize, o->second->materials[index].text->bold, o->second->materials[index].text->fontColor, o->second->materials[index].text->backColor, o->second->materials[index].text->textAlignment);
			}
		}
		o->second->materials[index].main.reset();
		return 1;
	}
	return 0;
}

int GetPlayerCameraTargetDynObject(int playerrid) {
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerrid);
	if (p != core->getData()->players.end()) {
		int objectid = sampgdk::GetPlayerCameraTargetObject(p->second.playerID);
		if (objectid != INVALID_OBJECT_ID) {
			for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i) {
				if (i->second == objectid) {
					return i->first;
				}
			}
		}
	}
	return 0;
}

STREAMER_END_NS