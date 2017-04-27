#pragma once

#include "GV00005.h"

static double getLen(GVCoord coord0, GVCoord coord1)
{
	return sqrt(pow(coord1.lon - coord0.lon, 2) + pow(coord1.lat - coord0.lat, 2));
}

static double cal_angle(const GVCoord &start_point, const GVCoord &end_point)
{
	double PI = osg::PI;
	double x01 = end_point.lon - start_point.lon;
	double y01 = end_point.lat - start_point.lat;
	double alpha;

	if(fabs(x01) <= 0.00001)
	{
		alpha = PI / 2.0;
		if(y01 < 0)
			alpha = -alpha + PI * 2.0;
	}
	else
		alpha = atan(y01 / x01);

	if(x01 < 0)
		alpha += PI;
	else if(y01 < 0)
		alpha += 2.0 * PI;

	return alpha;
}

GV00005::GV00005()
{

}

GV00005::~GV00005()
{

}

int GV00005::getControlPointCount()
{
	std::vector<GVCoord> controlPoints;
	getControlPoints(controlPoints);
	return controlPoints.size();
}

void GV00005::getControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	if(_controlPoints.size() <= 0)
		return ;
	vtxBuffer = _controlPoints;
}

GVCoord GV00005::getControlPoint(int idx)
{
	if(idx >= 0 && idx < _controlPoints.size())
		return _controlPoints[idx];

	return GVCoord();
}

void GV00005::setControlPointsEx(std::vector<GVCoord> &vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

void GV00005::setControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

void GV00005::setControlPoint(int idx, const GVCoord& vtx)
{
	int cpSize = _controlPoints.size();
	if(idx < 0 || idx >= cpSize)
		return ;
	_controlPoints[idx] = vtx;

}

bool GV00005::toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2)
{
	if(_controlPoints.size() < 2)
		return true;

	Bezier bezier;
	std::vector<GVCoord> bezierPoints;
	bezier.getBezier(_controlPoints, &bezierPoints);

	vtxBuffer = bezierPoints;
	vtxBuffer2->push_back(bezierPoints.size());

	double length = 0.0;

	for(int i=0; i<bezierPoints.size()-1; i++)
	{
		length += sqrt((bezierPoints[i].lon - bezierPoints[i+1].lon) * (bezierPoints[i].lon - bezierPoints[i+1].lon)
			   + (bezierPoints[i].lat - bezierPoints[i+1].lat) * (bezierPoints[i].lat - bezierPoints[i+1].lat));
	}

	double r = length / 50.0;
	int position = bezierPoints.size();
	for(int i=bezierPoints.size()-1; i>0; i--)
	{
		if (getLen(bezierPoints[i], bezierPoints[bezierPoints.size()-1]) > r)
		{
			position = i;
			break;
		}
	}

	double angle1 = cal_angle(bezierPoints[position], _controlPoints[_controlPoints.size()-1]);
	GVCoord arrowP0, arrowP1, arrowP2;
	arrowP0.lon = bezierPoints[position].lon + r * 0.2 * cos(angle1 + osg::PI/2);
	arrowP0.lat = bezierPoints[position].lat + r * 0.2 * sin(angle1 + osg::PI/2);
	arrowP1.lon = bezierPoints[position].lon + r * 0.2 * cos(angle1 - osg::PI/2);
	arrowP1.lat = bezierPoints[position].lat + r * 0.2 * sin(angle1 - osg::PI/2);
	arrowP2 = _controlPoints[_controlPoints.size()-1];

	vtxBuffer.push_back(arrowP0);
	vtxBuffer.push_back(arrowP1);
	vtxBuffer.push_back(arrowP2);
	vtxBuffer.push_back(arrowP0);
	vtxBuffer2->push_back(4);

	return true;
}