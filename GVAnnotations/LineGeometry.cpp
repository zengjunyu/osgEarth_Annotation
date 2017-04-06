#pragma once

#include "LineGeometry.h"

LineGeometry::LineGeometry()
{

}

LineGeometry::~LineGeometry()
{

}

void LineGeometry::setControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	_controlPoints = vtxBuffer;

	if(vtxBuffer.size() == 2)
	{
		_controlPoints.push_back(vtxBuffer[0]);
		_controlPoints.push_back(vtxBuffer[1]);
		double len = sqrt((_controlPoints[1].lon - _controlPoints[0].lon)*(_controlPoints[1].lon - _controlPoints[0].lon)
			+ (_controlPoints[1].lat - _controlPoints[0].lat)*(_controlPoints[1].lat - _controlPoints[0].lat));
		double sin_a0 = (_controlPoints[1].lat - _controlPoints[0].lat) / len;
		double cos_a0 = (_controlPoints[1].lon - _controlPoints[0].lon) / len;
		double a0 = asin(sin_a0);
		if(sin_a0>0 && cos_a0<0 || sin_a0<0 && cos_a0 <0) a0 = 3.1415926 -a0;
		double x = _controlPoints[1].lon + len / 4.0*cos(a0 + 3.1415926/6);
		double y = _controlPoints[1].lat + len / 4.0*sin(a0 + 3.1415926/6);
		_controlPoints.push_back(GVCoord(x,y,0));
		double x0 = _controlPoints[0].lon;
		double y0 = _controlPoints[0].lat;
		double x1 = _controlPoints[1].lon;
		double y1 = _controlPoints[1].lat;
		double x2 = _controlPoints[2].lon;
		double y2 = _controlPoints[2].lat;
		double len1 = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
		osg::Vec3d vec1 = osg::Vec3d(x0-x1,y0-y1,0);
		osg::Vec3d vec2 = osg::Vec3d(x2-x1,y2-y1,0);
		vec1.normalize();
		vec2.normalize();
		double cosa = vec1*vec2/(vec1.length()*vec2.length());
		double a = acos(cosa);
		scale = len1*sin(a)/len;
	}
}

bool LineGeometry::toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2 )
{
	if(_controlPoints.size() <= 1) return false;
	/*double x1 = _controlPoints[0].lon;
	double y1 = _controlPoints[0].lat;
	double x2 = _controlPoints[1].lon;
	double y2 = _controlPoints[1].lat;
	double x3 = _controlPoints[2].lon;
	double y3 = _controlPoints[2].lat;
	double x5,y5,z5 = 0.0;
	double x6,y6,z6 = 0.0;
	double x7,y7,z7 = 0.0;

	osg::Vec3d vec1 = osg::Vec3d(x2-x1,y2-y1,0);
	osg::Vec3d vec2 = osg::Vec3d(x3-x2,y3-y2,0);
	double len1 = vec1.length();
	double len2 = vec2.length2();
	double len3 = len1*scale;
	vec1.normalize();
	vec2.normalize();

	osg::Vec3d normal = osg::Vec3d(-vec1.y(),vec1.x(),0);

	x5 = _controlPoints[2].lon - normal.x()*len3*2;
	y5 = _controlPoints[2].lat - normal.y()*len3*2;

	x7 = _controlPoints[0].lon - vec2.x()*len2;
	y7 = _controlPoints[0].lat - vec2.y()*len2;

	y6 = x7 + normal.x()*len3*2;
	y6 = y7 + normal.y()*len3*2;

	vtxBuffer.push_back(GVCoord(x6,y6,z6));
	vtxBuffer.push_back(_controlPoints[0]);
	vtxBuffer.push_back(GVCoord(x7,y7,z7));
	vtxBuffer2->push_back(3);

	vtxBuffer.push_back(_controlPoints[0]);
	vtxBuffer.push_back(_controlPoints[1]);
	vtxBuffer2->push_back(2);

	vtxBuffer.push_back(_controlPoints[2]);
	vtxBuffer.push_back(_controlPoints[1]);
	vtxBuffer.push_back(GVCoord(x5,y5,z5));
	vtxBuffer2->push_back(3);*/


	vtxBuffer = _controlPoints;
	vtxBuffer2->push_back(_controlPoints.size());
	return true;
}