#include "ConfigControlPoints.h"

void ConfigControlPoints::configControlPoints(std::vector<GVCoord> &_controlPoints)
{
	int length = _controlPoints.size();
	std::vector<double> lon_interpolation;;

	if(length >= 2)
	{
		for (int i=1, j=0; i<length; i++, j++)
		{
			double lon_inter = _controlPoints[i].lon - _controlPoints[i-1].lon;
			lon_interpolation.push_back(lon_inter);
			if (abs(lon_interpolation[j]) > 180)
			{
				if(lon_interpolation[j] > 0)
					lon_interpolation[j] = lon_interpolation[j] - 360;
				else
					lon_interpolation[j] = lon_interpolation[j] + 360;
			}
			_controlPoints[i].lon = _controlPoints[i-1].lon + lon_interpolation[j];
		}
	}
}

void ConfigControlPoints::configControlPoints(GVCoord &vtx, std::vector<GVCoord> &_controlPoints)
{
	int cpSize = _controlPoints.size();
	double LON = 0.0;
	for (int i=0; i<cpSize; i++)
	{
		LON += _controlPoints[i].lon;
	}

	LON /= cpSize;
	if (vtx.lon - LON > 180)
	{
		vtx.lon -= 360;
	}
	else if(vtx.lon - LON < -180)
	{
		vtx.lon += 360;
	}
}

void ConfigControlPoints::configControlPoints(int idx, std::vector<GVCoord> &_controlPoints)
{
	if(_controlPoints.size() < 2)
		return;

	std::vector<double> deltaList;

	if (idx > 0)
	{
		for (int i = idx; i < _controlPoints.size(); i++)
		{
			double delta = _controlPoints[i].lon - _controlPoints[i-1].lon;
			deltaList.push_back(delta);
		}

		for (int i=0; i < deltaList.size(); i++, idx++)
		{
			if (abs(deltaList.at(i)) > 180)
			{
				if(deltaList.at(i) > 0)
					deltaList.at(i) = deltaList.at(i) - 360;
				else
					deltaList.at(i) = deltaList.at(i) + 360;
			}
			_controlPoints[idx].lon = _controlPoints[idx-1].lon + deltaList.at(i);
		}
	}
	else
	{
		for (int i=1; i<_controlPoints.size(); i++)
		{
			double delta = _controlPoints[i].lon - _controlPoints[i-1].lon;
			deltaList.push_back(delta);
		}

		for (int i=0; i<deltaList.size(); i++)
		{
			if(abs(deltaList.at(i)) > 180)
			{
				if(deltaList.at(i) > 0)
					deltaList.at(i) = deltaList.at(i) - 360;
				else
					deltaList.at(i) = deltaList.at(i) + 360;
			}
		}

		for (int i=deltaList.size(); i>0; i--)
		{
			_controlPoints[i-1].lon = _controlPoints[i].lon - deltaList.at(i-1);
		}
	}
}