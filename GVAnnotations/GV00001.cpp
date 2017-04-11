#pragma once

#include "GV00001.h"

GV00001::GV00001()
{

}

GV00001::~GV00001()
{

}

void GV00001::setControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

bool GV00001::toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2)
{
	vtxBuffer = _controlPoints;
	vtxBuffer2->push_back(_controlPoints.size());
	return true;
}