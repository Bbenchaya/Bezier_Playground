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
    previous = next = NULL;
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
    previous = other.previous;
    next = other.next;
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

void deCasteljau(int depth, int index, Vector3f *midpoints, Vector3f *newPointsLeft, Vector3f *newPointsRight){
    if (depth == 1){
        newPointsLeft[index] = (midpoints[0] + midpoints[1]) / 2;
        newPointsRight[0] = (midpoints[0] + midpoints[1]) / 2;
        return;
    }
    Vector3f *newMidPoints = new Vector3f[depth];
    for (int i = 0; i < depth; i++) {
        newMidPoints[i] = (midpoints[i] + midpoints[i + 1]) / 2;
    }
    newPointsLeft[index] = newMidPoints[0];
    newPointsRight[depth - index] = newMidPoints[depth - 1];
    deCasteljau(depth - 1, index + 1, newMidPoints, newPointsLeft, newPointsRight);
}

Bezier** Bezier::split(){
    Bezier **res = new Bezier*[2];
    Vector3f *midpoints = new Vector3f[numOfPoints];
    for (int i = 0; i < numOfPoints; i++) {
        midpoints[i] = points[i];
    }
    Vector3f *newPointsLeft = new Vector3f[numOfPoints];
    Vector3f *newPointsRight = new Vector3f[numOfPoints];
    newPointsLeft[0] = points[0];
    newPointsRight[numOfPoints - 1] = points[numOfPoints - 1];
    deCasteljau(numOfPoints - 1, 1, midpoints, newPointsLeft, newPointsRight);
    Bezier *left = new Bezier(numOfPoints, newPointsLeft);
    Bezier *right = new Bezier(numOfPoints, newPointsRight);
    if (!isLeftmost()) {
        left->setPreviousCurve(this->getPreviousCurve());
        (this->getPreviousCurve())->setNextCurve(left);
    }
    else
        left->setExtremum(LEFTMOST);
    if (!isRightmost()) {
        right->setNextCurve(this->getNextCurve());
        (this->getNextCurve())->setPreviousCurve(right);
    }
    else
        right->setExtremum(RIGHTMOST);
    left->setNextCurve(right);
    right->setPreviousCurve(left);
    res[0] = left;
    res[1] = right;
    return res;
}

void Bezier::moveControlPoint(int index, GLdouble x, GLdouble y){
    if (isLeftmost() && index == 0) {
        points[index] = Vector3f(x, 0, 0);
        return;
    }
    if (isRightmost() && index == numOfPoints - 1) {
        points[index] = Vector3f(x, 0, 0);
        return;
    }
    points[index] = Vector3f(x, y, 0);
}

void Bezier::moveAllPoints(GLdouble x, GLdouble y){
    if (isLeftmost())
        points[0] += Vector3f(x, 0, 0);
    else
        points[0] += Vector3f(x, y, 0);
    if (isRightmost())
        points[numOfPoints - 1] += Vector3f(x, 0, 0);
    else
        points[numOfPoints - 1] += Vector3f(x, y, 0);
    for (int i = 1; i < numOfPoints - 1; i++) {
        points[i] += Vector3f(x, y, 0);
    }
    if (!isLeftmost()) {
        previous->setPoint(numOfPoints - 1, points[0]);
    }
    if (!isRightmost()) {
        next->setPoint(0, points[numOfPoints - 1]);
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

void Bezier::clamp(int position, GLdouble newX){
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

pair<GLdouble, GLdouble> Bezier::getP0P1LinearFunction(){
    float deltaY = points[1].y - points[0].y;
    float deltaX = points[1].x - points[0].x;
    if (deltaX == 0) {
        return make_pair(0, 0);
    }
    float m = deltaY / deltaX;
    float n = points[0].y - m * points[0].x;
    return make_pair(m, n);
}

void Bezier::adjustPnMinus1(pair<GLdouble, GLdouble> linearFunc){
    if (numOfPoints == 2 && isLeftmost())
        return;
    Vector3f PnMinus1 = points[numOfPoints - 2];
    float newY = linearFunc.first * PnMinus1.x + linearFunc.second;
    points[numOfPoints - 2] = Vector3f(PnMinus1.x, newY, 0);
}

void Bezier::setNextCurve(Bezier *next){
    this->next = next;
}

Bezier* Bezier::getNextCurve(){
    return next;
}

void Bezier::setPreviousCurve(Bezier *previous){
    this->previous = previous;
}

Bezier* Bezier::getPreviousCurve(){
    return previous;
}

bool Bezier::isLinearCurve(){
    float deltaY = points[1].y - points[0].y;
    float deltaX = points[1].x - points[0].x;
    if (deltaX == 0) {
        for (int i = 2; i < numOfPoints; i++) {
            if (points[i].x != points[0].x) {
                return false;
            }
        }
        return true;
    }
    float m = deltaY / deltaX;
    float n = points[0].y - m * points[0].x;
    for (int i = 2; i < numOfPoints; i++) {
        if (points[i].y != m * points[i].x + n) {
            return false;
        }
    }
    return true;
}

void Bezier::getLinearCurveConvexHull(GLdouble **vals){
    float deltaY = points[1].y - points[0].y;
    float deltaX = points[1].x - points[0].x;
    if (deltaX == 0) {
        *vals[0] = points[0].x-1;
        *vals[1] = points[0].y;
        *vals[2] = points[0].x+1;
        *vals[3] = points[0].y;
        *vals[4] = points[numOfPoints-1].x-1;
        *vals[5] = points[numOfPoints-1].y;
        *vals[6] = points[numOfPoints-1].x+1;
        *vals[7] = points[numOfPoints-1].y;
        return;
    }
    float m = deltaY / deltaX;
    float inverse_m = - (1 / m);
    float ang = fabs(atanf(inverse_m));
    float B = 2 * sinf(ang);
    float C = 2 * cosf(ang);
    (*vals)[0] = points[0].x - (C / 2);
    (*vals)[1] = points[0].y + (B / 2);
    (*vals)[2] = points[0].x + (C / 2);
    (*vals)[3] = points[0].y - (B / 2);
    (*vals)[4] = points[numOfPoints-1].x - (C / 2);
    (*vals)[5] = points[numOfPoints-1].y + (B / 2);
    (*vals)[6] = points[numOfPoints-1].x + (C / 2);
    (*vals)[7] = points[numOfPoints-1].y - (B / 2);
}

void Bezier::flatten(){
    float deltaX = points[numOfPoints - 1].x - points[0].x;
    float deltaY = points[numOfPoints - 1].y - points[0].y;
    /*
     * If the two outer control points are one atop of the other, then there's no gradient between them, and points
     * should be repositioned in a specific manner. Otherwise, calculate the linear function between the two points,
     * and place all the inner points in equal intervals on that function.
     */
    if (deltaX == 0) {
        deltaY = (points[numOfPoints - 1].y - points[0].y) / (numOfPoints - 1);
        float x = points[0].x;
        float y = points[0].y + deltaY;
        for (int i = 1; i < numOfPoints - 1; i++, y += deltaY) {
            points[i] = Vector3f(x, y, 0);
        }
    }
    else {
        float m = deltaY / deltaX;
        float n = points[0].y - m * points[0].x;
        deltaX = (points[numOfPoints - 1].x - points[0].x) / (numOfPoints - 1);
        float x = points[0].x + deltaX;
        for (int i = 1; i < numOfPoints - 1; i++, x += deltaX) {
            points[i] = Vector3f(x, m * x + n, 0);
        }
    }
}