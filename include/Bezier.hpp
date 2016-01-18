//
//  Bezier.hpp
//  CG_ex5
//
//  Created by Asaf Chelouche on 11/1/16.
//  Copyright © 2016 Bbenchaya. All rights reserved.
//

#ifndef Bezier_h
#define Bezier_h

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>

#include "Vector3f.h"

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#elif __linux__
#include "glut.h"
#endif

#define LEFTMOST 0
#define RIGHTMOST 1
#define INNER_CURVE 2
#define DEPTH_OF_PASCAL_TRIANGLE 11
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

using namespace std;

class Bezier {
    
private:
    Vector3f *points;
    int numOfPoints;
    bool leftmost;
    bool rightmost;
    Bezier *next;
    Bezier *previous;
    /*
     * Since binomial coefficients are used, it is very effective to store them in advance using Pascal's triangle.
     * Default depth of the triangle is 11, since 11 is the maximum number of cotrol points per Bezier curve.
     */
    float *binom[DEPTH_OF_PASCAL_TRIANGLE];
    
public:
    Bezier(int numOfPoints, Vector3f *points);
    ~Bezier();
    Bezier(const Bezier &other);
    Vector3f getTinXYZ(float t);
    Bezier** split();
    void moveControlPoint(int index, GLdouble x, GLdouble y);
    void moveAllPoints(GLdouble x, GLdouble GLdouble);
    void setExtremum(int position);
    void clamp(int position, GLdouble newX);
    bool isLeftmost();
    bool isRightmost();
    Vector3f getPoint(int index);
    void setPoint(int index, Vector3f newPosition);
    int getNumOfPoints();
    pair<GLdouble, GLdouble> getP0P1LinearFunction();
    void adjustPnMinus1(pair<GLdouble, GLdouble> linearFunc);
    void setNextCurve(Bezier *next);
    Bezier* getNextCurve();
    void setPreviousCurve(Bezier *previous);
    Bezier* getPreviousCurve();
    bool isLinearCurve();
    void getLinearCurveConvexHull(GLdouble **vals);
    void flatten();
};


#endif /* Bezier_h */
