#pragma once

#include "GV00007.h"

static double getLen(GVCoord coord0, GVCoord coord1)
{
	return sqrt(pow(coord1.lon - coord0.lon, 2) + pow(coord1.lat - coord0.lat, 2));
}

GV00007::GV00007()
{

}

GV00007::~GV00007()
{

}

int GV00007::getControlPointCount()
{
	std::vector<GVCoord> controlPoints;
	getControlPoints(controlPoints);
	return controlPoints.size();
}

void GV00007::getControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	if(_controlPoints.size() <= 0)
		return ;
	vtxBuffer = _controlPoints;
}

GVCoord GV00007::getControlPoint(int idx)
{
	if(idx >= 0 && idx < _controlPoints.size())
		return _controlPoints[idx];

	return GVCoord();
}

void GV00007::setControlPointsEx(std::vector<GVCoord> &vtxBuffer)
{
	cfcp.configControlPoints(vtxBuffer);
	_controlPoints = vtxBuffer;
}

void GV00007::setControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	cfcp.configControlPoints(vtxBuffer);
	
	if(vtxBuffer.size() > 1)
	{
		_controlPoints.clear();
		_controlPoints.push_back(vtxBuffer[0]);
		_controlPoints.push_back(vtxBuffer[1]);

		double scale1 = 0.1;
		double length = getLen(_controlPoints[0], _controlPoints[1]);
		double width = scale1 * length;

		double dx = (_controlPoints[0].lat - _controlPoints[1].lat) / length;
		double dy = (_controlPoints[1].lon - _controlPoints[0].lon) / length;

		GVCoord coord;
		coord.lon = _controlPoints[0].lon + dx * width;
		coord.lat = _controlPoints[0].lat + dy * width;
		_controlPoints.push_back(coord);
	}
}

void GV00007::setControlPoint(int idx, const GVCoord& vtx)
{
	int cpSize = _controlPoints.size();
	if(idx < 0 || idx >= cpSize)
		return ;
	
	GVCoord vtx2 = vtx;
	cfcp.configControlPoints(vtx2, _controlPoints);

	if (idx == 0 || idx == 1)
	{
		double width = getLen(_controlPoints[0], _controlPoints[2]);
		_controlPoints[idx] = vtx2;
		double length = getLen(_controlPoints[0], _controlPoints[1]);
		double dx = (_controlPoints[0].lat - _controlPoints[1].lat) / length;
		double dy = (_controlPoints[1].lon - _controlPoints[0].lon) / length;

		_controlPoints[2].lon = _controlPoints[0].lon + width * dx;
		_controlPoints[2].lat = _controlPoints[0].lat + width * dy;
	}

	if (idx == 2)
	{
		double l = getLen(vtx2, _controlPoints[0]);
		double dx1 = (vtx2.lon - _controlPoints[0].lon) / l;
		double dy1 = (vtx2.lat - _controlPoints[0].lat) / l;

		double length = getLen(_controlPoints[0], _controlPoints[1]);
		double dx2 = (_controlPoints[0].lat - _controlPoints[1].lat) / length;
		double dy2 = (_controlPoints[1].lon - _controlPoints[0].lon) / length;

		double _cos = dx1 * dx2 + dy1 * dy2;
		double len = l * _cos;
		if (2 * len > length)
		{
			len = length / 2.0;
		}

		if (vtx2.lat > _controlPoints[0].lat)
		{
			_controlPoints[2].lon = _controlPoints[0].lon + len * dx2;
			_controlPoints[2].lat = _controlPoints[0].lat + len * dy2;
		}
		else
		{
			_controlPoints[2].lon = _controlPoints[0].lon - len * dx2;
			_controlPoints[2].lat = _controlPoints[0].lat - len * dy2;
		}
	}
}

bool GV00007::toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2)
{
	if (_controlPoints.size() > 2)
	{
		double length = getLen(_controlPoints[0], _controlPoints[1]);
		double width = getLen(_controlPoints[0], _controlPoints[2]);

		double dx1 = (_controlPoints[1].lon - _controlPoints[0].lon) / length;
		double dy1 = (_controlPoints[1].lat - _controlPoints[0].lat) / length;
		double dx2 = (_controlPoints[2].lon - _controlPoints[0].lon) / width;
		double dy2 = (_controlPoints[2].lat - _controlPoints[0].lat) / width;

		double xx = _controlPoints[1].lon + (_controlPoints[2].lon - _controlPoints[0].lon);
		double yy = _controlPoints[1].lat + (_controlPoints[2].lat - _controlPoints[0].lat);
		GVCoord coord1 = GVCoord(xx, yy, 0.0);
		vtxBuffer.push_back(_controlPoints[2]);
		vtxBuffer.push_back(coord1);
		vtxBuffer2->push_back(2);

		xx = _controlPoints[0].lon * 2 - _controlPoints[2].lon;
		yy = _controlPoints[0].lat * 2 - _controlPoints[2].lat;
		GVCoord coord2 = GVCoord(xx, yy, 0.0);
		vtxBuffer.push_back(coord2);
		xx = coord1.lon + (coord2.lon - _controlPoints[2].lon);
		yy = coord1.lat + (coord2.lat - _controlPoints[2].lat);
		vtxBuffer.push_back(GVCoord(xx, yy, 0.0));
		vtxBuffer2->push_back(2);


		double scale1 = 0.2;
		double scale2 = 0.05;
		double scale3 = 2.0 / 3;

		GVCoord tempPoint0, tempPoint1;
		if (_controlPoints[0].lon <= _controlPoints[1].lon)
		{
			tempPoint0 = _controlPoints[0];
			tempPoint1 = _controlPoints[1];
		}
		else
		{
			tempPoint0 = _controlPoints[1];
			tempPoint1 = _controlPoints[0];
			dx1 = (tempPoint1.lon - tempPoint0.lon) / length;
			dy1 = (tempPoint1.lat - tempPoint0.lat) / length;
			dx2 = -(_controlPoints[2].lon - _controlPoints[0].lon) / width;
			dy2 = -(_controlPoints[2].lat - _controlPoints[0].lat) / width;
		}

		GVCoord point1, point2, point3;
		point2.lon = tempPoint0.lon + scale1 * length * dx1;
		point2.lat = tempPoint0.lat + scale1 * length * dy1;
		double l = scale3 * width / 2.0;
		point1.lon = point2.lon + l * dx1 + l * dx2;
		point1.lat = point2.lat + l * dy1 + l * dy2;
		point3.lon = point2.lon + l * dx1 - l * dx2;
		point3.lat = point2.lat + l * dy1 - l * dy2;
		vtxBuffer.push_back(point1);
		vtxBuffer.push_back(point2);
		vtxBuffer.push_back(point3);
		vtxBuffer2->push_back(3);

		point2.lon = tempPoint0.lon + (scale1 + scale2) * length * dx1;
		point2.lat = tempPoint0.lat + (scale1 + scale2) * length * dy1;
		point1.lon = point2.lon + l * dx1 + l * dx2;
		point1.lat = point2.lat + l * dy1 + l * dy2;
		point3.lon = point2.lon + l * dx1 - l * dx2;
		point3.lat = point2.lat + l * dy1 - l * dy2;
		vtxBuffer.push_back(point1);
		vtxBuffer.push_back(point2);
		vtxBuffer.push_back(point3);
		vtxBuffer2->push_back(3);

		point2.lon = tempPoint1.lon - ((scale1 + scale2) * length + l) * dx1;
		point2.lat = tempPoint1.lat - ((scale1 + scale2) * length + l) * dy1;
		point1.lon = point2.lon + l * dx1 + l * dx2;
		point1.lat = point2.lat + l * dy1 + l * dy2;
		point3.lon = point2.lon + l * dx1 - l * dx2;
		point3.lat = point2.lat + l * dy1 - l * dy2;
		vtxBuffer.push_back(point1);
		vtxBuffer.push_back(point2);
		vtxBuffer.push_back(point3);
		vtxBuffer2->push_back(3);

		point2.lon = tempPoint1.lon - (scale1 * length + l) * dx1;
		point2.lat = tempPoint1.lat - (scale1 * length + l) * dy1;
		point1.lon = point2.lon + l * dx1 + l * dx2;
		point1.lat = point2.lat + l * dy1 + l * dy2;
		point3.lon = point2.lon + l * dx1 - l * dx2;
		point3.lat = point2.lat + l * dy1 - l * dy2;
		vtxBuffer.push_back(point1);
		vtxBuffer.push_back(point2);
		vtxBuffer.push_back(point3);
		vtxBuffer2->push_back(3);
	}

	return true;
}