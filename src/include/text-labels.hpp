#ifndef TEXTLABELS_HPP
#define TEXTLABELS_HPP

#pragma once

#include "config.hpp"
#include <string>

STREAMER_BEGIN_NS

int CreateDynamic3DTextLabel(std::string text, int color, float x, float y, float z, float drawDistance, int attachedPlayer, int attachedVehicle, bool testlos, int worldid, int interiorid, int playerid, float streamDistance, int areaid, int priority);
int DestroyDynamic3DTextLabel(int id);
int IsValidDynamic3DTextLabel(int id);
std::string GetDynamic3DTextLabelText(int id);
int UpdateDynamic3DTextLabelText(int label, int color, std::string text);

STREAMER_END_NS
#endif