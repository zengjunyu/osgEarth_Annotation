#include "KidNey.h"

void KidNey::setKidNeyConreolPoints(std::vector<GVCoord> &controlpoints, static std::vector<GVCoord> vtxBuffer)
{
	if(vtxBuffer.size() <= 3){
		controlpoints = vtxBuffer;
	}
	else
	{
		controlpoints.push_back(vtxBuffer[0]);
		controlpoints.push_back(vtxBuffer[1]);
		controlpoints.push_back(vtxBuffer[2]);
	}

	int count = controlpoints.size();

	if (count >= 2)
	{
		double point1x = controlpoints[0].lon;
		double point1y = controlpoints[0].lat;
		double point2x = controlpoints[1].lon;
		double point2y = controlpoints[1].lat;
		double point3x, point3y;
		double pointx = (point2x - point1x) / 2.0;
		double pointy = (point2y - point1y) / 2.0;
		double point4x = -pointy;
		double point4y = pointx;
		double dir_t = 1.0;

		if(count == 2)
		{
			point3x = point1x + pointx + point4x * 1.7;
			point3y = point1y + pointy + point4y * 1.7;
			controlpoints.push_back(GVCoord(point3x, point3y, 0));
		}
		else
		{
			point3x = controlpoints[2].lon;
			point3y = controlpoints[2].lat;
		}
		double allL = sqrt(pow(pointx * 2, 2) + pow(pointy * 2, 2));
		double allh = sqrt(pow((point3x - point1x), 2) + pow((point3y - point1y), 2) - pow(((point3x - point1x) * (point2x - point1x) + (point3y - point1y) * (point2y - point1y)) / allL, 2));
		double mozhi1 = ((point3x - point1x) * (point2x - point1x) + (point3y - point1y) * (point2y - point1y)) / allL;

		if(mozhi1 > 0.8 * allL)
		{
			mozhi1 = 0.8 * allL;
		}
		else if(mozhi1 < 0.2 * allL)
		{
			mozhi1 = 0.2 * allL;
		}

		if(allh > 1.15 * allL)
		{
			allh = 1.15 * allL;
		}
		else if(allh < 0.2 * allL)
		{
			allh = 0.2 * allL;
		}

		if((point3x - point1x) * point4x + (point3y - point1y) *point4y > 0)
		{
			point3x = point1x + mozhi1 * pointx / sqrt(pow(pointx, 2) + pow(pointy, 2)) + allh * point4x / sqrt(pow(point4x, 2) + pow(point4y, 2));
			point3y = point1y + mozhi1 * pointy / sqrt(pow(pointx, 2) + pow(pointy, 2)) + allh * point4y / sqrt(pow(point4x, 2) + pow(point4y, 2));
		}
		else
		{
			point3x = point1x + mozhi1 * pointx / sqrt(pow(pointx, 2) + pow(pointy, 2)) - allh * point4x / sqrt(pow(point4x, 2) + pow(point4y, 2));
			point3y = point1y + mozhi1 * pointy / sqrt(pow(pointx, 2) + pow(pointy, 2)) - allh * point4y / sqrt(pow(point4x, 2) + pow(point4y, 2));
		}
		controlpoints[2] = GVCoord(point3x, point3y, 0);

		double bx[9], by[9];

		double length = sqrt(pointx * pointx + pointy * pointy);
		pointx = pointx / length;
		pointy = pointy / length;
		point4x = point4x / length;
		point4y = point4y / length;

		if(point4x * (controlpoints[2].lon - controlpoints[0].lon) + point4y * (controlpoints[2].lat - controlpoints[0].lat) < 0)
		{
			point4x = -point4x;
			point4y = -point4y;
			dir_t = -1.0;
		}

		scaleBuffer.clear();
		scaleBuffer.push_back((0.15 + 0.62) / 2.0);
		scaleBuffer.push_back((0.022 + 0.205) / 2.0);
		scaleBuffer.push_back((0.59 + 0.89) / 2.0);
		scaleBuffer.push_back((0.06 + 0.595) / 2.0);
		scaleBuffer.push_back((0.055 + 0.239) / 2.0);
		scaleBuffer.push_back((0.111 + 0.444) / 2.0);
		scaleBuffer.push_back((0.044 + 0.222) / 2.0);
		scaleBuffer.push_back((0.094 + 0.411) / 2.0);
		scaleBuffer.push_back((0.15 + 0.455) / 2.0);

		bx[2] = controlpoints[0].lon + 0.4 * point4x * allh - (scaleBuffer[2] - 0.5) * allL * pointx;
		by[2] = controlpoints[0].lat + 0.4 * point4y * allh - (scaleBuffer[2] - 0.5) * allL * pointy;
		bx[1] = controlpoints[0].lon + scaleBuffer[1] * allL * point4x + 0.5 * allL * pointx;
		by[1] = controlpoints[0].lat + scaleBuffer[1] * allL * point4y + 0.5 * allL * pointy;
		bx[8] = bx[1] - scaleBuffer[8] * allL * pointx;
		by[8] = by[1] - scaleBuffer[8] * allL * pointy;
		bx[4] = bx[2] - scaleBuffer[4] * allL * point4x;
		by[4] = by[2] - scaleBuffer[4] * allL * point4y;
		bx[5] = bx[2] + scaleBuffer[5] * allL * point4x;
		by[5] = by[2] + scaleBuffer[5] * allL * point4y;
		bx[0] = controlpoints[2].lon - scaleBuffer[0] * allL * pointx * dir_t;
		by[0] = controlpoints[2].lat - scaleBuffer[0] * allL * pointy * dir_t;
		bx[3] = controlpoints[2].lon + scaleBuffer[3] * allL * pointx * dir_t;
		by[3] = controlpoints[2].lat + scaleBuffer[3] * allL * pointy * dir_t;

		double point5x = pointx + point4x * 0.3 * allh / allL;
		double point5y = pointy + point4y * 0.3 * allh / allL;
		double length5 = sqrt(point5x * point5x + point5y * point5y);
		point5x = point5x / length5;
		point5y = point5y / length5;
		bx[6] = controlpoints[1].lon + scaleBuffer[6] * allL * point5x;
		by[6] = controlpoints[1].lat + scaleBuffer[6] * allL * point5y;
		bx[7] = controlpoints[1].lon - scaleBuffer[7] * allL * point5x;
		by[7] = controlpoints[1].lat - scaleBuffer[7] * allL * point5y;

		if(controlpoints.size() == 3)
		{
			controlpoints.push_back(GVCoord(bx[0], by[0], 0));
			controlpoints.push_back(GVCoord(bx[1], by[1], 0));
			controlpoints.push_back(GVCoord(bx[2], by[2], 0));
			controlpoints.push_back(GVCoord(bx[3], by[3], 0));
			controlpoints.push_back(GVCoord(bx[4], by[4], 0));
			controlpoints.push_back(GVCoord(bx[5], by[5], 0));
			controlpoints.push_back(GVCoord(bx[6], by[6], 0));
			controlpoints.push_back(GVCoord(bx[7], by[7], 0));
			controlpoints.push_back(GVCoord(bx[8], by[8], 0));
		}
		else
		{
			controlpoints[3] = GVCoord(bx[0], by[0], 0);
			controlpoints[4] = GVCoord(bx[1], by[1], 0);
			controlpoints[5] = GVCoord(bx[2], by[2], 0);
			controlpoints[6] = GVCoord(bx[3], by[3], 0);
			controlpoints[7] = GVCoord(bx[4], by[4], 0);
			controlpoints[8] = GVCoord(bx[5], by[5], 0);
			controlpoints[9] = GVCoord(bx[6], by[6], 0);
			controlpoints[10] = GVCoord(bx[7], by[7], 0);
			controlpoints[11] = GVCoord(bx[8], by[8], 0);
		}
	}
}

void KidNey::drawKidNey(std::vector<GVCoord> controlpoints)
{
	tmpBuffer.clear();
	tmpBuffer2.clear();
	if(controlpoints.size() >= 12)
	{
		double cx[3], cy[3];
		cx[0] = controlpoints[0].lon;
		cy[0] = controlpoints[0].lat;
		cx[1] = controlpoints[1].lon;
		cy[1] = controlpoints[1].lat;
		cx[2] = controlpoints[2].lon;
		cy[2] = controlpoints[2].lat;

		double bx[9], by[9];
		bx[0] = controlpoints[3].lon;
		by[0] = controlpoints[3].lat;
		bx[1] = controlpoints[4].lon;
		by[1] = controlpoints[4].lat;
		bx[2] = controlpoints[5].lon;
		by[2] = controlpoints[5].lat;
		bx[3] = controlpoints[6].lon;
		by[3] = controlpoints[6].lat;
		bx[4] = controlpoints[7].lon;
		by[4] = controlpoints[7].lat;
		bx[5] = controlpoints[8].lon;
		by[5] = controlpoints[8].lat;
		bx[6] = controlpoints[9].lon;
		by[6] = controlpoints[9].lat;
		bx[7] = controlpoints[10].lon;
		by[7] = controlpoints[10].lat;
		bx[8] = controlpoints[11].lon;
		by[8] = controlpoints[11].lat;

		double pointx = (cx[1] - cx[0]) / 2.0;
		double pointy = (cy[1] - cy[0]) / 2.0;
		double point4x = -pointy;
		double point4y = pointx;
		double allL = sqrt(pow(pointx * 2, 2) + pow(pointy * 2, 2));
		double allh = sqrt(pow(cx[2] - cx[0], 2) + pow(cy[2] - cy[0], 2) - pow(((cx[2] - cx[0]) * (cx[1] - cx[0]) + (cy[2] - cy[0]) * (cy[1] - cy[0])) / allL, 2));

		double length = sqrt(pointx * pointx + pointy * pointy);
		pointx = pointx / length;
		pointy = pointy / length;
		point4x = point4x / length;
		point4y = point4y / length;
		double dir_t = 1.0;

		if(point4x * (cx[2] - cx[0]) + point4y * (cy[2] - cy[0]) < 0)
		{
			point4x = -point4x;
			point4y = -point4y;
			dir_t = -1.0;
			double temp;
			temp = bx[0];
			bx[0] = bx[3];
			bx[3] = temp;
			temp = by[0];
			by[0] = by[3];
			by[3] = temp;
		}

		double fx[6], fy[6];
		double projectLength = (bx[1] - bx[2]) * pointx + (by[1] - by[2]) * pointy;
		fx[0] = bx[2] + 2 * projectLength * pointx;
		fy[0] = by[2] + 2 * projectLength * pointy;
		fx[1] = fx[0] - sqrt(pow(bx[2] - bx[4], 2) + pow(by[2] - by[4], 2)) * point4x;
		fy[1] = fy[0] - sqrt(pow(bx[2] - bx[4], 2) + pow(by[2] - by[4], 2)) * point4y;
		fx[2] = fx[0] + sqrt(pow(bx[2] - bx[5], 2) + pow(by[2] - by[5], 2)) * point4x;
		fy[2] = fy[0] + sqrt(pow(bx[2] - bx[5], 2) + pow(by[2] - by[5], 2)) * point4y;
		fx[3] = bx[1] + sqrt(pow(bx[1] - bx[8], 2) + pow(by[1] - by[8], 2)) * pointx;
		fy[3] = by[1] + sqrt(pow(bx[1] - bx[8], 2) + pow(by[1] - by[8], 2)) * pointy;

		double point6x = pointx - point4x * 0.3 * allh / allL;
		double point6y = pointy - point4y * 0.3 * allh / allL;
		double length6 = sqrt(point6x * point6x + point6y * point6y);
		point6x = point6x / length6;
		point6y = point6y / length6;

		fx[4] = cx[0] + sqrt(pow(bx[7] - cx[1], 2) + pow(by[7] - cy[1], 2)) * point6x;
		fy[4] = cy[0] + sqrt(pow(bx[7] - cx[1], 2) + pow(by[7] - cy[1], 2)) * point6y;
		fx[5] = cx[0] - sqrt(pow(bx[6] - cx[1], 2) + pow(by[6] - cy[1], 2)) * point6x;
		fy[5] = cy[0] - sqrt(pow(bx[6] - cx[1], 2) + pow(by[6] - cy[1], 2)) * point6y;

		double n =30.0;
		int t;
		double bzx, bzy, bc;
		for (t = 0; t <= n; t++)
		{
			bc = t / n;
			bzx = pow((1 - bc), 3) * cx[0] + 3 * bc * pow((1 - bc), 2) * fx[4] + 3 * pow(bc, 2) * (1 - bc) * bx[8] + pow(bc, 3) * bx[1];
			bzy = pow((1 - bc), 3) * cy[0] + 3 * bc * pow((1 - bc), 2) * fy[4] + 3 * pow(bc, 2) * (1 - bc) * by[8] + pow(bc, 3) * by[1];
			tmpBuffer.push_back(GVCoord(bzx, bzy, 0));
		}

		for (t = 0; t <= n; t++)
		{
			bc = t / n;
			bzx = pow((1 - bc), 3) * bx[1] + 3 * bc * pow((1 - bc), 2) * fx[3] + 3 * pow(bc, 2) * (1 - bc) * bx[7] + pow(bc, 3) * cx[1];
			bzy = pow((1 - bc), 3) * by[1] + 3 * bc * pow((1 - bc), 2) * fy[3] + 3 * pow(bc, 2) * (1 - bc) * by[7] + pow(bc, 3) * cy[1];
			tmpBuffer.push_back(GVCoord(bzx, bzy, 0));
		}

		for (t = 0; t <= n; t++)
		{
			bc = t / n;
			bzx = pow((1 - bc), 3) * cx[1] + 3 * bc * pow((1 - bc), 2) * bx[6] + 3 * pow(bc, 2) * (1 - bc) * fx[1] + pow(bc, 3) * fx[0];
			bzy = pow((1 - bc), 3) * cy[1] + 3 * bc * pow((1 - bc), 2) * by[6] + 3 * pow(bc, 2) * (1 - bc) * fy[1] + pow(bc, 3) * fy[0];
			tmpBuffer.push_back(GVCoord(bzx, bzy, 0));
		}

		for (t = 0; t <= n; t++)
		{
			bc = t / n;
			bzx = pow((1 - bc), 3) * fx[0] + 3 * bc * pow((1 - bc), 2) * fx[2] + 3 * pow(bc, 2) * (1 - bc) * bx[3] + pow(bc, 3) * cx[2];
			bzy = pow((1 - bc), 3) * fy[0] + 3 * bc * pow((1 - bc), 2) * fy[2] + 3 * pow(bc, 2) * (1 - bc) * by[3] + pow(bc, 3) * cy[2];
			tmpBuffer.push_back(GVCoord(bzx, bzy, 0));
		}

		for (t = 0; t <= n; t++)
		{
			bc = t / n;
			bzx = pow((1 - bc), 3) * cx[2] + 3 * bc * pow((1 - bc), 2) * bx[0] + 3 * pow(bc, 2) * (1 - bc) * bx[5] + pow(bc, 3) * bx[2];
			bzy = pow((1 - bc), 3) * cy[2] + 3 * bc * pow((1 - bc), 2) * by[0] + 3 * pow(bc, 2) * (1 - bc) * by[5] + pow(bc, 3) * by[2];
			tmpBuffer.push_back(GVCoord(bzx, bzy, 0));
		}

		for (t = 0; t <= n; t++)
		{
			bc = t / n;
			bzx = pow((1 - bc), 3) * bx[2] + 3 * bc * pow((1 - bc), 2) * bx[4] + 3 * pow(bc, 2) * (1 - bc) * fx[5] + pow(bc, 3) * cx[0];
			bzy = pow((1 - bc), 3) * by[2] + 3 * bc * pow((1 - bc), 2) * by[4] + 3 * pow(bc, 2) * (1 - bc) * fy[5] + pow(bc, 3) * cy[0];
			tmpBuffer.push_back(GVCoord(bzx, bzy, 0));
		}

		tmpBuffer2.push_back(tmpBuffer.size());
	}
}

void KidNey::setKidNeyConreolPoint(std::vector<GVCoord> &controlpoints, int idx, const GVCoord vtx, std::vector<double> &s)
{
	double pointx;
	double pointy;
	double point4x;
	double point4y;
	double allL;
	double allh;

	pointx = (controlpoints[1].lon - controlpoints[0].lon) / 2.0;
	pointy = (controlpoints[1].lat - controlpoints[0].lat) / 2.0;
	point4x = -pointy;
	point4y = pointx;

	double length = sqrt(pointx * pointx + pointy * pointy);
	pointx = pointx / length;
	pointy = pointy / length;
	point4x = point4x / length;
	point4y = point4y / length;
	double dir_t = 1.0;

	if(point4x * (controlpoints[2].lon - controlpoints[0].lon) + point4y * (controlpoints[2].lat - controlpoints[0].lat) < 0)
	{
		point4x = -point4x;
		point4y = -point4y;
		dir_t = -1.0;
	}

	allL = sqrt(pow(controlpoints[1].lon - controlpoints[0].lon, 2) + pow(controlpoints[1].lat - controlpoints[0].lat, 2));
	allh = sqrt(pow((controlpoints[2].lon - controlpoints[0].lon), 2) + pow((controlpoints[2].lat - controlpoints[0].lat), 2) - pow(((controlpoints[2].lon - controlpoints[0].lon) * (controlpoints[1].lon - controlpoints[0].lon) + (controlpoints[2].lat - controlpoints[0].lat) * (controlpoints[1].lat - controlpoints[0].lat)) / allL, 2));

	double point5x = pointx + point4x * 0.3 * allh / allL;
	double point5y = pointy + point4y * 0.3 * allh / allL;
	double length5 = sqrt(point5x * point5x + point5y * point5y);
	point5x = point5x / length5;
	point5y = point5y / length5;

	if(idx == 0 || idx == 1)
	{
		controlpoints[idx] = vtx;
		pointx = (controlpoints[1].lon - controlpoints[0].lon) / 2.0;
		pointy = (controlpoints[1].lat - controlpoints[0].lat) / 2.0;
		point4x = -pointy;
		point4y = pointx;

		double length = sqrt(pointx * pointx + pointy * pointy);
		pointx = pointx / length;
		pointy = pointy / length;
		point4x = point4x / length;
		point4y = point4y / length;
		dir_t = 1.0;

		if(point4x * (controlpoints[2].lon - controlpoints[0].lon) + point4y * (controlpoints[2].lat - controlpoints[0].lat) < 0)
		{
			point4x = -point4x;
			point4y = -point4y;
			dir_t = -1.0;
		}

		allL = sqrt(pow(controlpoints[1].lon - controlpoints[0].lon, 2) + pow(controlpoints[1].lat - controlpoints[0].lat, 2));
		allh = sqrt(pow((controlpoints[2].lon - controlpoints[0].lon), 2) + pow((controlpoints[2].lat - controlpoints[0].lat), 2) - pow(((controlpoints[2].lon - controlpoints[0].lon) * (controlpoints[1].lon - controlpoints[0].lon) + (controlpoints[2].lat - controlpoints[0].lat) * (controlpoints[1].lat - controlpoints[0].lat)) / allL, 2));

		double bx[9], by[9];

		bx[1] = controlpoints[0].lon + s[1] * allL * point4x + 0.5 * allL * pointx;
		by[1] = controlpoints[0].lat + s[1] * allL * point4y + 0.5 * allL * pointy;
		bx[2] = controlpoints[0].lon + 0.4 * point4x * allh - (s[2] - 0.5) * allL * pointx;
		by[2] = controlpoints[0].lat + 0.4 * point4y * allh - (s[2] - 0.5) * allL * pointy;
		bx[8] = bx[1] - s[8] * allL * pointx;
		by[8] = by[1] - s[8] * allL * pointy;
		bx[4] = bx[2] - s[4] * allL * point4x;
		by[4] = by[2] - s[4] * allL * point4y;
		bx[5] = bx[2] + s[5] * allL * point4x;
		by[5] = by[2] + s[5] * allL * point4y;
		bx[0] = controlpoints[2].lon - s[0] * allL * pointx * dir_t;
		by[0] = controlpoints[2].lat - s[0] * allL * pointy * dir_t;
		bx[3] = controlpoints[2].lon + s[3] * allL * pointx * dir_t;
		by[3] = controlpoints[2].lat + s[3] * allL * pointy * dir_t;

		point5x = pointx + point4x * 0.3 * allh / allL;
		point5y = pointy + point4y * 0.3 * allh / allL;
		length5 = sqrt(point5x * point5x + point5y * point5y);
		point5x = point5x / length5;
		point5y = point5y / length5;

		bx[6] = controlpoints[1].lon + s[6] * allL * point5x;
		by[6] = controlpoints[1].lat + s[6] * allL * point5y;
		bx[7] = controlpoints[1].lon - s[7] * allL * point5x;
		by[7] = controlpoints[1].lat - s[7] * allL * point5y;

		controlpoints[3] = GVCoord(bx[0], by[0], 0);
		controlpoints[4] = GVCoord(bx[1], by[1], 0);
		controlpoints[5] = GVCoord(bx[2], by[2], 0);
		controlpoints[6] = GVCoord(bx[3], by[3], 0);
		controlpoints[7] = GVCoord(bx[4], by[4], 0);
		controlpoints[8] = GVCoord(bx[5], by[5], 0);
		controlpoints[9] = GVCoord(bx[6], by[6], 0);
		controlpoints[10] = GVCoord(bx[7], by[7], 0);
		controlpoints[11] = GVCoord(bx[8], by[8], 0);
	}

	if(idx == 2)
	{
		double point3x = vtx.lon;
		double point3y = vtx.lat;

		allh = sqrt(pow((point3x - controlpoints[0].lon), 2) + pow((point3y - controlpoints[0].lat), 2) - pow(((point3x - controlpoints[0].lon) * (controlpoints[1].lon - controlpoints[0].lon) + (point3y - controlpoints[0].lat) * (controlpoints[1].lat - controlpoints[0].lat)) / allL, 2));

		double mozhi1 = (point3x - controlpoints[0].lon) * pointx + (point3y - controlpoints[0].lat) * pointy;

		if(mozhi1 > 0.8 * allL)
		{
			mozhi1 = 0.8 * allL;
		}
		else if(mozhi1 < 0.2 * allL)
		{
			mozhi1 = 0.2 * allL;
		}

		if(allh > 1.15 * allL)
		{
			allh = 1.15 * allL;
		}
		else if(allh < 0.2 * allL)
		{
			allh = 0.2 * allL;
		}

		if((point3x - controlpoints[0].lon) * point4x + (point3y - controlpoints[0].lat) *point4y > 0)
		{
			point3x = controlpoints[0].lon + mozhi1 * pointx + allh * point4x;
			point3y = controlpoints[0].lat + mozhi1 * pointy + allh * point4y;
		}
		else
		{
			point3x = controlpoints[0].lon + mozhi1 * pointx - allh * point4x;
			point3y = controlpoints[0].lat + mozhi1 * pointy - allh * point4y;
		}
		controlpoints[2] = GVCoord(point3x, point3y, 0);

		double bx[9], by[9];

		bx[2] = controlpoints[0].lon + 0.4 * point4x * allh - (s[2] - 0.5) * allL * pointx;
		by[2] = controlpoints[0].lat + 0.4 * point4y * allh - (s[2] - 0.5) * allL * pointy;
		bx[4] = bx[2] - s[4] * allL * point4x;
		by[4] = by[2] - s[4] * allL * point4y;
		bx[5] = bx[2] + s[5] * allL * point4x;
		by[5] = by[2] + s[5] * allL * point4y;
		bx[0] = controlpoints[2].lon - s[0] * allL * pointx * dir_t;
		by[0] = controlpoints[2].lat - s[0] * allL * pointy * dir_t;
		bx[3] = controlpoints[2].lon + s[3] * allL * pointx * dir_t;
		by[3] = controlpoints[2].lat + s[3] * allL * pointy * dir_t;

		controlpoints[3] = GVCoord(bx[0], by[0], 0);
		controlpoints[5] = GVCoord(bx[2], by[2], 0);
		controlpoints[6] = GVCoord(bx[3], by[3], 0);
		controlpoints[7] = GVCoord(bx[4], by[4], 0);
		controlpoints[8] = GVCoord(bx[5], by[5], 0);
	}

	if (idx == 4)
	{
		double pointMidx = (controlpoints[0].lon + controlpoints[1].lon) / 2.0;
		double pointMidy = (controlpoints[0].lat + controlpoints[1].lat) / 2.0;

		double projectLength = (vtx.lon - pointMidx) * point4x + (vtx.lat - pointMidy) * point4y;

		double min = allL * 0.022;
		double max = allL * 0.205;

		if(projectLength < min)
		{
			projectLength = min;
		}
		else if (projectLength > max)
		{
			projectLength = max;
		}

		controlpoints[idx] = GVCoord(pointMidx + projectLength * point4x, pointMidy + projectLength * point4y, 0);
		controlpoints[11] = GVCoord(controlpoints[idx].lon - s[8] * allL * pointx, controlpoints[idx].lat - s[8] * allL * pointy, 0);

		s[1] = projectLength / allL;
	}

	if (idx == 5)
	{
		double pointTempx = (controlpoints[0].lon + controlpoints[1].lon) / 2.0 + 0.4 * allh * point4x;
		double pointTempy = (controlpoints[0].lat + controlpoints[1].lat) / 2.0 + 0.4 * allh * point4y;

		double projectLength = (vtx.lon - pointTempx) * pointx + (vtx.lat - pointTempy) * pointy;

		double min = allL * 0.59;
		double max = allL * 0.89;

		if(-projectLength < min)
		{
			projectLength = -min;
		}
		else if (-projectLength > max)
		{
			projectLength = -max;
		}

		projectLength = abs(projectLength);

		controlpoints[idx] = GVCoord(pointTempx - projectLength * pointx, pointTempy - projectLength * pointy, 0);
		controlpoints[7] = GVCoord(controlpoints[idx].lon - s[4] * allL * point4x, controlpoints[idx].lat - s[4] * allL * point4y, 0);
		controlpoints[8] = GVCoord(controlpoints[idx].lon + s[5] * allL * point4x, controlpoints[idx].lat + s[5] * allL * point4y, 0);

		s[2] = projectLength / allL;
	}

	if (idx == 3)
	{
		double projectLength = (vtx.lon - controlpoints[2].lon) * pointx * dir_t + (vtx.lat - controlpoints[2].lat) * pointy * dir_t;

		double min = allL * 0.15;
		double max = allL * 0.62;

		if(-projectLength < min)
		{
			projectLength = -min;
		}
		else if (-projectLength > max)
		{
			projectLength = -max;
		}

		projectLength = abs(projectLength);

		controlpoints[idx] = GVCoord(controlpoints[2].lon - projectLength * pointx * dir_t, controlpoints[2].lat - projectLength * pointy * dir_t, 0);

		s[0] = projectLength / allL;
	}

	if (idx == 6)
	{
		double projectLength = (vtx.lon - controlpoints[2].lon) * pointx * dir_t + (vtx.lat - controlpoints[2].lat) * pointy * dir_t;

		double min = allL * 0.06;
		double max = allL * 0.595;

		if(projectLength < min)
		{
			projectLength = min;
		}
		else if (projectLength > max)
		{
			projectLength = max;
		}

		controlpoints[idx] = GVCoord(controlpoints[2].lon + projectLength * pointx * dir_t, controlpoints[2].lat + projectLength * pointy * dir_t, 0);

		s[3] = projectLength / allL;
	}

	if (idx == 7)
	{
		double projectLength = (vtx.lon - controlpoints[5].lon) * point4x + (vtx.lat - controlpoints[5].lat) * point4y;

		double min = allL * 0.055;
		double max = allL * 0.239;

		if(-projectLength < min)
		{
			projectLength = -min;
		}
		else if (-projectLength > max)
		{
			projectLength = -max;
		}

		projectLength = abs(projectLength);

		controlpoints[idx] = GVCoord(controlpoints[5].lon - projectLength * point4x, controlpoints[5].lat - projectLength * point4y, 0);

		s[4] = projectLength / allL;
	}

	if (idx == 8)
	{
		double projectLength = (vtx.lon - controlpoints[5].lon) * point4x + (vtx.lat - controlpoints[5].lat) * point4y;

		double min = allL * 0.111;
		double max = allL * 0.444;

		if(projectLength < min)
		{
			projectLength = min;
		}
		else if (projectLength > max)
		{
			projectLength = max;
		}

		controlpoints[idx] = GVCoord(controlpoints[5].lon + projectLength * point4x, controlpoints[5].lat + projectLength * point4y, 0);

		s[5] = projectLength / allL;
	}

	if (idx == 9)
	{
		double projectLength = (vtx.lon - controlpoints[1].lon) * point5x + (vtx.lat - controlpoints[1].lat) * point5y;

		double min = allL * 0.044;
		double max = allL * 0.222;

		if(projectLength < min)
		{
			projectLength = min;
		}
		else if (projectLength > max)
		{
			projectLength = max;
		}

		controlpoints[idx] = GVCoord(controlpoints[1].lon + projectLength * point5x, controlpoints[1].lat + projectLength * point5y, 0);

		s[6] = projectLength / allL;
	}

	if (idx == 10)
	{
		double projectLength = (vtx.lon - controlpoints[1].lon) * point5x + (vtx.lat - controlpoints[1].lat) * point5y;

		double min = allL * 0.094;
		double max = allL * 0.411;

		if(-projectLength < min)
		{
			projectLength = -min;
		}
		else if (-projectLength > max)
		{
			projectLength = -max;
		}

		projectLength = abs(projectLength);

		controlpoints[idx] = GVCoord(controlpoints[1].lon - projectLength * point5x, controlpoints[1].lat - projectLength * point5y, 0);

		s[7] = projectLength / allL;
	}

	if (idx == 11)
	{
		double projectLength = (vtx.lon - controlpoints[4].lon) * pointx + (vtx.lat - controlpoints[4].lat) * pointy;

		double min = allL * 0.15;
		double max = allL * 0.455;

		if(-projectLength < min)
		{
			projectLength = -min;
		}
		else if (-projectLength > max)
		{
			projectLength = -max;
		}

		projectLength = abs(projectLength);

		controlpoints[idx] = GVCoord(controlpoints[4].lon - projectLength * pointx, controlpoints[4].lat - projectLength * pointy, 0);

		s[8] = projectLength / allL;
	}
}

GVCoord KidNey::getKidNeyCenterPoint(const std::vector<GVCoord> &controlpoints)
{
	int count = controlpoints.size();
	if(count < 12)
	{
		return GVCoord(0, 0, 0);
	}

	double tempPointX = (controlpoints[1].lon + controlpoints[0].lon) / 2;
	double tempPointY = (controlpoints[1].lat + controlpoints[1].lat) / 2;
	double dir1X = controlpoints[1].lon - controlpoints[0].lon;
	double dir1Y = controlpoints[1].lat - controlpoints[0].lat;
	double dir2X = -dir1Y;
	double dir2Y = dir1X;
	double dir3X = controlpoints[2].lon - controlpoints[0].lon;
	double dir3Y = controlpoints[2].lat - controlpoints[0].lat;
	dir2X = dir2X * (dir2X * dir3X + dir2Y * dir3Y) / (pow(dir2X, 2) + pow(dir2Y, 2)); 
	dir2Y = dir2Y * (dir2X * dir3X + dir2Y * dir3Y) / (pow(dir2X, 2) + pow(dir2Y, 2)); 
	double centerPointX = tempPointX + dir2X / 2;
	double centerPointY = tempPointY + dir2Y / 2;

	return GVCoord(centerPointX, centerPointY, controlpoints[0].alt);
}

void KidNey::getKidNeyScales(std::vector<double> &s)
{
	s.clear();
	if(scaleBuffer.size() == 9)
	{
		for(int i=0; i<9; i++)
		{
			s.push_back(scaleBuffer[i]);
		}
	}
	else
	{
		for (int i=0; i<9; i++)
		{
			s.push_back(0.0);
		}
	}
}