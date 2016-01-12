//
//  bezier.cpp
//  CG_ex5
//
//  Created by Ben Benchaya on 06/01/16.
//  Copyright © 2016 Bbenchaya. All rights reserved.
//

#include "Bezier.hpp"

Bezier::Bezier(int numOfPoints, Vector3f *points){
    this->points = new Vector3f[numOfPoints];
    for (int i = 0; i < numOfPoints; i++) {
        this->points[i] = points[i];
    }
    leftmost = rightmost = false;
    this->numOfPoints = numOfPoints;
    for (int i = 0; i < DEPTH_OF_PASCAL_TRIANGLE; i++) {
        binom[i] = new float[i + 1];
        binom[i][0] = 1;
        for (int j = 1; j < i; j++) {
            binom[i][j] = binom[i - 1][j - 1] + binom[i - 1][j];
        }
        binom[i][i] = 1;
    }
}

Bezier::~Bezier(){
    printf("I was deleted! %p\n", this);
}

Bezier::Bezier(const Bezier &other){
    this->numOfPoints = other.numOfPoints;
    this->points = new Vector3f[numOfPoints];
    for (int i = 0; i < numOfPoints; i++) {
        this->points[i] = other.points[i];
    }
    leftmost = other.leftmost;
    rightmost = other.rightmost;
    for (int i = 0; i < DEPTH_OF_PASCAL_TRIANGLE; i++) {
        binom[i] = new float[i + 1];
        binom[i][0] = 1;
        for (int j = 1; j < i; j++) {
            binom[i][j] = binom[i - 1][j - 1] + binom[i - 1][j];
        }
        binom[i][i] = 1;
    }
}

Vector3f Bezier::getTinXYZ(float t) {
    float x = 0;
    float y = 0;
    float z = 0;
    for (int i = 0; i < numOfPoints; i++) {
        x += binom[numOfPoints - 1][i] * powf(1 - t, numOfPoints - i -1) * powf(t, i) * points[i].x;
        y += binom[numOfPoints - 1][i] * powf(1 - t, numOfPoints - i -1) * powf(t, i) * points[i].y;
        z += binom[numOfPoints - 1][i] * powf(1 - t, numOfPoints - i -1) * powf(t, i) * points[i].z;
    }
    return Vector3f(x, y, z);
}

Bezier** Bezier::split(){
    Bezier *res[2];
    // TODO implement
    return res;
}

void Bezier::moveControlPoint(int index, float x, float y){
    if (isLeftmost() && index == 0)
        return;
    if (isRightmost() && index == numOfPoints - 1)
        return;
    points[index] += Vector3f(x, y, 0);
}

void Bezier::moveAllPoints(float x, float y){
    if (!isLeftmost())
        points[0] += Vector3f(x, y, 0);
    if (!isRightmost())
        points[numOfPoints - 1] += Vector3f(x, y, 0);
    for (int i = 1; i < numOfPoints - 1; i++) {
        points[i] += Vector3f(x, y, 0);
    }
}

void Bezier::setExtremum(int position){
    switch (position) {
        case LEFTMOST:
            leftmost = true;
            rightmost = false;
            break;
        case RIGHTMOST:
            leftmost = false;
            rightmost = true;
            break;
        case INNER_CURVE:
            leftmost = rightmost = false;
    }
}

void Bezier::clamp(int position, float newX){
    setExtremum(position);
    switch (position) {
        case LEFTMOST:
            points[0] = Vector3f(0, 0, 0);
            break;
        case RIGHTMOST:
            points[numOfPoints - 1] = Vector3f(newX, 0, 0);
    }
}

bool Bezier::isLeftmost(){
    return leftmost;
}

bool Bezier::isRightmost(){
    return rightmost;
}

Vector3f Bezier::getPoint(int index){
    return points[index];
}

void Bezier::setPoint(int index, Vector3f newPosition){
    points[index] = newPosition;
}

int Bezier::getNumOfPoints(){
    return numOfPoints;
}