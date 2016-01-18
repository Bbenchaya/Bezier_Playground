//
//  bezier.cpp
//  CG_ex5
//
//  Created by Ben Benchaya on 06/01/16.
//  Copyright © 2016 Bbenchaya. All rights reserved.
//

#include "Bezier.hpp"
#include "ControlPoint.hpp"

#include <limits>

#define ESC 27

#define Z_NEAR 200.0
#define Z_FAR 2.0
#define RESOLUTION 50
#define POINT_RADIUS 1
#define INITIAL_CURVE_LENGTH 15.0
#define MIN_NUM_OF_CURVES 2
#define MAX_NUM_OF_CURVES 6
#define DEFAULT_NUM_OF_CURVES 3
#define CONTROL_POINTS_PICKING 0
#define CONVEX_HULLS_PICKING 1

vector<Bezier*> curves;
vector<ControlPoint*> controlPoints;
vector<int> pickedControlPoints;
vector<int> pickedConvexHulls;
int old_x;
int old_y;
int numOfControlPointsPerCurve;
bool left_button_pressed;
bool middle_button_pressed;
bool right_button_pressed;
bool design_mode;
GLfloat window_width = 512;
GLfloat window_height = 512;
GLfloat camAngle;
GLint hits;
GLfloat zValuesForMouseMotion[4];
GLfloat rotationMatrix[16] = {1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1};
GLfloat translationMatrix[16] = {1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1};

void printModelviewMatrix(){
    float modelviewMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    printf("Modelview matrix: \n");
    for(int i = 0; i < 4;i++){
        for(int j = 0; j < 4; j++)
            printf("%f ",modelviewMatrix[j * 4 + i]);
        printf("\n");
    }
}

void printProjectionMatrix(){
    float projectionMatrix[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
    printf("Projection matrix: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            printf("%f ", projectionMatrix[j * 4 + i]);
        printf("\n");
    }
}

void drawAxes(){
    glLineWidth(1.5);
    glBegin(GL_LINES);
    float red[4] = {1, 0, 0, 1};
    float green[4] = {0, 1, 0, 1};
    float blue[4] = {0, 0, 1, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
    glVertex3fv(Vector3f(50, 0, 0));  glVertex3fv(Vector3f(-50, 0, 0));
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
    glVertex3fv(Vector3f(0, 50, 0));  glVertex3fv(Vector3f(0,-50, 0));
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
    glVertex3fv(Vector3f(0, 0, 50));  glVertex3fv(Vector3f(0, 0, -50));
    glEnd();
}

void drawCurves(){
    glBegin(GL_LINE_STRIP);
    for (vector<Bezier*>::iterator curve = curves.begin(); curve != curves.end(); curve++) {
        for (float i = 0; i <= RESOLUTION; i++) {
            Vector3f point = (*curve)->getTinXYZ(i / RESOLUTION);
            glVertex3f(point.x, point.y, point.z);
        }
    }
    glEnd();
}

void drawControlPoints(int mode){
    /*
     * Each curve draws its points in ascending order, except for the last point. This is so the last point of a curve, which
     * overlaps the first point of the next curve, wouldn't be rendered atop of each other.
     * Only the last curve draws its last point, which always rests on the X axis.
     * The first and last control points of a curve are blue, and the remaining inner control points are gray.
     */
    GLfloat blue[4] = {0, 0, 1, 1};
    GLfloat gray[4] = {0.5, 0.5, 0.5, 1};
    switch (mode) {
        case GL_RENDER:
            for (vector<Bezier*>::iterator curve = curves.begin(); curve != curves.end(); curve++) {
                Vector3f point = (*curve)->getPoint(0);
                glPushMatrix();
                glTranslatef(point.x, point.y, point.z);
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
                glutSolidSphere(POINT_RADIUS, 20, 20);
                glPopMatrix();
                for (int i = 1; i < (*curve)->getNumOfPoints() - 1; i++) {
                    point = (*curve)->getPoint(i);
                    glPushMatrix();
                    glTranslatef(point.x, point.y, point.z);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gray);
                    glutSolidSphere(POINT_RADIUS, 20, 20);
                    glPopMatrix();
                }
                if ((*curve)->isRightmost()) {
                    Vector3f point = (*curve)->getPoint((*curve)->getNumOfPoints() - 1);
                    glPushMatrix();
                    glTranslatef(point.x, point.y, point.z);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
                    glutSolidSphere(POINT_RADIUS, 20, 20);
                    glPopMatrix();
                }
            }
            break;
        case GL_SELECT:
            int curveNum = 0;
            for (vector<Bezier*>::iterator curve = curves.begin(); curve != curves.end(); curve++, curveNum++) {
                Vector3f point = (*curve)->getPoint(0);
                for (int i = 0; i < (*curve)->getNumOfPoints(); i++) {
                    point = (*curve)->getPoint(i);
                    glLoadName(i + curveNum * numOfControlPointsPerCurve);
                    glPushMatrix();
                    glTranslatef(point.x, point.y, point.z);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gray);
                    glutSolidSphere(POINT_RADIUS, 20, 20);
                    glPopMatrix();
                }
            }
    }
}

void drawConvexHulls(){
    int curveIndex = 0;
    for (vector<Bezier*>::iterator curve = curves.begin(); curve != curves.end(); curve++, curveIndex++) {
        float xMin, xMax, yMin, yMax;
        xMin = yMin = INFINITY;
        xMax = yMax = -INFINITY;
        for (int i = 0; i < numOfControlPointsPerCurve; i++) {
            Vector3f point = (*curve)->getPoint(i);
            if (point.x < xMin) {
                xMin = point.x;
            }
            if (point.x > xMax) {
                xMax = point.x;
            }
            if (point.y < yMin) {
                yMin = point.y;
            }
            if (point.y > yMax) {
                yMax = point.y;
            }
        }
        GLdouble *linearCurve = new GLdouble[8];
        if ((*curve)->isLinearCurve()) {
            (*curve)->getLinearCurveConvexHull(&linearCurve);
            glLoadName(curveIndex);
            glBegin(GL_QUADS);
            glVertex3f(linearCurve[0], linearCurve[1], 0);
            glVertex3f(linearCurve[2], linearCurve[3], 0);
            glVertex3f(linearCurve[4], linearCurve[5], 0);
            glVertex3f(linearCurve[6], linearCurve[7], 0);
            glEnd();
            continue;
        }
        glLoadName(curveIndex);
        glBegin(GL_QUADS);
        glVertex3f(xMin, yMin, 0);
        glVertex3f(xMin, yMax, 0);
        glVertex3f(xMax, yMax, 0);
        glVertex3f(xMax, yMin, 0);
        glEnd();
    }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawAxes();
    drawCurves();
    drawControlPoints(GL_RENDER);
    glFlush();
}

/*
 * The following 2 functions were provided as mandatory implemenatition, and they don't
 * reflect our coding style or thinking process.
 */

/////////////////////////////
//surface: 2D NURBS
//degree is the degree of each polynom in u
//degree2 is the degree of each polynom in v
//subNum is number of sub curve, how many parts in the whole curve
//////////////////////////////
void drawSurface(vector <Vector3f> &surface,int degree, int degree2, int subNum){
    GLfloat curve[degree2][degree][3];
    
    for (int o = 0; o < subNum; o++)
    {
        
        for (int n = 0; n < 4; n++) // ring
        {
            
            for (int m = 0; m < degree2; m++) // square
            {
                
                for (int k = 0; k < degree; k++) //p1, p2, p3...
                {
                    for (int j = 0; j < 3; j++) //X,Y,Z
                        curve[m][k][j] = surface[k + m*degree + n*degree2*degree + degree2 * 4 * degree*o][j];
                }
            }
            
            glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, degree, 0, 1, 3 * degree, degree2, &curve[0][0][0]);
            //for (int j = 0; j <= 8; j++) {
            //glBegin(GL_LINE_STRIP);
            //for (int i = 0; i <= 30; i++)
            //	glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)j / 8.0);
            //glEnd();
            //glBegin(GL_LINE_STRIP);
            //for (int i = 0; i <= 30; i++)
            //	glEvalCoord2f((GLfloat)j / 8.0, (GLfloat)i / 30.0);
            //glEnd();
            //}
            
            for (int j = 0; j < 8; j++) {
                
                glBegin(GL_QUAD_STRIP);
                for (int i = 0; i <= 30; i++){
                    glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)j / 8.0);
                    glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)(j + 1) / 8.0);
                }
                glEnd();
                
            }
        }
    }
}

/////////////////////////////
//contour: vector contains your 1D curve. The coordinates of Pn and P0 of consecutive sub curves
//appear just once in the vector
//contSurface, function output, contains the 2D surface calculate from the 1D curve
//degree is the degree of the each polynom in t
//degree2 is the degree of polynom in another dymention
//subNum is number of sub curve, how many parts in the whole curve
//////////////////////////////
void calcSurface(vector <Vector3f> &contour, vector <Vector3f> &contSurface, int degree, int degree2,int subNum){
    //    for (int i = 0; i < contour.size(); i++)
    //        printf("y(%d) = %f, ", i, contour[i].y);
    //    printf("\n");
    
    contSurface.clear();
    float r, R;
    float const alpha = M_PI / 2.0 / (float)(degree2 - 1);
    float const rathio = 1.0 / cos(alpha);
    for (int k = 0; k < subNum; k++)
    {
        for (int m = 0; m < 4; m++) //full ring
        {
            
            for (int i = 1, j = 0; j < degree2; i += degree - 1, j++) // square
            {
                
                for (int l = 0; l < degree; l++) // part of ring
                {
                    Vector3f p1, n1;
                    
                    if (l == 0 || l == degree - 1)
                    {
                        r = contour[(degree - 1) * k + l].y;
                        if (j == 0 || j == degree2 - 1)
                            p1 = Vector3f(contour[(degree - 1) * k + l].x, r*sin((float)((degree2 - 1) * m + j)*alpha), r*cos((float)((degree2 - 1) * m + j)*alpha));
                        else
                        {
                            R = r*rathio;
                            p1 = Vector3f(contour[(degree - 1) * k + l].x, R*sin((float)((degree2 - 1) * m + j)*alpha), R*cos((float)((degree2 - 1) * m + j)*alpha));
                        }
                        
                    }
                    else
                    {
                        if (j >0 && j < degree2 - 1)
                            R = contour[(degree - 1) * k + l].y*rathio;
                        else R = contour[(degree - 1) * k + l].y;
                        
                        p1 = Vector3f(contour[(degree - 1) * k + l].x, R*sin((float)((degree2 - 1) * m + j)*alpha), R*cos((float)((degree2 - 1) * m + j)*alpha));
                    }
                    contSurface.push_back(p1);
                    
                }
            }
        }
    }
}

void displaySurface(){
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glMultMatrixf(rotationMatrix);
    glMultMatrixf(translationMatrix);
    drawAxes();
    vector<Vector3f> contour;
    vector<Vector3f> surface;
    /*
     * contour contains the 1D curve. For each curves, all of its control point, excluding the last,
     * will be inserted to the vector. Only the rightmost curve will have its last control point
     * inserted to te vector.
     */
    contour.push_back(curves[curves.size() - 1]->getPoint(numOfControlPointsPerCurve - 1));
    for (vector<Bezier*>::reverse_iterator curve = curves.rbegin(); curve != curves.rend(); curve++) {
        for (int i = numOfControlPointsPerCurve - 2; i >= 0; i--) {
            contour.push_back((*curve)->getPoint(i));
        }
    }
    calcSurface(contour, surface, numOfControlPointsPerCurve, numOfControlPointsPerCurve, curves.size());
    drawSurface(surface, numOfControlPointsPerCurve, numOfControlPointsPerCurve, curves.size());
    glFlush();
    glPopMatrix();
}

void listHits(GLint hits, GLuint *names) {
    int i;
    printf("%d hits:\n", hits);
    for (i = 0; i < hits; i++)
        printf("Number: %d\n"
               "Min Z: %d\n"
               "Max Z: %d\n"
               "Name on stack: %d\n",
               (GLubyte)names[i * 4],
               (GLubyte)names[i * 4 + 1],
               (GLubyte)names[i * 4 + 2],
               (GLubyte)names[i * 4 + 3]
               );
    printf("\n");
}

void processPicks(GLint hits, GLuint *names, int mode) {
    switch (mode) {
        case CONTROL_POINTS_PICKING:
            for (int i = 0; i < hits; i++) {
                pickedControlPoints.push_back((GLubyte)names[i * 4 + 3]);
            }
            break;
        case CONVEX_HULLS_PICKING:
            for (int i = 0; i < hits; i++) {
                pickedConvexHulls.push_back((GLubyte)names[i * 4 + 3]);
            }
    }
    
}

void pickObjects(int x, int y, int mode){
    GLuint *buff;
    if (mode == CONTROL_POINTS_PICKING) {
        buff = new GLuint[controlPoints.size()];
    }
    else
        buff = new GLuint[curves.size()];
    GLint hits, viewport[4];
    glReadPixels((GLdouble) x, (GLdouble) viewport[3] - y, 2, 2, GL_DEPTH_COMPONENT, GL_FLOAT, zValuesForMouseMotion);
    glSelectBuffer(64, buff);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    /*
     * The window's top-left corner is (0,0), but in the picking matrix (0,0) is the bottom-left,
     * so we have to invert the Y values.
     */
    gluPickMatrix(x, window_height - y, 1, 1, viewport);
    gluPerspective(camAngle, window_width / window_height, Z_NEAR, Z_FAR);
    glMatrixMode(GL_MODELVIEW);
    if (mode == CONTROL_POINTS_PICKING) {
        drawControlPoints(GL_SELECT);
    }
    else if (mode == CONVEX_HULLS_PICKING)
        drawConvexHulls();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    hits = glRenderMode(GL_RENDER);
    processPicks(hits, buff, mode);
    //    listHits(hits, buff);
    display();
}

void mouseClick(int button, int state, int x, int y){
    switch (button) {
        case GLUT_LEFT_BUTTON:
            left_button_pressed = !left_button_pressed;
            if (!design_mode)
                return;
            if (left_button_pressed) {
                pickObjects(x, y, CONTROL_POINTS_PICKING);
                pickObjects(x, y, CONVEX_HULLS_PICKING);
            }
            else {
                // if control points were picked, than a curve should be removed or two curves should be made continuous
                if (pickedControlPoints.size() > 0) {
                    // section 4b,c
                    for (vector<int>::iterator pickedPoint = pickedControlPoints.begin(); pickedPoint != pickedControlPoints.end(); pickedPoint++) {
                        /*
                         * The picked point has index 0 in the curve - subsection b
                         */
                        if (*pickedPoint % numOfControlPointsPerCurve == 0 && curves.size() > MIN_NUM_OF_CURVES) {
                            Bezier *curve = controlPoints[*pickedPoint]->getCurve();
                            if (curve->isLeftmost()) {
                                curves[1]->clamp(LEFTMOST, 0);
                                curves[1]->setPreviousCurve(NULL);
                                curves.erase(curves.begin());
                                controlPoints.erase(controlPoints.begin(), controlPoints.begin() + numOfControlPointsPerCurve);
                            }
                            else if (curve->isRightmost()) {
                                Bezier *last = curves.back();
                                float newX = last->getPoint(numOfControlPointsPerCurve - 1).x;
                                curves.pop_back();
                                curves.back()->clamp(RIGHTMOST, newX);
                                curves.back()->setNextCurve(NULL);
                                for (int i = 0; i < numOfControlPointsPerCurve - 1; i++) {
                                    controlPoints.pop_back();
                                }
                            }
                            else {
                                int index = *pickedPoint / numOfControlPointsPerCurve;
                                Bezier *toRemove = curves[index];
                                Vector3f pn = toRemove->getPoint(numOfControlPointsPerCurve - 1);
                                curves[index - 1]->setPoint(numOfControlPointsPerCurve - 1, pn);
                                curves.erase(curves.begin() + index);
                                controlPoints.erase(controlPoints.begin() + index * numOfControlPointsPerCurve, controlPoints.begin() + (index + 1) * numOfControlPointsPerCurve);
                            }
                        }
                        /*
                         * The picked point has index 1 in the curve - subsection c
                         */
                        if (*pickedPoint % numOfControlPointsPerCurve == 1 && curves.size() < MAX_NUM_OF_CURVES) {
                            Bezier *curve = controlPoints[*pickedPoint]->getCurve();
                            if (curve->isLeftmost()) {
                                continue;
                            }
                            Bezier *previous = curve->getPreviousCurve();
                            pair<float, float> linearFunc = curve->getP0P1LinearFunction();
                            if (linearFunc.first == 0 && linearFunc.second == 0) {
                                continue;
                            }
                            previous->adjustPnMinus1(linearFunc);
                        }
                    }
                    
                }
                // no control points were chosen, so if a covnex hull was chosen, it should linearize the curve
                else if (pickedConvexHulls.size() > 0) {
                    Bezier *curve = curves[pickedConvexHulls[0]];
                    curve->flatten();
                }
                display();
                pickedControlPoints.clear();
                pickedConvexHulls.clear();
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            middle_button_pressed = !middle_button_pressed;
            if (!design_mode)
                return;
            if (middle_button_pressed) {
                pickObjects(x, y, CONTROL_POINTS_PICKING);
                pickObjects(x, y, CONVEX_HULLS_PICKING);
            }
            else {
                pickedControlPoints.clear();
                pickedConvexHulls.clear();
            }
            break;
        case GLUT_RIGHT_BUTTON:
            right_button_pressed = !right_button_pressed;
            if (!design_mode)
                return;
            if (right_button_pressed) {
                pickObjects(x, y, CONTROL_POINTS_PICKING);
                pickObjects(x, y, CONVEX_HULLS_PICKING);
            }
            else {
                // section 4e
                if (pickedControlPoints.size() == 0 && pickedConvexHulls.size() > 0 && curves.size() < MAX_NUM_OF_CURVES) {
                    int index = pickedConvexHulls[0];
                    Bezier **newCurves = curves[index]->split();
                    curves.erase(curves.begin() + index);
                    vector<Bezier*>::iterator insertion = curves.begin() + index;
                    insertion = curves.insert(insertion, newCurves[1]);
                    insertion = curves.insert(insertion, newCurves[0]);
                    controlPoints.clear();
                    int curveNum = 0;
                    for (vector<Bezier*>::iterator curve = curves.begin(); curve != curves.end(); curve++, curveNum++) {
                        for (int i = 0; i < numOfControlPointsPerCurve; i++) {
                            ControlPoint* cp = new ControlPoint(i, *curve);
                            controlPoints.push_back(cp);
                        }
                    }
                    display();
                }
                pickedControlPoints.clear();
                pickedConvexHulls.clear();
            }
    }
}

void movePoints(int x, int y){
    GLdouble newX, newY, newZ;
    GLdouble modelViewMatrix[16];
    GLdouble projectionMatrix[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluUnProject((GLdouble) x, window_height - (GLdouble) y, 0.0, modelViewMatrix, projectionMatrix, viewport, &newX, &newY, &newZ);
    for (vector<int>::iterator pickedPoint = pickedControlPoints.begin(); pickedPoint != pickedControlPoints.end(); pickedPoint++) {
        controlPoints[*pickedPoint]->translate(newX, newY);
    }
    old_x = newX;
    old_y = newY;
    display();
}

void moveCurve(int x, int y){
    int deltaX = old_x - x;
    int deltaY = old_y - y;
    if (abs(deltaX) > 3){
        old_x = x;
    }
    if (abs(deltaY) > 3){
        old_y = y;
    }
    Bezier *curve = curves[pickedConvexHulls[0]];
    if (abs(deltaX) > 3 && abs(deltaY) > 3) {
        curve->moveAllPoints(-0.1 * deltaX, 0.1 * deltaY);
    }
    display();
}

void mouseMotion(int x, int y){
    if (left_button_pressed) {
        if (!design_mode) {
            glPushMatrix();
            glLoadIdentity();
            if (abs(old_x - x) > 3) {
                glRotatef((old_x - x > 0 ? -1 : 1), rotationMatrix[4], rotationMatrix[5], rotationMatrix[6]);
                glMultMatrixf(rotationMatrix);
                glGetFloatv(GL_MODELVIEW_MATRIX, rotationMatrix);
                old_x = x;
            }
            glLoadIdentity();
            if (abs(old_y - y) > 3) {
                glRotatef((old_y - y > 0 ? 1 : -1), rotationMatrix[8], rotationMatrix[9], rotationMatrix[10]);
                glMultMatrixf(rotationMatrix);
                glGetFloatv(GL_MODELVIEW_MATRIX, rotationMatrix);
                old_y = y;
            }
            glPopMatrix();
            displaySurface();
        }
    }
    else if (middle_button_pressed){
        if (design_mode && pickedControlPoints.size() > 0) {
            // section 4a
            movePoints(x, y);
        }
        else if (design_mode && pickedConvexHulls.size() > 0){
            // section 4d
            moveCurve(x, y);
        }
        else if (!design_mode){
            glPushMatrix();
            glLoadIdentity();
            if (abs(old_x - x) > 3 && abs(old_y - y) > 3) {
                glTranslatef(-0.1*(old_x - x), 0.1*(old_y - y), 0);
                glMultMatrixf(translationMatrix);
                glGetFloatv(GL_MODELVIEW_MATRIX, translationMatrix);
                glPopMatrix();
                old_x = x;
                old_y = y;
            }
            glPopMatrix();
            displaySurface();
        }
    }
    else if (right_button_pressed){
        if (design_mode && pickedControlPoints.size() > 0) {
            // section 4a
            movePoints(x, y);
        }
        else{
            // no control points or convex hulls were selected, so zoom in\out
            if (design_mode) {
                if (old_y - y > 3){
                    camAngle = fmax(15, camAngle - 0.5);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    gluPerspective(camAngle, window_width / window_height, Z_NEAR, Z_FAR);
                    glMatrixMode(GL_MODELVIEW);
                    display();
                    old_y = y;
                }
                else if (old_y - y < -3){
                    camAngle = fmin(140, camAngle + 0.5);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    gluPerspective(camAngle, window_width / window_height, Z_NEAR, Z_FAR);
                    glMatrixMode(GL_MODELVIEW);
                    display();
                    old_y = y;
                }
            }
            // the system is not in design mode, so translate the Bezier surface around the scene
            else {
                glPushMatrix();
                glLoadIdentity();
                if (abs(old_x - x) > 3 && abs(old_y - y) > 3) {
                    glTranslatef(-0.1*(old_x - x), 0.1*(old_y - y), 0);
                    glMultMatrixf(translationMatrix);
                    glGetFloatv(GL_MODELVIEW_MATRIX, translationMatrix);
                    glPopMatrix();
                    old_x = x;
                    old_y = y;
                }
                glPopMatrix();
                displaySurface();
            }
        }
        
    }
}

void generateCurves(){
    Vector3f *v1 = new Vector3f[numOfControlPointsPerCurve];
    Vector3f *v2 = new Vector3f[numOfControlPointsPerCurve];
    Vector3f *v3 = new Vector3f[numOfControlPointsPerCurve];
    float num = (float) (numOfControlPointsPerCurve - 1);
    float step = INITIAL_CURVE_LENGTH / num;
    for (int i = 0; i < numOfControlPointsPerCurve; i++) {
        float iFloat = (float) i;
        v1[i] = Vector3f(iFloat * step, iFloat * step, 0);
        v2[i] = Vector3f(INITIAL_CURVE_LENGTH + iFloat * step, num * step, 0);
        v3[i] = Vector3f(2 * INITIAL_CURVE_LENGTH + iFloat * step, (num - iFloat) * step, 0);
    }
    Bezier *b1 = new Bezier(numOfControlPointsPerCurve, v1);
    Bezier *b2 = new Bezier(numOfControlPointsPerCurve, v2);
    Bezier *b3 = new Bezier(numOfControlPointsPerCurve, v3);
    /*
     * After the curves are created, construct the vector of control points. Each control point holds a reference to its respective
     * curve and the index of the control point in that curve, which relates to this specific control point.
     * This enables easy picking of control points when trying to move them around the screen with the mouse.
     */
    for (int i = 0; i < numOfControlPointsPerCurve; i++) {
        ControlPoint *point = new ControlPoint(i, b1);
        controlPoints.push_back(point);
    }
    for (int i = 0; i < numOfControlPointsPerCurve; i++) {
        ControlPoint *point = new ControlPoint(i, b2);
        controlPoints.push_back(point);
    }
    for (int i = 0; i < numOfControlPointsPerCurve; i++) {
        ControlPoint *point = new ControlPoint(i, b3);
        controlPoints.push_back(point);
    }
    b1->setExtremum(LEFTMOST);
    b3->setExtremum(RIGHTMOST);
    b1->setNextCurve(b2);
    b2->setPreviousCurve(b1);
    b2->setNextCurve(b3);
    b3->setPreviousCurve(b2);
    curves.push_back(b1);
    curves.push_back(b2);
    curves.push_back(b3);
}

void initLights(){
    GLfloat light_ambient[] = {0.5, 0.5, 0.5};
    GLfloat light_diffuse[] = {0, 0.5, 0.5};
    GLfloat light_specular[] = {0, 0.0, 0.5};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void init(){
    camAngle = 60.0;
    left_button_pressed = false;
    middle_button_pressed = false;
    right_button_pressed = false;
    design_mode = true;
    old_x = window_width / 2;
    old_y = window_height / 2;
    numOfControlPointsPerCurve = 4;
    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camAngle, window_width / window_height, Z_NEAR, Z_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -100);
    glLineWidth(4);
    glClearColor(0, 0, 0, 1);
    glEnable(GL_MAP2_VERTEX_3);
    //    glEnable(GL_MAP1_VERTEX_3);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //    GLfloat light_direction[]={0,-1,0};
    GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1};
    GLfloat light_diffuse[] = {0, 1, 0.5, 1};
    GLfloat light_specular[] = {0, 0, 0.5, 1};
    GLfloat light_position[] = {0, 0, 1, 0};
    //    GLfloat angle[] = {20.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    GLfloat mat_a[] = {0.3, 0.4, 0.5, 1.0};
    GLfloat mat_d[] = {0.0, 0.6, 0.7, 1.0};
    GLfloat mat_s[] = {0.0, 0.0, 0.8, 1.0};
    GLfloat low_sh[] = {5.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
    //    initLights();
    generateCurves();
}

void recalibrateControlPoints(){
    controlPoints.clear();
    for (vector<Bezier*>::iterator curve = curves.begin(); curve != curves.end(); curve++) {
        for (int i = 0; i < numOfControlPointsPerCurve; i++) {
            ControlPoint *point = new ControlPoint(i, *curve);
            controlPoints.push_back(point);
        }
    }
}

void clearVectors(){
    controlPoints.clear();
    curves.clear();
    pickedControlPoints.clear();
    pickedConvexHulls.clear();
}

void readKey(unsigned char key, int x, int y){
    if ('0' <= key && key <= '9') {
        cout << "Design mode. Number of inner control points per curve: " << key << endl;
        design_mode = true;
        init();
        numOfControlPointsPerCurve = key - 48 + 2; // map key's ASCII value to its symbolic value, add the two outer points
        clearVectors();
        generateCurves();
        display();
        glLoadIdentity();
        glGetFloatv(GL_MODELVIEW_MATRIX, rotationMatrix);
        glGetFloatv(GL_MODELVIEW_MATRIX, translationMatrix);
        glTranslatef(0, 0, -100);
        return;
    }
    if (key == 'd') {
        design_mode = false;
        cout << "3D object mode" << endl;
        glPushMatrix();
        displaySurface();
        glPopMatrix();
        return;
    }
    if (key == 'r' || key == 'R') {
        cout << "RESETTING SCENE" << endl;
        clearVectors();
        init();
        glLoadIdentity();
        glGetFloatv(GL_MODELVIEW_MATRIX, rotationMatrix);
        glGetFloatv(GL_MODELVIEW_MATRIX, translationMatrix);
        glTranslatef(0, 0, -100);
        display();
    }
    if (key == ESC) {
        exit(0);
    }
}

void reshape(int width, int height){
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    window_width = width;
    window_height = height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camAngle, window_width / window_height, Z_FAR, Z_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -100);
    display();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Bezier's playground");
    init();
    glutKeyboardFunc(readKey);
    glutDisplayFunc(display);
//    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseClick);
    glutMainLoop();
    return 0;
    
}