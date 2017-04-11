#ifndef __GV00001_H__
#define __GV00001_H__

#include "GVGeometry.h"

class GV00001 : public GVGeometry
{
public:
	GV00001();
	~GV00001();

	void setControlPoints(std::vector<GVCoord>& vtxBuffer);
	bool toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2);

private:
	std::vector<GVCoord> _controlPoints;
};

#endif //__GV00001_H__