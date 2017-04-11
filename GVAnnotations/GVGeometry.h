#ifndef __GVGEOMETRY_H__
#define __GVGEOMETRY_H__

#include "GVCoord.h"
#include <vector>

class GVGeometry
{
public:
	virtual void setControlPoints(std::vector<GVCoord>& vtxBuffer) = 0;
	virtual bool toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2) = 0;
};

#endif //__GVGEOMETRY_H__