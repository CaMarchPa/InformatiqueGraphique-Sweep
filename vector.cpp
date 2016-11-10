#include "vector.hpp"

Vector::Vector() : x(0), y(0), z(0) {}
Vector::Vector(const double x, const double y, const double z) : x(x), y(y), z(z)  {}
Vector::Vector(const Vector& v) : x(v.x), y(v.y), z(v.z) {}

Vector::Vector(const Point& a, const Point& b) {
	this->x = b.getX() - a.getX(); 
	this->y = b.getY() - a.getY();
	this->z = b.getZ() - a.getZ();
}

Vector::~Vector() {}
double Vector::getX() const { return x; }
double Vector::getY() const { return y; }
double Vector::getZ() const { return z; }
void Vector::setX(const double x) { this->x = x; }
void Vector::setY(const double y) { this->y = y; }
void Vector::setZ(const double z) { this->z = z; }

double Vector::norme() const {
	return sqrt(x*x + y*y + z*z);
}

void Vector::normalize() {
	double n = this->norme();
	this->x = x / n;
	this->y = y / n;
	this->z = z / n;
}

double Vector::scalar(const Vector& v) const {
	return (this->x * v.x + this->y*v.y + this->z*v.z);
}

Vector Vector::vectoriel(const Vector& v) const {
	Vector r = Vector(this->y*v.z - this->z*v.y,
					  this->z*v.x - this->x*v.z,
					  this->x*v.y - this->y*v.x);
	return r;
}

double Vector::angle(const Vector& v) const {
	return acos(this->scalar(v)/(this->norme() * v.norme()));
}

void Vector::drawLine(const Point& o) const {
	Point p(o);
	p.translate(*this);
	o.drawLine(p);	
}