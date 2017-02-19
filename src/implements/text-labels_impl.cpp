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
#include "../include/config.hpp"
#include "../natives.h"

#include "../core.h"
#include "../utility.h"

#include "../include/text-labels.hpp"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include <Eigen/Core>

#include <string>

STREAMER_BEGIN_NS

int CreateDynamic3DTextLabel(std::string text, int color, float x, float y, float z, float drawDistance, int attachedPlayer, int attachedVehicle, bool testlos, int worldid, int interiorid, int playerid, float streamDistance, int areaid, int priority) {
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
	if (attachedPlayer != INVALID_GENERIC_ID || attachedVehicle != INVALID_GENERIC_ID) {
		textLabel->attach = boost::intrusive_ptr<Item::TextLabel::Attach>(new Item::TextLabel::Attach);
		textLabel->attach->player = attachedPlayer;
		textLabel->attach->vehicle = attachedVehicle;
		if (textLabel->position.cwiseAbs().maxCoeff() > 50.0f) {
			textLabel->position.setZero();
		}
		core->getStreamer()->attachedTextLabels.insert(textLabel);
	}
	textLabel->testLOS = testlos;
	Utility::addToContainer(textLabel->worlds, worldid);
	Utility::addToContainer(textLabel->interiors, interiorid);
	Utility::addToContainer(textLabel->players, playerid);
	textLabel->comparableStreamDistance = streamDistance < STREAMER_STATIC_DISTANCE_CUTOFF ? streamDistance : streamDistance * streamDistance;
	textLabel->streamDistance = streamDistance;
	Utility::addToContainer(textLabel->areas, areaid);
	textLabel->priority = priority;
	core->getGrid()->addTextLabel(textLabel);
	core->getData()->textLabels.insert(std::make_pair(textLabelID, textLabel));
	return textLabelID;
}

int DestroyDynamic3DTextLabel(int id) {
	boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
	if (t != core->getData()->textLabels.end()) {
		Utility::destroyTextLabel(t);
		return 1;
	}
	return 0;
}

int IsValidDynamic3DTextLabel(int id) {
	boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
	if (t != core->getData()->textLabels.end()) {
		return 1;
	}
	return 0;
}

std::string GetDynamic3DTextLabelText(int id) {
	boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
	if (t != core->getData()->textLabels.end()) {
		return t->second->text;
	}
	return "";
}

int UpdateDynamic3DTextLabelText(int label, int color, std::string text) {
	boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(label);
	if (t != core->getData()->textLabels.end()) {
		t->second->color = color;
		t->second->text = text;
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(t->first);
			if (i != p->second.internalTextLabels.end())
			{
				sampgdk::UpdatePlayer3DTextLabelText(p->first, i->second, t->second->color, t->second->text.c_str());
			}
		}
		return 1;
	}
	return 0;
}

STREAMER_END_NS