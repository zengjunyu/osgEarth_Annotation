#pragma once

#include "GV00002.h"

GV00002::GV00002()
{

}

GV00002::~GV00002()
{

}

int GV00002::getControlPointCount()
{
	std::vector<GVCoord> controlPoints;
	getControlPoints(controlPoints);
	return controlPoints.size();
}

void GV00002::getControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	if(_controlPoints.size() <= 0)
		return ;
	vtxBuffer = _controlPoints;
}

GVCoord GV00002::getControlPoint(int idx)
{
	if(idx >= 0 && idx < _controlPoints.size())
		return _controlPoints[idx];

	return GVCoord();
}

void GV00002::setControlPointsEx(std::vector<GVCoord> &vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

void GV00002::setControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

void GV00002::setControlPoint(int idx, const GVCoord& vtx)
{
	int cpSize = _controlPoints.size();
	if(idx < 0 || idx >= cpSize)
		return ;
	_controlPoints[idx] = vtx;

}

bool GV00002::toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2)
{
	vtxBuffer = _controlPoints;
	vtxBuffer2->push_back(vtxBuffer.size());

	return true;
}