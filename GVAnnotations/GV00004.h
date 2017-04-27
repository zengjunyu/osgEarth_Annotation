#ifndef __GV00004_H__
#define __GV00004_H__

#include "GVGeometry.h"
#include "KidNey.h"

class GV00004 : public GVGeometry
{
public:
	GV00004();
	~GV00004();

	int getControlPointCount();
	void getControlPoints(std::vector<GVCoord>& vtxBuffer);
	GVCoord getControlPoint(int idx);
	void setControlPointsEx(std::vector<GVCoord>& vtxBuffer);
	void setControlPoints(std::vector<GVCoord>& vtxBuffer);
	void setControlPoint(int idx, const GVCoord& vtx);
	bool toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2);

private:
	std::vector<GVCoord> _controlPoints;
	std::vector<double> _s;

	KidNey kidney;
};

#endif //__GV00004_H__