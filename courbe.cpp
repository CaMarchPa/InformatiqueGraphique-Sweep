#include "courbe.hpp"

#include <cstdlib>
#include <cstdio>

unsigned long factoriel(int);

Courbe::Courbe() : nbPoints(0), tabPointsOfCurve(NULL) {}
Courbe::Courbe(const Point P0, const Point P1, const Vector V0, const Vector V1, const long nbU) { this->genererCubiqueHermite(P0, P1, V0, V1, nbU); }
Courbe::Courbe(const Point tabControlPoint[], const long nbControlPoint, const long nbU) { this->genererBezierCasteljau(tabControlPoint, nbControlPoint, nbU); }
Courbe::~Courbe() {}

void Courbe::genererCubiqueHermite(const Point P0, const Point P1, const Vector V0, const Vector V1, const long nbU) {
	this->tabPointsOfCurve = new Point[nbU];
	this->nbPoints = nbU;
	for(int i = 0; i < nbU; i++) {
		double u = ((double)i)/nbU;
		double F1 = 2*u*u*u - 3*u*u + 1;
		double F2 = -2*u*u*u + 3*u*u;
		double F3 = u*u*u - 2*u*u + u;
		double F4 = u*u*u - u*u;
		Point p(F1*P0.getX() + F2*P1.getX() + F3*V0.getX() + F4*V1.getX(),
				F1*P0.getY() + F2*P1.getY() + F3*V0.getY() + F4*V1.getY(),
				F1*P0.getZ() + F2*P1.getZ() + F3*V0.getZ() + F4*V1.getZ());
		tabPointsOfCurve[i] = p;
	}
}

void Courbe::genererBezierBernstein(const Point tabControlPoint[], const long nbControlPoint, const long nbU) {
	this->tabPointsOfCurve = new Point[nbU];
	this->nbPoints = nbU;
	for(int i = 0; i < nbU; i++) {
		double u = ((double)i)/nbU;
		double x = 0;
		double y = 0;
		double z = 0;
		for(int j = 0; j < nbControlPoint; j++) {
			double Bu = (factoriel(nbControlPoint - 1) / (factoriel(j) * factoriel(nbControlPoint - j - 1))) * pow(u, j) * pow(1-u, nbControlPoint-j-1);
			x += Bu * tabControlPoint[j].getX();
			y += Bu * tabControlPoint[j].getY();
			z += Bu * tabControlPoint[j].getZ();
		}
		Point p(x, y, z);
		this->tabPointsOfCurve[i] = p;
	}
	Point p(tabControlPoint[nbControlPoint-1]);
	this->tabPointsOfCurve[nbU-1] = p;
}

void Courbe::genererBezierCasteljau(const Point tabControlPoint[], const long nbControlPoint, const long nbU) {
	this->tabPointsOfCurve = new Point[nbU+1];
	this->nbPoints = nbU+1;
	this->nbControlPoint = nbControlPoint;
	this->tabPointsIntermediary = new Point**[nbU+1]; // Initialisation intermediaire 1 : Un ensemble de points intermediaires par point de la courbe.
	for(int i = 0; i <= nbU; i++) {
		double u = ((double)i)/nbU;

		// Initialisation intermediaire 2
		this->tabPointsIntermediary[i] = new Point*[nbControlPoint - 1]; // Etapes intermediaires (Ne contient pas les points de controles)
		for (int j = 0; j < nbControlPoint - 1; j++) {
			this->tabPointsIntermediary[i][j] = new Point[nbControlPoint - 1 - j]; // Nombre de points à l'étape courante (à tracer).
		}

		Point p = this->casteljau(tabControlPoint, nbControlPoint, u, i, 0, nbControlPoint - 1);

		this->tabPointsOfCurve[i] = p;
	}
}

Point Courbe::casteljau(const Point tabControlPoint[], const long nbControlPoint, const double u, const int i, const int j, const int k) {
	if (k == 0) { // Point final
		return tabControlPoint[j];
	} else { // Points intermediaires
		Point p1 = this->casteljau(tabControlPoint, nbControlPoint, u, i, j, k-1);
		Point p2 = this->casteljau(tabControlPoint, nbControlPoint, u, i, j + 1, k-1);
		double x = (1 - u) * p1.getX() + u * p2.getX();
		double y = (1 - u) * p1.getY() + u * p2.getY();
		double z = (1 - u) * p1.getZ() + u * p2.getZ();

		Point p(x, y, z);
		//printf("%d:%d:%d\n", i, k-1, j);
		this->tabPointsIntermediary[i][k-1][j] = p;
		return p;
	}	
}


void Courbe::drawCurve() {
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i < this->nbPoints; i++) {
			glVertex3f(this->tabPointsOfCurve[i].getX(), this->tabPointsOfCurve[i].getY(), this->tabPointsOfCurve[i].getZ());
		}
	glEnd();
}

void Courbe::drawIntermediary(const int i) {
	// Dessine les points intermediaires du point i de la courbe de casteljau.

	for (int j = 0; j < this->nbControlPoint - 1; j++) {
		glColor3f(rand()/(double)RAND_MAX, rand()/(double)RAND_MAX, rand()/(double)RAND_MAX);
		//printf("%f\n", rand()/(double)RAND_MAX);
		glBegin(GL_LINE_STRIP);
		for (int k = 0; k < this->nbControlPoint - 1 - j; k++) {
			glVertex3f(tabPointsIntermediary[i][j][k].getX(), tabPointsIntermediary[i][j][k].getY(), tabPointsIntermediary[i][j][k].getZ());
		}
		glEnd();
		for (int k = 0; k < this->nbControlPoint - 1 - j; k++) {
			this->tabPointsIntermediary[i][j][k].draw();
		}		
	}
}

Point* Courbe::getPoints() const {
	return this->tabPointsOfCurve;
}

unsigned long factoriel(int i) {
	if (i == 0) { return 1; }
	else { return (i * factoriel(i-1)); }
}