#ifndef __LINE_GEOMETRY_H__
#define __LINE_GEOMETRY_H__

#include <vector>
#include "GVCoord.h"
#include <cmath>
#include <iostream>

#include <osg/Vec3d>

class LineGeometry
{
public:
	LineGeometry();
	~LineGeometry();

	virtual void setControlPoints(std::vector<GVCoord>& vtxBuffer);
	virtual bool toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2);

private:
	std::vector<GVCoord> _controlPoints;
	double scale;
};

#endif  // __LINE_GEOMETRY_H__