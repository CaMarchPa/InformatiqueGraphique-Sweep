#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
//#define _WCHAR_T_DEFINED // A décommmenter si compilation windows
//#include "GL/glut.h"
#include <GL/glut.h>

#include "point.hpp"
class Point;

class Vector {
private:
	double x;
	double y;
	double z;
public:
	Vector();
	Vector(const double x, const double y, const double z);
	Vector(const Vector& v);
	Vector(const Point& a, const Point& b);
	~Vector();
	double getX() const;
	double getY() const;
	double getZ() const;
	void setX(const double x);
	void setY(const double y);
	void setZ(const double z);
	double norme() const;
	void normalize();
	double scalar(const Vector& v) const;
	Vector vectoriel(const Vector& v) const;
	double angle(const Vector& v) const;
	void drawLine(const Point& p) const;
};

#endif