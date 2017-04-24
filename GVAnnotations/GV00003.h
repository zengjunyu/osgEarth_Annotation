#ifndef __GV00003_H__
#define __GV00003_H__

#include "GVGeometry.h"
#include "Bezier.h"

class GV00003 : public GVGeometry
{
public:
	GV00003();
	~GV00003();

	int getControlPointCount();
	void getControlPoints(std::vector<GVCoord>& vtxBuffer);
	GVCoord getControlPoint(int idx);
	void setControlPointsEx(std::vector<GVCoord>& vtxBuffer);
	void setControlPoints(std::vector<GVCoord>& vtxBuffer);
	void setControlPoint(int idx, const GVCoord& vtx);
	bool toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2);
	double brokenLineDistance(int start, int end) const;

private:
	std::vector<GVCoord> _controlPoints;
	double scale[4];
};

#endif //__GV00003_H__