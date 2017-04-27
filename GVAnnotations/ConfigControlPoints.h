#ifndef __CONFIG_CONTROL_POINTS_H__
#define __CONFIG_CONTROL_POINTS_H__

#include "GVCoord.h"
#include <vector>

class ConfigControlPoints
{
public:
	void configControlPoints(std::vector<GVCoord> &_controlPoints);
	void configControlPoints(GVCoord &vtx, std::vector<GVCoord> &_controlPoints);
	void configControlPoints(int idx, std::vector<GVCoord> &_controlPoints);
};

#endif //__CONFIG_CONTROL_POINTS_H__