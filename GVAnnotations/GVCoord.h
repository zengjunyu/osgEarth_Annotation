#ifndef __GVCOORD_H__
#define __GVCOORD_H__

struct GVCoord 
{
	double lon;
	double lat;
	double alt;

	GVCoord();
	GVCoord(double lon, double lat, double alt);
	bool equal(const GVCoord &c) const;
	bool vaild() const;
	GVCoord operator+(const GVCoord & coord) const;
};

#endif // __GVCOORD_H__