#ifndef __GV00006_H__
#define __GV00006_H__

#include "GVGeometry.h"
#include "Bezier.h"
#include "ConfigControlPoints.h"

class GV00006 : public GVGeometry
{
public:
	GV00006();
	~GV00006();

	int getControlPointCount();
	void getControlPoints(std::vector<GVCoord>& vtxBuffer);
	GVCoord getControlPoint(int idx);
	void setControlPointsEx(std::vector<GVCoord>& vtxBuffer);
	void setControlPoints(std::vector<GVCoord>& vtxBuffer);
	void setControlPoint(int idx, const GVCoord& vtx);
	bool toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2);

private:
	std::vector<GVCoord> _controlPoints;
	double s0;

	Bezier bezier;
	ConfigControlPoints cfcp;
};

#endif //__GV00006_H__