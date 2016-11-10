#include "point.hpp"

Point::Point() : x(0), y(0), z(0) {}
Point::Point(const double x, const double y, const double z) : x(x), y(y), z(z) {}
Point::Point(const Point& p) : x(p.x), y(p.y), z(p.z) {}
Point::~Point() {}
double Point::getX() const { return x; }
double Point::getY() const { return y; }
double Point::getZ() const { return z; }
void Point::setX(const double x) { this->x = x; }
void Point::setY(const double y) { this->y = y; }
void Point::setZ(const double z) { this->z = z; }

bool Point::near(double precision, double x, double y) {
	if (this->x > x - precision && this->x < x + precision && this->y > y - precision && this->y < y + precision) {
		return true;
	} else {
		return false;
	}
}

void Point::translate(const Vector& v) {
	this->x += v.getX();
	this->y += v.getY();
	this->z += v.getZ();
}

Point Point::ProjectOnLine(const Point& Point1Line, const Point& Point2Line) const {
	Vector bc(Point1Line, Point2Line);
	Vector ba(Point1Line, *this);
	double baPrime = ba.scalar(bc) / bc.norme();
	Vector u(bc); 
	u.normalize();// u = bc normé

	Point r(Point1Line.getX() + u.getX() * baPrime,
			Point1Line.getY() + u.getY() * baPrime,
			Point1Line.getZ() + u.getZ() * baPrime);
	return r;
}

Point Point::ProjectOnLine(const Vector& vecteur, const Point& PLine) const {
	Point p2(PLine);
	p2.translate(vecteur);
	return this->ProjectOnLine(PLine, p2);
}

Point Point::ProjectOnPlan(const Point& PointOnPlane, const Vector& NormalOfPlane) const {
	Vector ma(*this, PointOnPlane);
	double distanceAuPlan = ma.scalar(NormalOfPlane) / NormalOfPlane.norme();
	Point r(this->x - NormalOfPlane.getX() * distanceAuPlan,
			this->y - NormalOfPlane.getY() * distanceAuPlan,
			this->z - NormalOfPlane.getZ() * distanceAuPlan);
	return r;
}

void Point::draw() const {
	double demicoteCarre = 0.02;
	// Face avant
	glBegin(GL_LINE_STRIP);
		glVertex3f(this->x-demicoteCarre, this->y-demicoteCarre, this->z+demicoteCarre);
		glVertex3f(this->x-demicoteCarre, this->y+demicoteCarre, this->z+demicoteCarre);
		glVertex3f(this->x+demicoteCarre, this->y+demicoteCarre, this->z+demicoteCarre);
		glVertex3f(this->x+demicoteCarre, this->y-demicoteCarre, this->z+demicoteCarre); 
		glVertex3f(this->x-demicoteCarre, this->y-demicoteCarre, this->z+demicoteCarre);
	glEnd();

	// Face arrière
	glBegin(GL_LINE_STRIP);
		glVertex3f(this->x-demicoteCarre, this->y-demicoteCarre, this->z-demicoteCarre);
		glVertex3f(this->x-demicoteCarre, this->y+demicoteCarre, this->z-demicoteCarre);
		glVertex3f(this->x+demicoteCarre, this->y+demicoteCarre, this->z-demicoteCarre);
		glVertex3f(this->x+demicoteCarre, this->y-demicoteCarre, this->z-demicoteCarre); 
		glVertex3f(this->x-demicoteCarre, this->y-demicoteCarre, this->z-demicoteCarre);
	glEnd();

	//Arête haut gauche
	glBegin(GL_LINES);
		glVertex3f(this->x-demicoteCarre, this->y-demicoteCarre, this->z+demicoteCarre);
		glVertex3f(this->x-demicoteCarre, this->y-demicoteCarre, this->z-demicoteCarre);
	glEnd();

	//Arête haut droite
	glBegin(GL_LINES);
		glVertex3f(this->x+demicoteCarre, this->y-demicoteCarre, this->z+demicoteCarre);
		glVertex3f(this->x+demicoteCarre, this->y-demicoteCarre, this->z-demicoteCarre);
	glEnd();

	//Arête bas droite
	glBegin(GL_LINES);
		glVertex3f(this->x+demicoteCarre, this->y+demicoteCarre, this->z+demicoteCarre);
		glVertex3f(this->x+demicoteCarre, this->y+demicoteCarre, this->z-demicoteCarre);
	glEnd();

	//Arête bas gauche
	glBegin(GL_LINES);
		glVertex3f(this->x-demicoteCarre, this->y+demicoteCarre, this->z+demicoteCarre);
		glVertex3f(this->x-demicoteCarre, this->y+demicoteCarre, this->z-demicoteCarre);
	glEnd();
}

void Point::drawLine(const Point& p) const {
	glBegin(GL_LINES);
		glVertex3f(this->x, this->y, this->z);
		glVertex3f(p.getX(), p.getY(), p.getZ());
	glEnd();
}

