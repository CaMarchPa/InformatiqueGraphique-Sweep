#ifndef POINT_HPP
#define POINT_HPP

#include <cmath>

#include "vector.hpp"
class Vector;

class Point {
private:
	double x;
	double y;
	double z;
public:
	Point();
	Point(const double x, const double y, const double z);
	Point(const Point& p);
	~Point();
	double getX() const;
	double getY() const;
	double getZ() const;
	void setX(const double x);
	void setY(const double y);
	void setZ(const double z);
	bool near(double precision, double x, double y);
	void translate(const Vector& v);
	Point ProjectOnLine(const Point& Point1Line, const Point& Point2Line) const;
	Point ProjectOnLine(const Vector& vecteur, const Point& PLine) const;
	Point ProjectOnPlan(const Point& PointOnPlane, const Vector& NormalOfPlane) const;
	void draw() const;
	void drawLine(const Point& p) const;
};



#endif
