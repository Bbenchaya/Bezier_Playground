//
//  ControlPoint.cpp
//  CG_ex5
//
//  Created by Asaf Chelouche on 12/1/16.
//  Copyright © 2016 Bbenchaya. All rights reserved.
//

#include "ControlPoint.hpp"

ControlPoint::ControlPoint(int index, Bezier *curve){
    this->index = index;
    this->curve = curve;
}

void ControlPoint::translate(GLdouble x, GLdouble y){
    curve->moveControlPoint(index, x, y);
}

void ControlPoint::adjustIndex(int delta){
    this->index += delta;
}

Bezier* ControlPoint::getCurve(){
    return curve;
}