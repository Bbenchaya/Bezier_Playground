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
#define NUM_OF_POINTS_IN_CURVE 4

class Bezier {
    
private:
    Vector3f *points;
    int numOfPoints;
    bool leftmost;
    bool rightmost;
    float *binom[10];
    
public:
    Bezier(int numOfPoints, Vector3f *points);
    Vector3f getTinXYZ(float t);
    Bezier* split();
    void moveControlPoint(int index, float x, float y);
    void moveAllPoints(float x, float y);
    void setExtremum(int position);
    bool isLeftmost();
    bool isRightmost();
    Vector3f getPoint(int index);
    int getNumOfPoints();
};


#endif /* Bezier_h */
