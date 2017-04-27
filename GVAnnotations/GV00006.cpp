#pragma once

#include "GV00006.h"

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

GV00006::GV00006()
{
	s0 = 0.5;
}

GV00006::~GV00006()
{

}

int GV00006::getControlPointCount()
{
	std::vector<GVCoord> controlPoints;
	getControlPoints(controlPoints);
	return controlPoints.size();
}

void GV00006::getControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	if(_controlPoints.size() <= 0)
		return ;
	vtxBuffer = _controlPoints;
}

GVCoord GV00006::getControlPoint(int idx)
{
	if(idx >= 0 && idx < _controlPoints.size())
		return _controlPoints[idx];

	return GVCoord();
}

void GV00006::setControlPointsEx(std::vector<GVCoord> &vtxBuffer)
{
	cfcp.configControlPoints(vtxBuffer);
	_controlPoints = vtxBuffer;
}

void GV00006::setControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	cfcp.configControlPoints(vtxBuffer);

	if (vtxBuffer.size() < 2)
	{
		return;
	}

	_controlPoints = vtxBuffer;

	std::vector<GVCoord> bezierPoints;
	bezier.getBezier(_controlPoints, &bezierPoints);

	double len1 = 0.0;
	for (int i=1; i<bezierPoints.size(); i++)
	{
		len1 += getLen(bezierPoints[i], bezierPoints[i-1]);
	}

	double tempLen = 0.0;
	int num = 0;
	for (int i=1; i<bezierPoints.size(); i++)
	{
		tempLen += getLen(bezierPoints[i], bezierPoints[i-1]);
		if (tempLen >= (len1 * s0))
		{
			num = i;
			break;
		}
	}
	_controlPoints.push_back(bezierPoints[num]);
}

void GV00006::setControlPoint(int idx, const GVCoord& vtx)
{
	int cpSize = _controlPoints.size();
	if(idx < 0 || idx >= cpSize)
		return ;

	GVCoord modify = vtx;
	cfcp.configControlPoints(modify, _controlPoints);

	if (idx >= 0 && idx <= _controlPoints.size()-2)
	{
		_controlPoints[idx] = modify;

		std::vector<GVCoord> myControlPoints;
		for (int i=0; i<_controlPoints.size()-1; i++)
		{
			myControlPoints.push_back(_controlPoints[i]);
		}

		std::vector<GVCoord> bezierPoints;
		bezier.getBezier(myControlPoints, &bezierPoints);

		double len1 = 0.0;
		for (int i=1; i<bezierPoints.size(); i++)
		{
			len1 += getLen(bezierPoints[i], bezierPoints[i-1]);
		}

		double tempLen = 0.0;
		int num = 0;
		for (int i=1; i<bezierPoints.size(); i++)
		{
			tempLen += getLen(bezierPoints[i], bezierPoints[i-1]);
			if (tempLen >= (len1 * s0))
			{
				num = i;
				break;
			}
		}
		_controlPoints[cpSize-1] = bezierPoints[num];
	}
	
	if (idx == _controlPoints.size()-1)
	{
		std::vector<GVCoord> myControlPoints;
		for (int i=0; i<_controlPoints.size()-1; i++)
		{
			myControlPoints.push_back(_controlPoints[i]);
		}

		std::vector<GVCoord> tempBezierPoints;
		bezier.getBezier(myControlPoints, &tempBezierPoints);

		double length = 0.0;
		for (int k=1; k<tempBezierPoints.size(); k++)
		{
			length += getLen(tempBezierPoints[k], tempBezierPoints[k-1]);
		}

		double min = getLen(tempBezierPoints[0], modify);
		int minPoint = 0;
		for (int i=0; i<tempBezierPoints.size(); i++)
		{
			if (getLen(tempBezierPoints[i], modify) < min)
			{
				min = getLen(tempBezierPoints[i], modify);
				minPoint = i;
			}
		}
		_controlPoints[idx] = tempBezierPoints[minPoint];

		double pointLength = 0.0;
		for (int i=1; i<=minPoint; i++)
		{
			pointLength += getLen(tempBezierPoints[i], tempBezierPoints[i-1]);
		}
		s0 = pointLength / length;
	}

}

bool GV00006::toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2)
{
	if(_controlPoints.size() < 3)
		return true;

	std::vector<GVCoord> bezierPoints;
	std::vector<GVCoord> myControlPoints;

	for (int i=0; i<_controlPoints.size()-1; i++)
	{
		myControlPoints.push_back(_controlPoints[i]);
	}

	bezier.getBezier(myControlPoints, &bezierPoints);

	double length = 0.0;

	for(int i=0; i<bezierPoints.size()-1; i++)
	{
		length += sqrt((bezierPoints[i].lon - bezierPoints[i+1].lon) * (bezierPoints[i].lon - bezierPoints[i+1].lon)
			+ (bezierPoints[i].lat - bezierPoints[i+1].lat) * (bezierPoints[i].lat - bezierPoints[i+1].lat));
	}

	double r = length / 50.0;

	int start = 0;
	int end = bezierPoints.size() - 1;
	for(int i=0; i<bezierPoints.size(); i++)
	{
		if (getLen(bezierPoints[i], _controlPoints[_controlPoints.size()-1]) < r)
		{
			start = i;
			break;
		}
	}

	for(int i=start; i<bezierPoints.size(); i++)
	{
		if (getLen(bezierPoints[i], _controlPoints[_controlPoints.size()-1]) > r)
		{
			end = i;
			break;
		}
	}

	for (int i=0; i<=start; i++)
	{
		vtxBuffer.push_back(bezierPoints[i]);
	}
	vtxBuffer2->push_back(start+1);

	double angle2 = cal_angle(bezierPoints[start], bezierPoints[end]);
	double distance = getLen(bezierPoints[start], bezierPoints[end]);
	GVCoord p0, p1, p2, p3, p4;
	p0.lon = bezierPoints[start].lon + distance * 0.35 * cos(angle2 + osg::PI/2);
	p0.lat = bezierPoints[start].lat + distance * 0.35 * sin(angle2 + osg::PI/2);
	p1.lon = bezierPoints[start].lon + distance * 0.15 * cos(angle2 - osg::PI/2);
	p1.lat = bezierPoints[start].lat + distance * 0.15 * sin(angle2 - osg::PI/2);
	p2.lon = p1.lon + distance * cos(angle2);
	p2.lat = p1.lat + distance * sin(angle2);
	p3.lon = p2.lon + distance * 0.25 * cos(angle2 + osg::PI/2);
	p3.lat = p2.lat + distance * 0.25 * sin(angle2 + osg::PI/2);
	p4.lon = p0.lon + distance * 0.7 * cos(angle2);
	p4.lat = p0.lat + distance * 0.7 * sin(angle2);

	vtxBuffer.push_back(p0);
	vtxBuffer.push_back(p1);
	vtxBuffer.push_back(p2);
	vtxBuffer.push_back(p3);
	vtxBuffer.push_back(p4);
	vtxBuffer.push_back(p0);
	vtxBuffer2->push_back(6);

	int position = bezierPoints.size();
	for(int i=bezierPoints.size()-1; i>0; i--)
	{
		if (getLen(bezierPoints[i], bezierPoints[bezierPoints.size()-1]) > r)
		{
			position = i;
			break;
		}
	}

	double angle1 = cal_angle(bezierPoints[position], _controlPoints[_controlPoints.size()-2]);
	GVCoord arrowP0, arrowP1, arrowP2;
	arrowP0.lon = bezierPoints[position].lon + r * 0.2 * cos(angle1 + osg::PI/2);
	arrowP0.lat = bezierPoints[position].lat + r * 0.2 * sin(angle1 + osg::PI/2);
	arrowP1.lon = bezierPoints[position].lon + r * 0.2 * cos(angle1 - osg::PI/2);
	arrowP1.lat = bezierPoints[position].lat + r * 0.2 * sin(angle1 - osg::PI/2);
	arrowP2 = _controlPoints[_controlPoints.size()-2];

	vtxBuffer.push_back(arrowP0);
	vtxBuffer.push_back(arrowP1);
	vtxBuffer.push_back(arrowP2);
	vtxBuffer.push_back(arrowP0);
	vtxBuffer2->push_back(4);

	for (int i=end; i<=position; i++)
	{
		vtxBuffer.push_back(bezierPoints[i]);
	}
	vtxBuffer2->push_back(position-end+1);

	return true;
}