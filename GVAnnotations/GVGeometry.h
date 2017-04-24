#ifndef __GVGEOMETRY_H__
#define __GVGEOMETRY_H__

#include "GVCoord.h"
#include <vector>
#include <osg/Vec3d>

class GVGeometry
{
public:
	virtual int getControlPointCount() = 0;
	virtual void getControlPoints(std::vector<GVCoord>& vtxBuffer) = 0;
	virtual GVCoord getControlPoint(int idx) = 0;
	virtual void setControlPointsEx(std::vector<GVCoord> &vtxBuffer) = 0;
	virtual void setControlPoints(std::vector<GVCoord>& vtxBuffer) = 0;
	virtual void setControlPoint(int idx, const GVCoord& vtx) = 0;
	virtual bool toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2) = 0;
};

#endif //__GVGEOMETRY_H__