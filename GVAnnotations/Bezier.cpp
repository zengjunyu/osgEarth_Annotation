#include "Bezier.h"

void Bezier::getBezier(const std::vector<GVCoord>& controlPoints, std::vector<GVCoord>* bezierPoints)
{
	std::vector<GVCoord> tempControlPoints;
	tempControlPoints.push_back(controlPoints[0]);
	tempControlPoints.push_back(controlPoints[0]);
	for(int i=1; i < controlPoints.size()-1; i++)
	{
		GVCoord p0;
		GVCoord p1;
		GVCoord p2;
		GVCoord curPoint = GVCoord(controlPoints[i].lon, controlPoints[i].lat, 0);
		p0.lon = (controlPoints[i-1].lon + curPoint.lon) / 2;
		p0.lat = (controlPoints[i-1].lat + curPoint.lat) / 2;
		p1.lon = (controlPoints[i+1].lon + curPoint.lon) / 2;
		p1.lat = (controlPoints[i+1].lat + curPoint.lat) / 2;
		p2.lon = (p0.lon + p1.lon) / 2;
		p2.lat = (p0.lat + p1.lat) / 2;

		tempControlPoints.push_back(GVCoord(p0.lon-p2.lon, p0.lat-p2.lat,0) + curPoint);
		tempControlPoints.push_back(GVCoord(curPoint));
		tempControlPoints.push_back(GVCoord(p1.lon-p2.lon, p1.lat-p2.lat,0) + curPoint);

	}
	tempControlPoints.push_back(controlPoints[controlPoints.size()-1]);
	tempControlPoints.push_back(controlPoints[controlPoints.size()-1]);
	bezierPoints->clear();
	int factors[4] = {1,3,3,1}; //贝塞尔曲线的多项式系数
	int num = tempControlPoints.size();
	std::vector<double> lon_interpolation;
	if(num >= 2)
	{
		for(int i = 1,j=0; i <= num-1; i++,j++)
		{
			double lon_inter = tempControlPoints[i].lon - tempControlPoints[i-1].lon;
			lon_interpolation.push_back(lon_inter);
			if(abs(lon_interpolation[j]) > 180)
			{
				if(lon_interpolation[j] > 0)
					lon_interpolation[j] = lon_interpolation[j] - 360;
				else
					lon_interpolation[j] = lon_interpolation[j] + 360;
			}
			tempControlPoints[i].lon = tempControlPoints[i-1].lon + lon_interpolation[j];
		}
	}
	int size = 20;
	for(int index=0; index <= tempControlPoints.size()-4; index += 3)
	{
		//计算贝塞尔曲线上的点（3阶）
		double t = 0.0;
		for(int i=0; i < size; i++)
		{
			t = i / (double)size;
			double lon = 0.0;
			double lat = 0.0;

			for(int j = 0; j < 4; j++)
			{
				lon += factors[j] * pow(1-t,3-j) * pow(t,j) * tempControlPoints[index + j].lon;
				lat += factors[j] * pow(1-t,3-j) * pow(t,j) * tempControlPoints[index + j].lat;
			}
			bezierPoints->push_back(GVCoord(lon,lat,0));
		}
	}
	bezierPoints->push_back(tempControlPoints[tempControlPoints.size()-1]);
}