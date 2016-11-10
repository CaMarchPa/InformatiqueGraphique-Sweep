#ifndef COURBE_HPP
#define COURBE_HPP

#include "point.hpp"
#include "vector.hpp"

#include <cmath>

class Courbe {
private:
	long nbPoints, nbControlPoint;
	Point* tabPointsOfCurve;
	Point*** tabPointsIntermediary; // U, Etape, Point
	Point casteljau(const Point tabControlPoint[], const long nbControlPoint, const double u, const int i, const int j, const int k);
public:
	Courbe();
	Courbe(const Point P0, const Point P1, const Vector V0, const Vector V1, const long nbU);
	Courbe(const Point tabControlPoint[], const long nbControlPoint, const long nbU);
	~Courbe();
	void genererCubiqueHermite(const Point P0, const Point P1, const  Vector V0, const  Vector V1, const  long nbU);
	void genererBezierBernstein(const Point tabControlPoint[], const long nbControlPoint, const long nbU);
	void genererBezierCasteljau(const Point tabControlPoint[], const long nbControlPoint, const long nbU);
	void drawCurve();
	void drawIntermediary(const int i);
	Point* getPoints() const;
};

#endif