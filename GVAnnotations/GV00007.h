#ifndef __GV00007_H__
#define __GV00007_H__

#include "GVGeometry.h"
#include "ConfigControlPoints.h"

class GV00007 : public GVGeometry
{
public:
	GV00007();
	~GV00007();

	int getControlPointCount();
	void getControlPoints(std::vector<GVCoord>& vtxBuffer);
	GVCoord getControlPoint(int idx);
	void setControlPointsEx(std::vector<GVCoord>& vtxBuffer);
	void setControlPoints(std::vector<GVCoord>& vtxBuffer);
	void setControlPoint(int idx, const GVCoord& vtx);
	bool toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2);

private:
	std::vector<GVCoord> _controlPoints;

	ConfigControlPoints cfcp;
};

#endif //__GV00007_H__