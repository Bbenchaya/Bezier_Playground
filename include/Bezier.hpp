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

#include "Vector3f.h"

#include <GLUT/GLUT.h>

#define LEFTMOST 0
#define RIGHTMOST 1
#define INNER_CURVE 2
#define DEPTH_OF_PASCAL_TRIANGLE 11
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

class Bezier {
    
private:
    Vector3f *points;
    int numOfPoints;
    bool leftmost;
    bool rightmost;
    /*
     * Since binomial coefficients are used, it is very effective to store them in advance using Pascal's triangle.
     */
    float *binom[DEPTH_OF_PASCAL_TRIANGLE];
    
public:
    Bezier(int numOfPoints, Vector3f *points);
    ~Bezier();
    Bezier(const Bezier &other);
    Vector3f getTinXYZ(float t);
    Bezier** split();
    void moveControlPoint(int index, float x, float y);
    void moveAllPoints(float x, float y);
    void setExtremum(int position);
    void clamp(int position, float newX);
    bool isLeftmost();
    bool isRightmost();
    Vector3f getPoint(int index);
    void setPoint(int index, Vector3f newPosition);
    int getNumOfPoints();
};


#endif /* Bezier_h */
