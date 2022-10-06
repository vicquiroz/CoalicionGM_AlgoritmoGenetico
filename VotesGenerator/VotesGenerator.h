#include <iostream>
double euclidian_distance(double x1, double y1, double x2, double y2)
{
	double calculation = pow(pow((x2 - x1), 2) + pow((y2 - y1), 2), 1 / (double)2);
	return calculation;
}