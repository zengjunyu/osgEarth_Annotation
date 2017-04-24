#pragma once

#include "GV00003.h"

#define Distance_3(coord1, coord2) (sqrt((coord1.lat - coord2.lat) * (coord1.lat - coord2.lat) + (coord1.lon - coord2.lon) * (coord1.lon - coord2.lon)))

GV00003::GV00003()
{
	scale[0] = 0.12;
	scale[1] = 0.15;
	scale[2] = 0.4;
	scale[3] = 0.3;
}

GV00003::~GV00003()
{

}

int GV00003::getControlPointCount()
{
	std::vector<GVCoord> controlPoints;
	getControlPoints(controlPoints);
	return controlPoints.size();
}

void GV00003::getControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	if(_controlPoints.size() <= 0)
		return ;

	vtxBuffer = _controlPoints;
}

GVCoord GV00003::getControlPoint(int idx)
{
	if(idx >= 0 && idx < _controlPoints.size())
		return _controlPoints[idx];

	return GVCoord();
}

void GV00003::setControlPointsEx(std::vector<GVCoord> &vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

void GV00003::setControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	_controlPoints.clear();
	_controlPoints = vtxBuffer;

	if(_controlPoints.size() > 2)
	{
		GVCoord arrowHead = _controlPoints[_controlPoints.size()-1];
		GVCoord arrowTailLeft = _controlPoints[0];
		GVCoord arrowTailRight = _controlPoints[1];

		double brockenLineLength = Distance_3(GVCoord((arrowTailLeft.lon + arrowTailRight.lon)/2, (arrowTailLeft.lat + arrowTailRight.lat)/2, 0), arrowHead);

		if(_controlPoints.size() > 3)
		{
			brockenLineLength = Distance_3(_controlPoints[_controlPoints.size()-2], _controlPoints[_controlPoints.size()-1]);
		}

		GVCoord penultimatePointOnAxis;
		penultimatePointOnAxis.lon = (arrowTailLeft.lon + arrowTailRight.lon)/2;
		penultimatePointOnAxis.lat = (arrowTailLeft.lat + arrowTailRight.lat)/2;
		if(_controlPoints.size() > 3)
		{
			penultimatePointOnAxis = _controlPoints[_controlPoints.size()-2];
		}

		double arrowTopLength = 2 * brockenLineLength * scale[0];
		double arrowNeckWidthHalf = arrowTopLength * scale[1];
		double arrowEarWidthHalf = arrowTopLength * scale[2];
		double arrowEarthLength = arrowTopLength * scale[3];

		GVCoord arrowNeckRight;
		GVCoord arrowEarLeft;
		GVCoord arrowEarMiddle;

		osg::Vec3d vecTop = osg::Vec3d(arrowHead.lon - penultimatePointOnAxis.lon, arrowHead.lat - penultimatePointOnAxis.lat, 0);
		vecTop.normalize();
		arrowEarMiddle.lon = arrowHead.lon - arrowTopLength * vecTop.x();
		arrowEarMiddle.lat = arrowHead.lat - arrowTopLength * vecTop.y();

		osg::Vec3d vecTopVertical = osg::Vec3d(-vecTop.y(), vecTop.x(), 0);
		arrowEarLeft.lon = arrowEarMiddle.lon + arrowEarWidthHalf * vecTopVertical.x();
		arrowEarLeft.lat = arrowEarMiddle.lat + arrowEarWidthHalf * vecTopVertical.y();

		GVCoord arrowNeckMiddle;
		arrowNeckMiddle.lon = arrowHead.lon - (arrowTopLength - arrowEarthLength) * vecTop.x();
		arrowNeckMiddle.lat = arrowHead.lat - (arrowTopLength - arrowEarthLength) * vecTop.y();

		arrowNeckRight.lon = arrowNeckMiddle.lon - arrowNeckWidthHalf * vecTopVertical.x();
		arrowNeckRight.lat = arrowNeckMiddle.lat - arrowNeckWidthHalf * vecTopVertical.y();

		_controlPoints.insert(_controlPoints.begin(), arrowEarMiddle);
		_controlPoints.insert(_controlPoints.begin(), arrowEarLeft);
		_controlPoints.insert(_controlPoints.begin(), arrowNeckRight);
	}
}

void GV00003::setControlPoint(int idx, const GVCoord& vtx)
{
	int cpSize = _controlPoints.size();
	if(idx < 0 || idx >= cpSize)
		return ;

	GVCoord arrowHead = _controlPoints[_controlPoints.size()-1];
	GVCoord arrowTailLeft = _controlPoints[3];
	GVCoord arrowTailRight = _controlPoints[4];

	double brockenLineLength = Distance_3(GVCoord((arrowTailLeft.lon + arrowTailRight.lon)/2, (arrowTailLeft.lat + arrowTailRight.lat)/2, 0), arrowHead);

	if(_controlPoints.size() > 6)
	{
		brockenLineLength = Distance_3(_controlPoints[_controlPoints.size()-2], _controlPoints[_controlPoints.size()-1]);
	}

	GVCoord penultimatePointOnAxis;
	penultimatePointOnAxis.lon = (arrowTailLeft.lon + arrowTailRight.lon)/2;
	penultimatePointOnAxis.lat = (arrowTailLeft.lat + arrowTailRight.lat)/2;
	if(_controlPoints.size() > 6)
	{
		penultimatePointOnAxis = _controlPoints[_controlPoints.size()-2];
	}

	double arrowTopLength = 2 * brockenLineLength * scale[0];
	double arrowNeckWidthHalf = arrowTopLength * scale[1];
	double arrowEarWidthHalf = arrowTopLength * scale[2];
	double arrowEarthLength = arrowTopLength * scale[3];

	osg::Vec3d vecTop = osg::Vec3d(arrowHead.lon - penultimatePointOnAxis.lon, arrowHead.lat - penultimatePointOnAxis.lat, 0);
	vecTop.normalize();
	osg::Vec3d vecTopVertical = osg::Vec3d(-vecTop.y(), vecTop.x(), 0);

	if(idx == 0)
	{
		GVCoord arrowNeckMiddle;
		arrowNeckMiddle.lon = arrowHead.lon - (arrowTopLength - arrowEarthLength) * vecTop.x();
		arrowNeckMiddle.lat = arrowHead.lat - (arrowTopLength - arrowEarthLength) * vecTop.y();

		osg::Vec3d vec = osg::Vec3d(vtx.lon - arrowNeckMiddle.lon, vtx.lat - arrowNeckMiddle.lat, 0);
		double projectLength = vec * vecTopVertical;
		if(projectLength >= 0){
			projectLength = 0.3 * arrowNeckWidthHalf;
		}else{
			if(abs(projectLength) > 0.7 * arrowEarWidthHalf){
				projectLength = 0.7 * arrowEarWidthHalf;
			}else{
				projectLength = abs(projectLength);
			}
		}

		_controlPoints[0].lon = arrowNeckMiddle.lon - projectLength * vecTopVertical.x();
		_controlPoints[0].lat = arrowNeckMiddle.lat - projectLength * vecTopVertical.y();

		scale[1] = projectLength / arrowTopLength;
	}else if(idx == 1){
		double distanceTop = Distance_3(_controlPoints[1], arrowHead);
		osg::Vec3d vec = osg::Vec3d(arrowHead.lon - vtx.lon, arrowHead.lat - vtx.lat, 0);
		vec.normalize();
		_controlPoints[1].lon = arrowHead.lon - distanceTop * vec.x();
		_controlPoints[1].lat = arrowHead.lat - distanceTop * vec.y();

		vec.x() = distanceTop * vec.x();
		vec.y() = distanceTop * vec.y();
		arrowEarWidthHalf = abs(vec * vecTopVertical);

		_controlPoints[2].lon = _controlPoints[1].lon - arrowEarWidthHalf * vecTopVertical.x();
		_controlPoints[2].lat = _controlPoints[1].lat - arrowEarWidthHalf * vecTopVertical.y();

		double arrowTopLengthNew = Distance_3(_controlPoints[2], arrowHead);

		scale[0] = (arrowTopLengthNew / 2.0) / brockenLineLength;
		scale[1] = arrowNeckWidthHalf / arrowTopLengthNew;
		scale[2] = arrowEarWidthHalf / arrowTopLengthNew;
		scale[3] = (arrowEarthLength - (arrowTopLength - arrowTopLengthNew)) / arrowTopLengthNew;
		arrowTopLength = arrowTopLengthNew;
	}else if(idx == 2){
		osg::Vec3d vec = osg::Vec3d(arrowHead.lon - vtx.lon, arrowHead.lat - vtx.lat, 0);
		double projectLength = vec * vecTop;
		static double maxLength = arrowTopLength;

		if(projectLength > 0 && projectLength < maxLength)
		{
			_controlPoints[2].lon = arrowHead.lon - projectLength * vecTop.x();
			_controlPoints[2].lat = arrowHead.lat - projectLength * vecTop.y();

			arrowTopLength = projectLength;
			arrowNeckWidthHalf = arrowTopLength * scale[1];
			arrowEarWidthHalf = arrowTopLength * scale[2];
			arrowEarthLength = arrowTopLength * scale[3];

			scale[0] = (arrowTopLength / 2.0) / brockenLineLength;

			_controlPoints[1].lon = _controlPoints[2].lon + arrowEarWidthHalf * vecTopVertical.x();
			_controlPoints[1].lat = _controlPoints[2].lat + arrowEarWidthHalf * vecTopVertical.y();

			GVCoord arrowNeckMiddle;
			arrowNeckMiddle.lon = _controlPoints[2].lon + arrowEarthLength * vecTop.x();
			arrowNeckMiddle.lat = _controlPoints[2].lat + arrowEarthLength * vecTop.y();

			_controlPoints[0].lon = arrowNeckMiddle.lon - arrowNeckWidthHalf * vecTopVertical.x();
			_controlPoints[0].lat = arrowNeckMiddle.lat - arrowNeckWidthHalf * vecTopVertical.y();
		}
	}else{
		_controlPoints[idx] = vtx;

		GVCoord arrowHead = _controlPoints[_controlPoints.size()-1];
		GVCoord arrowTailLeft = _controlPoints[3];
		GVCoord arrowTailRight = _controlPoints[4];

		double brockenLineLength = Distance_3(GVCoord((arrowTailLeft.lon + arrowTailRight.lon)/2, (arrowTailLeft.lat + arrowTailRight.lat)/2, 0), arrowHead);

		if(_controlPoints.size() > 6)
		{
			brockenLineLength = Distance_3(_controlPoints[_controlPoints.size()-2], _controlPoints[_controlPoints.size()-1]);
		}

		GVCoord penultimatePointOnAxis;
		penultimatePointOnAxis.lon = (arrowTailLeft.lon + arrowTailRight.lon)/2;
		penultimatePointOnAxis.lat = (arrowTailLeft.lat + arrowTailRight.lat)/2;
		if(_controlPoints.size() > 6)
		{
			penultimatePointOnAxis = _controlPoints[_controlPoints.size()-2];
		}

		double arrowTopLength = 2 * brockenLineLength * scale[0];
		double arrowNeckWidthHalf = arrowTopLength * scale[1];
		double arrowEarWidthHalf = arrowTopLength * scale[2];
		double arrowEarthLength = arrowTopLength * scale[3];

		GVCoord arrowNeckRight;
		GVCoord arrowEarLeft;
		GVCoord arrowEarMiddle;

		osg::Vec3d vecTop = osg::Vec3d(arrowHead.lon - penultimatePointOnAxis.lon, arrowHead.lat - penultimatePointOnAxis.lat, 0);
		vecTop.normalize();

		arrowEarMiddle.lon = arrowHead.lon - arrowTopLength * vecTop.x();
		arrowEarMiddle.lat = arrowHead.lat - arrowTopLength * vecTop.y();

		osg::Vec3d vecTopVertical = osg::Vec3d(-vecTop.y(), vecTop.x(), 0);
		arrowEarLeft.lon = arrowEarMiddle.lon + arrowEarWidthHalf * vecTopVertical.x();
		arrowEarLeft.lat = arrowEarMiddle.lat + arrowEarWidthHalf * vecTopVertical.y();

		GVCoord arrowNeckMiddle;
		arrowNeckMiddle.lon = arrowHead.lon - (arrowTopLength - arrowEarthLength) * vecTop.x();
		arrowNeckMiddle.lat = arrowHead.lat - (arrowTopLength - arrowEarthLength) * vecTop.y();

		arrowNeckRight.lon = arrowNeckMiddle.lon - arrowNeckWidthHalf * vecTopVertical.x();
		arrowNeckRight.lat = arrowNeckMiddle.lat - arrowNeckWidthHalf * vecTopVertical.y();

		_controlPoints[0] = arrowNeckRight;
		_controlPoints[1] = arrowEarLeft;
		_controlPoints[2] = arrowEarMiddle;
	}
}

bool GV00003::toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2)
{
	if(_controlPoints.size() < 6)
		return false;

	GVCoord arrowHead = _controlPoints[_controlPoints.size()-1];
	GVCoord arrowTailLeft = _controlPoints[3];
	GVCoord arrowTailRight = _controlPoints[4];

	double brockenLineLength = Distance_3(GVCoord((arrowTailLeft.lon + arrowTailRight.lon)/2, (arrowTailLeft.lat + arrowTailRight.lat)/2, 0), arrowHead);

	if(_controlPoints.size() > 6)
	{
		brockenLineLength = Distance_3(_controlPoints[_controlPoints.size()-2], _controlPoints[_controlPoints.size()-1]);
	}

	GVCoord penultimatePointOnAxis;
	penultimatePointOnAxis.lon = (arrowTailLeft.lon + arrowTailRight.lon)/2;
	penultimatePointOnAxis.lat = (arrowTailLeft.lat + arrowTailRight.lat)/2;
	if(_controlPoints.size() > 6)
	{
		penultimatePointOnAxis = _controlPoints[_controlPoints.size()-2];
	}

	double arrowTopLength = 2 * brockenLineLength * scale[0];
	double arrowNeckWidthHalf = arrowTopLength * scale[1];
	double arrowEarWidthHalf = arrowTopLength * scale[2];
	double arrowEarthLength = arrowTopLength * scale[3];

	GVCoord arrowNeckRight, arrowNeckLeft;
	GVCoord arrowEarRight, arrowEarLeft;

	osg::Vec3d vecTop = osg::Vec3d(arrowHead.lon - penultimatePointOnAxis.lon, arrowHead.lat - penultimatePointOnAxis.lat, 0);
	vecTop.normalize();
	osg::Vec3d vecTopVertical = osg::Vec3d(-vecTop.y(), vecTop.x(), 0);

	arrowEarLeft = _controlPoints[1];
	arrowEarRight.lon = arrowEarLeft.lon - 2 * arrowEarWidthHalf * vecTopVertical.x();
	arrowEarRight.lat = arrowEarLeft.lat - 2 * arrowEarWidthHalf * vecTopVertical.y();

	arrowNeckRight = _controlPoints[0];
	arrowNeckLeft.lon = arrowNeckRight.lon + 2 * arrowNeckWidthHalf * vecTopVertical.x();
	arrowNeckLeft.lat = arrowNeckRight.lat + 2 * arrowNeckWidthHalf * vecTopVertical.y();

	double arrowTailWidth = Distance_3(arrowTailLeft, arrowTailRight);

	GVCoord arrowTailMiddle;
	arrowTailMiddle.lon = (arrowTailLeft.lon + arrowTailRight.lon)/2;
	arrowTailMiddle.lat = (arrowTailLeft.lat + arrowTailRight.lat)/2;

	double offset = arrowTailWidth/2.0 - arrowNeckWidthHalf;

	Bezier bezier;

	if(_controlPoints.size() == 6)
	{
		osg::Vec3d vecTail = osg::Vec3d(arrowTailLeft.lon - arrowTailRight.lon, arrowTailLeft.lat - arrowTailRight.lat, 0);
		vecTail.normalize();

		GVCoord topTailMiddle;
		topTailMiddle.lon = (arrowHead.lon + arrowTailMiddle.lon) / 2;
		topTailMiddle.lat = (arrowHead.lat + arrowTailMiddle.lat) / 2;

		GVCoord arrowBodyLeft, arrowBodyRight;
		arrowBodyLeft.lon = topTailMiddle.lon + (arrowNeckWidthHalf + 0.25 * offset) * vecTail.x();
		arrowBodyLeft.lat = topTailMiddle.lat + (arrowNeckWidthHalf + 0.25 * offset) * vecTail.y();
		arrowBodyRight.lon = topTailMiddle.lon - (arrowNeckWidthHalf + 0.25 * offset) * vecTail.x();
		arrowBodyRight.lat = topTailMiddle.lat - (arrowNeckWidthHalf + 0.25 * offset) * vecTail.y();

		std::vector<GVCoord> keyPoints;
		std::vector<GVCoord> bezierPoints;
		double t = (_controlPoints[3].lon - _controlPoints[5].lon) * (_controlPoints[4].lat - _controlPoints[5].lat) - (_controlPoints[4].lon - _controlPoints[5].lon) * (_controlPoints[3].lat - _controlPoints[5].lat);
		keyPoints.clear();
		bezierPoints.clear();
		if(t < 0)
		{
			keyPoints.push_back(arrowTailRight);
			keyPoints.push_back(arrowBodyRight);
		}
		else
		{
			keyPoints.push_back(arrowTailLeft);
			keyPoints.push_back(arrowBodyLeft);
		}
		keyPoints.push_back(arrowNeckLeft);

		bezier.getBezier(keyPoints, &bezierPoints);
		for(int i=0; i<bezierPoints.size(); i++)
			vtxBuffer.push_back(bezierPoints[i]);

		double t1 = 0.0;
		for(; t1<=1; t1+=0.02)
		{
			double x = (1-t1) * bezierPoints[bezierPoints.size()-1].lon + t1 * arrowEarLeft.lon;
			double y = (1-t1) * bezierPoints[bezierPoints.size()-1].lat + t1 * arrowEarLeft.lat;
			vtxBuffer.push_back(GVCoord(x, y, 0.0));
		}

		vtxBuffer.push_back(arrowEarLeft);

		t1 = 0.0;
		for(; t1<=1; t1+=0.02)
		{
			double x = (1-t1) * arrowEarLeft.lon + t1 * arrowHead.lon;
			double y = (1-t1) * arrowEarLeft.lat + t1 * arrowHead.lat;
			vtxBuffer.push_back(GVCoord(x, y, 0.0));
		}

		vtxBuffer.push_back(arrowHead);

		t1 = 0.0;
		for(; t1<=1; t1+=0.02)
		{
			double x = (1-t1) * arrowHead.lon + t1 * arrowEarRight.lon;
			double y = (1-t1) * arrowHead.lat + t1 * arrowEarRight.lat;
			vtxBuffer.push_back(GVCoord(x, y, 0.0));
		}

		vtxBuffer.push_back(arrowEarRight);

		keyPoints.clear();
		bezierPoints.clear();
		keyPoints.push_back(arrowNeckRight);
		if(t < 0)
		{
			keyPoints.push_back(arrowBodyLeft);
			keyPoints.push_back(arrowTailLeft);
		}
		else
		{
			keyPoints.push_back(arrowBodyRight);
			keyPoints.push_back(arrowTailRight);
		}
		bezier.getBezier(keyPoints, &bezierPoints);

		t1 = 0.0;
		for(; t1<=1; t1+=0.02)
		{
			double x = (1-t1) * arrowEarRight.lon + t1 * bezierPoints[0].lon;
			double y = (1-t1) * arrowEarRight.lat + t1 * bezierPoints[0].lat;
			vtxBuffer.push_back(GVCoord(x, y, 0.0));
		}

		for(int i=0; i<bezierPoints.size(); i++)
			vtxBuffer.push_back(bezierPoints[i]);

		vtxBuffer2->push_back(vtxBuffer.size());
	}
	else
	{
		double axisBrokenLineLength = Distance_3(arrowTailMiddle, _controlPoints[5]);
		axisBrokenLineLength += brokenLineDistance(5, _controlPoints.size()-1);

		double currentTopBrokenLineLength = 0;
		std::vector<GVCoord> keyPointsLeft, keyPointsRight;
		std::vector<GVCoord> bezierPointsLeft, bezierPointsRight;

		keyPointsLeft.clear();
		keyPointsRight.clear();
		keyPointsLeft.push_back(arrowNeckLeft);
		keyPointsRight.push_back(arrowNeckRight);

		for (int i=_controlPoints.size()-2; i>4; i--)
		{
			currentTopBrokenLineLength = brokenLineDistance(i, _controlPoints.size()-1);
			double rate = pow(currentTopBrokenLineLength / axisBrokenLineLength, 2);
			osg::Vec3d vecNear = osg::Vec3d(_controlPoints[i+1].lon - _controlPoints[i].lon, _controlPoints[i+1].lat - _controlPoints[i].lat, 0);
			vecNear.normalize();
			osg::Vec3d vecNearVertical = osg::Vec3d(-vecNear.y(), vecNear.x(), 0);
			GVCoord bodyLeft, bodyRight;
			bodyLeft.lon = _controlPoints[i].lon + (arrowNeckWidthHalf + rate * offset) * vecNearVertical.x();
			bodyLeft.lat = _controlPoints[i].lat + (arrowNeckWidthHalf + rate * offset) * vecNearVertical.y();
			bodyRight.lon = _controlPoints[i].lon - (arrowNeckWidthHalf + rate * offset) * vecNearVertical.x();
			bodyRight.lat = _controlPoints[i].lat - (arrowNeckWidthHalf + rate * offset) * vecNearVertical.y();
			keyPointsLeft.push_back(bodyLeft);
			keyPointsRight.push_back(bodyRight);
		}
		keyPointsLeft.push_back(arrowTailLeft);
		keyPointsRight.push_back(arrowTailRight);

		double t = (_controlPoints[3].lon - _controlPoints[5].lon) * (_controlPoints[4].lat - _controlPoints[5].lat) - (_controlPoints[4].lon - _controlPoints[5].lon) * (_controlPoints[3].lat - _controlPoints[5].lat);
		if(t < 0)
		{
			keyPointsLeft[keyPointsLeft.size()-1] = arrowTailRight;
			keyPointsRight[keyPointsRight.size()-1] = arrowTailLeft;
		}

		bezier.getBezier(keyPointsLeft, &bezierPointsLeft);
		for (int i=bezierPointsLeft.size()-1; i>=0; i--)
			vtxBuffer.push_back(bezierPointsLeft[i]);

		double t1 = 0.0;
		for (; t1<=1; t1+=0.02)
		{
			double x = (1-t1) * bezierPointsLeft[0].lon + t1 * arrowEarLeft.lon;
			double y = (1-t1) * bezierPointsLeft[0].lat + t1 * arrowEarLeft.lat;
			vtxBuffer.push_back(GVCoord(x, y, 0.0));
		}

		vtxBuffer.push_back(arrowEarLeft);

		t1 = 0;
		for (; t1<=1; t1+=0.02)
		{
			double x = (1-t1) * arrowEarLeft.lon + t1 * arrowHead.lon;
			double y = (1-t1) * arrowEarLeft.lat + t1 * arrowHead.lat;
			vtxBuffer.push_back(GVCoord(x, y, 0.0));
		}

		vtxBuffer.push_back(arrowHead);

		t1 = 0;
		for (; t1<=1; t1+=0.02)
		{
			double x = (1-t1) * arrowHead.lon + t1 * arrowEarRight.lon;
			double y = (1-t1) * arrowHead.lat + t1 * arrowEarRight.lat;
			vtxBuffer.push_back(GVCoord(x, y, 0.0));
		}

		vtxBuffer.push_back(arrowEarRight);

		bezier.getBezier(keyPointsRight, &bezierPointsRight);

		t1 = 0;
		for (; t1<=1; t1+=0.02)
		{
			double x = (1-t1) * arrowEarRight.lon + t1 * bezierPointsRight[0].lon;
			double y = (1-t1) * arrowEarRight.lat + t1 * bezierPointsRight[0].lat;
			vtxBuffer.push_back(GVCoord(x, y, 0.0));
		}

		for(int i=0; i<bezierPointsRight.size(); i++)
			vtxBuffer.push_back(bezierPointsRight[i]);

		vtxBuffer2->push_back(vtxBuffer.size());
	}

	return true;
}

double GV00003::brokenLineDistance(int start, int end) const
{
	double axisBrokenlineLength = 0;
	for(int i=start; i<end; i++)
	{
		axisBrokenlineLength += Distance_3(_controlPoints[i], _controlPoints[i+1]);
	}

	return axisBrokenlineLength;
}