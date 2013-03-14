#define _USE_MATH_DEFINES

#include "pixel.hpp"
#include <cmath>
#include <iostream>

using namespace std;

// Class definition for Pixels
class Pixel {
	// The values X and Y correspond to the pixel's location.
	// The value for color
	int X;
	int Y;

	// The following two-dimensional array corresponds to the type of encompassing triangle.
	// 0 - Red, 1 - Blue
	// 0 - Top piece, 1 - Bottom piece - (T/B corresponds to location on Ianiv Schweber's table)
	// The third entry is the rotation.
	// The value 0 refers to the position in the downward pointing star in the top-left position.
	// From there, the subsequent value increments the angle clockwise by 36 degrees.
	int triType[3];

	// Keeps track of which Chuck Close layer this pixel lies on
	int layer;

	// The X's and Y's correspond to the encompassing triangle's vertices.
	// If Pixel is red, the P's correspond to the point on the long edge, and
	// the Q's correspond to the point on one of the shorter edges.
	// Otherwise, the P's correspond to the only other point and the Q's are null.
	double X1, X2, X3, Y1, Y2, Y3, P1, P2, Q1, Q2;

	bool isColored;
public:

	int
	getX()
	{
		return X;
	}

	void
	setX(int x) {
		X = x;
	}

	void
	setY(int y) {
		Y = y;
	}

	void
	color() {
		isColored = true;
	}

	bool
	coloredYet() {
		return isColored;
	}

	double
	getX1() {
		return X1;
	}

	double
	getY1() {
		return Y1;
	}

	double
	getX2() {
		return X2;
	}

	double
	getY2() {
		return Y2;
	}

	double
	getX3() {
		return X3;
	}

	double
	getY3() {
		return Y3;
	}

	void
	setTypes(int* type) {
		isColored = false;
		layer = 0;
		triType[0] = type[0];
		triType[1] = type[1];
		triType[2] = type[2];
	}

	// Returns 0 if containing triangle is a 108/36/36 triangle.
	// Returns 1 if containing triangle is a 36/72/72 triangle.
	int
	getType() {
		return triType[0];
	}

	// Returns 0 if containing triangle is a top-rotation triangle
	// Returns 1 if containing triangle is a bottom-rotation triangle
	// See Ianiv Schweber's table for specification.
	int
	getRot() {
		return triType[1];
	}

	int
	getSpin() {
		return triType[2];
	}

	int
	getLayer() {
		return layer;
	}

	void
	setVertices(double x1, double y1, double x2, double y2, double x3, double y3)
	{
		X1 = x1;
		Y1 = y1;
		X2 = x2;
		Y2 = y2;
		X3 = x3;
		Y3 = y3;

		// Evaluate the P and (possibly) Q point
		if ( triType[0] == 0 ) {
			if ( triType[1] == 0 ) {
				// First, we determine the values for P1 and P2.
				if ( triType[2] % 5 == 4 )
					P1 = X2;
				else if ( triType[2] % 5 == 1 )
					P1 = X1;
				else {
					P1 = ((Y1 - Y3)/(X1 - X3))*X1 - tan(((36 * triType[2]) - 54) / (180 / M_PI))*X2 + Y2 - Y1;
					P1 = P1 / ((Y1 - Y3)/(X1 - X3) - tan(((36 * triType[2]) - 54) / (180 / M_PI)));
				}
				if ( triType[2] % 5 == 1 )
					P2 = tan((0 - 18)/(180 / M_PI))*(P1 - X2) + Y2;
				else
					P2 = ((Y1 - Y3)/(X1 - X3))*(P1 - X1) + Y1;

				// Then, we find the (Q1, Q2).
				if ( triType[2] % 5 == 0 )
					Q1 = P1;
				else if ( triType[2] % 5 == 2 )
					Q1 = X2;
				else {
					Q1 = ((Y2 - Y3)/(X2 - X3))*X2 - tan(((36 * triType[2]) - 90) / (180 / M_PI))*P1 + P2 - Y2;
					Q1 = Q1 / (((Y2 - Y3)/(X2 - X3)) - tan(((36 * triType[2]) - 90) / (180 / M_PI)));
				}
				if ( triType[2] % 5 == 2 )
					Q2 = tan(((36 * (triType[2] - 2)) - 18)/(180 / M_PI))*(Q1 - P1) + P2;
				else
					Q2 = ((Y2 - Y3)/(X2 - X3))*(Q1 - X2) + Y2;

			}
			else {
				if ( triType[2] % 5 == 3 )
					P1 = X1;
				else if ( triType[2] % 5 == 1 )
					P1 = X2;
				else {
					P1 = ((Y3 - Y2)/(X3 - X2))*X3 - tan(((36 * triType[2]) - 18) / (180 / M_PI))*X1 + Y1 - Y3;
					P1 = P1 / (((Y3 - Y2)/(X3 - X2)) - tan(((36 * triType[2]) - 18) / (180 / M_PI)));
				}
				if ( triType[2] % 5 == 1 )
					P2 = tan((18)/(180 / M_PI))*(P1 - X1) + Y1;
				else
					P2 = ((Y3 - Y2)/(X3 - X2))*(P1 - X3) + Y3;

				if ( triType[2] % 5 == 0 )
					Q1 = X1;
				else if ( triType[2] % 5 == 2 )
					Q1 = P1;
				else {
					Q1 = ((Y3 - Y1)/(X3 - X1))*X3 - tan((18 + (36 * triType[2])) / (180 / M_PI))*P1 + P2 - Y3;
					Q1 = Q1 / (((Y3 - Y1)/(X3 - X1)) - tan((18 + (36 * triType[2])) / (180 / M_PI)));
				}
				if ( triType[2] % 5 == 0 )
					Q2 = tan((18 + (36 * triType[2]))/(180 / M_PI))*(Q1 - P1) + P2;
				else
					Q2 = ((Y3 - Y1)/(X3 - X1))*(Q1 - X3) + Y3;
			}
		}
		else {
			if ( triType[1] == 0 ) {
				if (triType[2] == 4 || triType[2] == 9) {
					P1 = X2;
				}
				else if (triType[2] == 2 || triType[2] == 7) {
					P1 = X1;
				}
				else {
					P1 = ((Y1 - Y3)/(X1 - X3))*X3 - tan(((36 * triType[2]) - 54)/(180 / M_PI))*X2 + Y2 - Y3;
					P1 = P1 / (((Y1 - Y3)/(X1 - X3)) - tan(((36 * triType[2]) - 54)/(180 / M_PI)));
				}
				if (triType[2] == 4 || triType[2] == 9) {
					P2 = ((Y3 - Y1)/(X3 - X1))*(P1 - X1) + Y1;
				}
				else {
					P2 = tan(((36 * triType[2]) - 54)/(180 / M_PI))*(P1 - X2) + Y2;
				}
			}
			else {
				if (triType[2] == 3 || triType[2] == 8) {
					P1 = X1;
				}
				else if (triType[2] == 1 || triType[2] == 6) {
					P1 = X3;
				}
				else {
					P1 = ((Y1 - Y2)/(X1 - X2))*X2 - tan((54 + (36 * triType[2]))/(180 / M_PI))*X3 + Y3 - Y2;
					P1 = P1 / (((Y1 - Y2)/(X1 - X2)) - tan((54 + (36 * triType[2]))/(180 / M_PI)));
				}
				if (triType[2] == 1 || triType[2] == 6) {
					P2 = ((Y2 - Y1)/(X2 - X1))*(P1 - X2) + Y2;
				}
				else {
					P2 = tan(((36 * triType[2]) + 54)/(180 / M_PI))*(P1 - X3) + Y3;
				}
			}
		}
	}

	void
	changeLt() {
		if ( triType[2] == 0 ) {
			if ( X < P1 ) {
				triType[1] = 1;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(Q1, Q2, X3, Y3, P1, P2);
			}
			else if ( Y > tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
				triType[2] = (triType[2] + 6) % 10;
				setVertices(X2, Y2, P1, P2, X1, Y1);
			}
			else {
				triType[0] = 1;
				triType[1] = 1;
				triType[2] = (triType[2] + 3) % 10;
				setVertices(P1, P2, Q1, Q2, X2, Y2);
			}
		}
		else if ((triType[2] == 1) || (triType[2] == 2) || (triType[2] == 3)) {
			if ( Y < tan((0 - 90 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
				triType[1] = 1;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(Q1, Q2, X3, Y3, P1, P2);
			}
			else if ( Y > tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
				triType[2] = (triType[2] + 6) % 10;
				setVertices(X2, Y2, P1, P2, X1, Y1);
			}
			else {
				triType[0] = 1;
				triType[1] = 1;
				triType[2] = (triType[2] + 3) % 10;
				setVertices(P1, P2, Q1, Q2, X2, Y2);
			}
		}
		else if ( triType [2] == 4 ) {
			if ( Y < tan((0 - 90 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
				triType[1] = 1;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(Q1, Q2, X3, Y3, P1, P2);
			}
			else if ( X < X2 ) {
				triType[2] = (triType[2] + 6) % 10;
				setVertices(X2, Y2, P1, P2, X1, Y1);
			}
			else {
				triType[0] = 1;
				triType[1] = 1;
				triType[2] = (triType[2] + 3) % 10;
				setVertices(P1, P2, Q1, Q2, X2, Y2);
			}
		}
		else if ( triType[2] == 5 ) {
			if ( X > P1 ) {
				triType[1] = 1;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(Q1, Q2, X3, Y3, P1, P2);
			}
			else if ( Y < tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
				triType[2] = (triType[2] + 6) % 10;
				setVertices(X2, Y2, P1, P2, X1, Y1);
			}
			else {
				triType[0] = 1;
				triType[1] = 1;
				triType[2] = (triType[2] + 3) % 10;
				setVertices(P1, P2, Q1, Q2, X2, Y2);
			}
		}
		else if ( (triType[2] == 6) || (triType[2] == 7) || (triType[2] == 8) ) {
			if ( Y > tan((0 - 90 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
				triType[1] = 1;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(Q1, Q2, X3, Y3, P1, P2);
			}
			else if ( Y < tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
				triType[2] = (triType[2] + 6) % 10;
				setVertices(X2, Y2, P1, P2, X1, Y1);
			}
			else {
				triType[0] = 1;
				triType[1] = 1;
				triType[2] = (triType[2] + 3) % 10;
				setVertices(P1, P2, Q1, Q2, X2, Y2);
			}
		}
		else {
			if ( Y > tan((0 - 90 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
				triType[1] = 1;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(Q1, Q2, X3, Y3, P1, P2);
			}
			else if ( X > P1 ) {
				triType[2] = (triType[2] + 6) % 10;
				setVertices(X2, Y2, P1, P2, X1, Y1);
			}
			else {
				triType[0] = 1;
				triType[1] = 1;
				triType[2] = (triType[2] + 3) % 10;
				setVertices(P1, P2, Q1, Q2, X2, Y2);
			}
		}
	}

	void
	changeLT() {
		if ( triType[2] == 2 ) {
			if ( X > P1 ) {
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(X3, Y3, Q1, Q2, P1, P2);
			}
			else if ( Y > tan((0 - 18 + (36 * triType[2]))/(180 / M_PI))*(X - X1) + Y1 ) {
				triType[2] = (triType[2] + 4) % 10;
				setVertices(P1, P2, X1, Y1, X2, Y2);
			}
			else {
				triType[0] = 1;
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(P1, P2, X1, Y1, Q1, Q2);
			}
		}
		else if ( triType[2] == 3 ) {
			if ( Y > tan((18 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(X3, Y3, Q1, Q2, P1, P2);
			}
			else if ( X < P1 ) {
				triType[2] = (triType[2] + 4) % 10;
				setVertices(P1, P2, X1, Y1, X2, Y2);
			}
			else {
				triType[0] = 1;
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(P1, P2, X1, Y1, Q1, Q2);
			}
		}
		else if ( triType[2] > 3 && triType[2] < 7 ) {
			if ( Y > tan((18 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(X3, Y3, Q1, Q2, P1, P2);
			}
			else if ( Y < tan((0 - 18 + (36 * triType[2]))/(180 / M_PI))*(X - X1) + Y1 ) {
				triType[2] = (triType[2] + 4) % 10;
				setVertices(P1, P2, X1, Y1, X2, Y2);
			}
			else {
				triType[0] = 1;
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(P1, P2, X1, Y1, Q1, Q2);
			}
		}
		else if ( triType[2] == 7 ) {
			if ( X < P1 ) {
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(X3, Y3, Q1, Q2, P1, P2);
			}
			else if ( Y < tan((0 - 18 + (36 * triType[2]))/(180 / M_PI))*(X - X1) + Y1 ) {
				triType[2] = (triType[2] + 4) % 10;
				setVertices(P1, P2, X1, Y1, X2, Y2);
			}
			else {
				triType[0] = 1;
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(P1, P2, X1, Y1, Q1, Q2);
			}
		}
		else if ( triType[2] == 8 ) {
			if ( Y < tan((18 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(X3, Y3, Q1, Q2, P1, P2);
			}
			else if ( X > P1 ) {
				triType[2] = (triType[2] + 4) % 10;
				setVertices(P1, P2, X1, Y1, X2, Y2);
			}
			else {
				triType[0] = 1;
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(P1, P2, X1, Y1, Q1, Q2);
			}
		}
		else {
			if ( Y < tan((18 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(X3, Y3, Q1, Q2, P1, P2);
			}
			else if ( Y > tan((0 - 18 + (36 * triType[2]))/(180 / M_PI))*(X - X1) + Y1 ) {
				triType[2] = (triType[2] + 4) % 10;
				setVertices(P1, P2, X1, Y1, X2, Y2);
			}
			else {
				triType[0] = 1;
				triType[1] = 0;
				triType[2] = (triType[2] + 5) % 10;
				setVertices(P1, P2, X1, Y1, Q1, Q2);
			}
		}
	}

	void
	changelt() {
		if ( triType[2] == 4 ) {
			if ( X > P1 ) {
				triType[0] = 0;
				triType[2] = (triType[2] + 3) % 10;
				setVertices(X1, Y1, P1, P2, X2, Y2);
			} else {
				triType[2] = (triType[2] + 3) % 10;
				setVertices(X2, Y2, X3, Y3, P1, P2);
			}
		}
		else if ( triType[2] == 9 ) {
			if ( X < P1 ) {
				triType[0] = 0;
				triType[2] = (triType[2] + 3) % 10;
				setVertices(X1, Y1, P1, P2, X2, Y2);
			} else {
				triType[2] = (triType[2] + 3) % 10;
				setVertices(X2, Y2, X3, Y3, P1, P2);
			}
		}
		else if ( triType[2] < 4 ) {
			if ( Y < tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
				triType[0] = 0;
				triType[2] = (triType[2] + 3) % 10;
				setVertices(X1, Y1, P1, P2, X2, Y2);
			} else {
				triType[2] = (triType[2] + 3) % 10;
				setVertices(X2, Y2, X3, Y3, P1, P2);
			}
		}
		else {
			if ( Y > tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
				triType[0] = 0;
				triType[2] = (triType[2] + 3) % 10;
				setVertices(X1, Y1, P1, P2, X2, Y2);
			} else {
				triType[2] = (triType[2] + 3) % 10;
				setVertices(X2, Y2, X3, Y3, P1, P2);
			}
		}
	}

	void
	changelT() {
		if ( triType[2] == 6 ) {
			if ( X < P1 ) {
				triType[0] = 0;
				triType[2] = (triType[2] + 9) % 10;
				setVertices(P1, P2, X1, Y1, X3, Y3);
			} else {
				triType[2] = (triType[2] + 7) % 10;
				setVertices(X3, Y3, P1, P2, X2, Y2);
			}
		}
		else if ( triType[2] == 1 ) {
			if ( X > P1 ) {
				triType[0] = 0;
				triType[2] = (triType[2] + 9) % 10;
				setVertices(P1, P2, X1, Y1, X3, Y3);
			} else {
				triType[2] = (triType[2] + 7) % 10;
				setVertices(X3, Y3, P1, P2, X2, Y2);
			}
		}
		else if (triType[2] < 6 && triType[2] > 1 ) {
			if ( Y > tan((54 + (36 * triType[2]))/(180 / M_PI))*(X - X3) + Y3 ) {
				triType[0] = 0;
				triType[2] = (triType[2] + 9) % 10;
				setVertices(P1, P2, X1, Y1, X3, Y3);
			} else {
				triType[2] = (triType[2] + 7) % 10;
				setVertices(X3, Y3, P1, P2, X2, Y2);
			}
		}
		else {
			if ( Y < tan((54 + (36 * triType[2]))/(180 / M_PI))*(X - X3) + Y3 ) {
				triType[0] = 0;
				triType[2] = (triType[2] + 9) % 10;
				setVertices(P1, P2, X1, Y1, X3, Y3);
			} else {
				triType[2] = (triType[2] + 7) % 10;
				setVertices(X3, Y3, P1, P2, X2, Y2);
			}
		}
	}

	void
	ellipticize() {
		int dist;
		if (triType[0] == 0) {
			if (triType[2] == 1) {
				if (triType[1] == 0) {
					P1 = Q1 = X1 + (1.0/3)*(X2 - X1);

					P2 = Y3 + (1.0/4)*(Y1 - Y3);
					Q2 = Y3 + (3.0/4)*(Y1 - Y3);

					dist = (17.0/32)*(Y1 - Y3);
				}
				else if (triType[1] == 1) {
					P1 = Q1 = X1 + (2.0/3)*(X2 - X1);

					P2 = Y3 + (1.0/4)*(Y2 - Y3);
					Q2 = Y3 + (3.0/4)*(Y2 - Y3);

					dist = (17.0/32)*(Y2 - Y3);
				}
			}
			else if (triType[2] == 6) {
				if (triType[1] == 0) {
					P1 = Q1 = X2 + (2.0/3)*(X1 - X2);

					P2 = Y1 + (1.0/4)*(Y3 - Y1);
					Q2 = Y1 + (3.0/4)*(Y3 - Y1);

					dist = (17.0/32)*(Y3 - Y1);
				}
				else if (triType[1] == 1) {
					P1 = Q1 = X2 + (1.0/3)*(X1 - X2);

					P2 = Y2 + (1.0/4)*(Y3 - Y2);
					Q2 = Y2 + (3.0/4)*(Y3 - Y2);

					dist = (17.0/32)*(Y3 - Y2);
				}
			}
			else {
				if (triType[1] == 0) {
					// Set midpoint of long edge
					P1 = (1.0/2)*(X1 + X3);
					P2 = (1.0/2)*(Y1 + Y3);

					// Find center of ellipse
					Q1 = P1 + (1.0/3)*(X2 - P1);
					Q2 = P2 + (1.0/3)*(Y2 - P2);

					// Set (P1, P2) onto line with Center - extended from X3
					dist = P2 - Q2;
					P2 = Y3 - dist;
					dist = P1 - Q1;
					P1 = X3 - dist;

					// Find first foci - (Q1, Q2)
					Q1 = (1.0/2)*(P1 + Q1);
					Q2 = (1.0/2)*(P2 + Q2);

					// Find second foci - (P1, P2)
					P1 = P1 + 3*(Q1 - P1);
					P2 = P2 + 3*(Q2 - P2);

					dist = (17.0/32)*sqrt(pow(X1 - X3, 2) + pow(Y1 - Y3, 2));
				}
				else if (triType[1] == 1) {
					// Set midpoint of long edge
					P1 = (1.0/2)*(X2 + X3);
					P2 = (1.0/2)*(Y2 + Y3);

					// Find center of ellipse
					Q1 = X1 + (2.0/3)*(P1 - X1);
					Q2 = Y1 + (2.0/3)*(P2 - Y1);

					// Set (P1, P2) onto line with Center - extended from X3
					dist = Q1 - P1;
					P1 = X3 + dist;
					dist = Q2 - P2;
					P2 = Y3 + dist;

					// Find first foci - (Q1, Q2)
					Q1 = (1.0/2)*(Q1 + P1);
					Q2 = (1.0/2)*(Q2 + P2);

					// Find second foci - (P1, P2)
					P1 = P1 + 3*(Q1 - P1);
					P2 = P2 + 3*(Q2 - P2);

					dist = (17.0/32)*sqrt(pow(X2 - X3, 2) + pow(Y2 - Y3, 2));
				}
			}
		}
		else if (triType[0] == 1) {
			P1 = (1.0/2)*(X2 + X3);
			P2 = (1.0/2)*(Y2 + Y3);

			Q1 = X1 + (10.0/12)*(P1 - X1);
			Q2 = Y1 + (10.0/12)*(P2 - Y1);

			P1 = (1.0/2)*(P1 + X1);
			P2 = (1.0/2)*(P2 + Y1);

			dist = (1.5)*sqrt(pow(P1 - Q1, 2) + pow(P2 - Q2, 2));
		}

		// Determine if pixel lies within ellipse
		if (triType[0] == 0) {
			if (sqrt(pow(X - P1,2) + pow(Y - P2,2)) + sqrt(pow(X - Q1,2) + pow(Y - Q2, 2)) <= (1.1)*dist) {
				layer = 1;
				if (sqrt(pow(X - P1,2) + pow(Y - P2,2)) + sqrt(pow(X - Q1,2) + pow(Y - Q2, 2)) <= dist)
					layer = 2;
			}
		}
		else if (triType[0] == 1) {
			if (sqrt(pow(X - P1,2) + pow(Y - P2,2)) + sqrt(pow(X - Q1,2) + pow(Y - Q2, 2)) <= (1.3)*dist) {
				layer = 1;
				if (sqrt(pow(X - P1,2) + pow(Y - P2,2)) + sqrt(pow(X - Q1,2) + pow(Y - Q2, 2)) <= dist)
					layer = 2;
			}
		}
	}
};