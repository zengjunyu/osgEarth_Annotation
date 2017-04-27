#pragma once

#include "GV00004.h"

GV00004::GV00004()
{

}

GV00004::~GV00004()
{

}

int GV00004::getControlPointCount()
{
	return _controlPoints.size();
}

void GV00004::getControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	if(_controlPoints.size() <= 0)
		return ;
	vtxBuffer = _controlPoints;
}

GVCoord GV00004::getControlPoint(int idx)
{
	if(idx >= 0 && idx < _controlPoints.size())
		return _controlPoints[idx];

	return GVCoord();
}

void GV00004::setControlPointsEx(std::vector<GVCoord> &vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

void GV00004::setControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	if (vtxBuffer.size() == 1)
	{
		return;
	}

	if(vtxBuffer.size() > 1)
	{
		kidney.setKidNeyConreolPoints(_controlPoints, vtxBuffer);
		kidney.getKidNeyScales(_s);
	}
}

void GV00004::setControlPoint(int idx, const GVCoord& vtx)
{
	int cpSize = _controlPoints.size();
	if(idx < 0 || idx >= cpSize)
		return ;
	
	kidney.setKidNeyConreolPoint(_controlPoints, idx, vtx, _s);

}

bool GV00004::toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2)
{
	if(_controlPoints.size() >= 12)
	{
		kidney.drawKidNey(_controlPoints);

		vtxBuffer = kidney.tmpBuffer;
		vtxBuffer2->push_back(kidney.tmpBuffer2[0]);
	}

	return true;
}