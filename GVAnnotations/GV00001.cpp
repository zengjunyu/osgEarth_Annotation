#pragma once

#include "GV00001.h"

GV00001::GV00001()
{

}

GV00001::~GV00001()
{

}

int GV00001::getControlPointCount()
{
	std::vector<GVCoord> controlPoints;
	getControlPoints(controlPoints);
	return controlPoints.size();
}

void GV00001::getControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	if(_controlPoints.size() <= 0)
		return ;
	vtxBuffer = _controlPoints;
}

GVCoord GV00001::getControlPoint(int idx)
{
	if(idx >= 0 && idx < _controlPoints.size())
		return _controlPoints[idx];

	return GVCoord();
}

void GV00001::setControlPointsEx(std::vector<GVCoord> &vtxBuffer)
{
	setControlPoints(vtxBuffer);
}

void GV00001::setControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

void GV00001::setControlPoint(int idx, const GVCoord& vtx)
{
	int cpSize = _controlPoints.size();
	if(idx < 0 || idx >= cpSize)
		return ;
	_controlPoints[idx] = vtx;

}

bool GV00001::toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2)
{
	Bezier bezier;
	std::vector<GVCoord> bezierPoints;
	bezier.getBezier(_controlPoints, &bezierPoints);
	vtxBuffer = bezierPoints;
	vtxBuffer2->push_back(bezierPoints.size());

	return true;
}