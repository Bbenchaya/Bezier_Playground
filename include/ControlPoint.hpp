//
//  ControlPoint.hpp
//  CG_ex5
//
//  Created by Asaf Chelouche on 12/1/16.
//  Copyright © 2016 Bbenchaya. All rights reserved.
//

#ifndef ControlPoint_h
#define ControlPoint_h

#include "Bezier.hpp"

class ControlPoint {

private:
    int index;
    Bezier *curve;
    
public:
    ControlPoint(int index, Bezier *curve);
    void translate(int x, int y);
    void adjustIndex(int delta);
    Bezier* getCurve();
    
};

#endif /* ControlPoint_h */
