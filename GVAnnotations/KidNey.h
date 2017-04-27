#ifndef __KIDNEY_H__
#define __KIDNEY_H__

#include "GVCoord.h"
#include <vector>

class KidNey
{
public:
	void setKidNeyConreolPoints(std::vector<GVCoord> &controlpoints, std::vector<GVCoord> vtxBuffer);
	void drawKidNey(std::vector<GVCoord> controlpoints);
	void setKidNeyConreolPoint(std::vector<GVCoord> &controlpoints, int idx, const GVCoord vtx, std::vector<double> &s);
	GVCoord getKidNeyCenterPoint(const std::vector<GVCoord> &controlpoints);
	void getKidNeyScales(std::vector<double> &s);

	std::vector<GVCoord> tmpBuffer;
	std::vector<int> tmpBuffer2;
	std::vector<GVCoord> bilidian;
	std::vector<double> scaleBuffer;
};

#endif //__KIDNEY_H__