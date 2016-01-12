//
//  bezier.cpp
//  CG_ex5
//
//  Created by Ben Benchaya on 06/01/16.
//  Copyright © 2016 Bbenchaya. All rights reserved.
//

#include "Bezier.hpp"
#include "ControlPoint.hpp"

#define ESC 27

#define Z_NEAR 200.0
#define Z_FAR 2.0
#define bufSize 128
#define maxSize 512 //what is maxsize?
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define ROTATION_DEGREE 1
#define RESOLUTION 50
#define POINT_RADIUS 1
#define INITIAL_CURVE_LENGTH 15.0

using namespace std;

vector<Bezier*> curves;
vector<ControlPoint*> controlPoints;
vector<int> pickedControlPoints;
int old_x;
int old_y;
int dotIndex = -1; // what is this?
int numOfControlPointsPerCurve;
int numOfCurves;
bool left_button_pressed;
bool middle_button_pressed;
bool right_button_pressed;
bool design_mode;
GLfloat camAngle;
GLint hits;

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


//void changePointPosition(vector <Vector3f> &control, Vector3f diff){
//
//}

//void drawShape(){
//    float red[4] = {1, 0, 0, 1};
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
//    glutSolidSphere(POINT_SIZE, 50, 50);
//}

//void drawCurve(vector <Vector3f> &curve, GLenum mode, int degree, int subNum){
//    for (int j = 1; j <= control_points_num; j++) {
//        drawShape();
//        curve.push_back(Vector3f(j,j-1,0));
//    }
//    glBegin(GL_LINE_STRIP);
//    for (int i = 0; i < curve.size(); i++)
//        glVertex3f(curve.at(i).x, curve.at(i).y, curve.at(i).z);
//    glEnd();
//}

/////////////////////////////
//surface: 2D NURBS
//degree is the degree of each polynom in u
//degree2 is the degree of each polynom in v
//subNum is number of sub curve, how many parts in the whole curve
//////////////////////////////
//void drawSurface(vector <Vector3f> &surface,int degree, int degree2, int subNum){
//    GLfloat curve[degree2][degree][3];
//
//    for (int o = 0; o < subNum; o++)
//    {
//
//        for (int n = 0; n < 4; n++) // ring
//        {
//
//            for (int m = 0; m < degree2; m++) // square
//            {
//
//                for (int k = 0; k < degree; k++) //p1, p2, p3...
//                {
//                    for (int j = 0; j < 3; j++) //X,Y,Z
//                        curve[m][k][j] = surface[k + m*degree + n*degree2*degree + degree2 * 4 * degree*o][j];
//                }
//            }
//
//            glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, degree, 0, 1, 3 * degree, degree2, &curve[0][0][0]);
//            //for (int j = 0; j <= 8; j++) {
//            //glBegin(GL_LINE_STRIP);
//            //for (int i = 0; i <= 30; i++)
//            //	glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)j / 8.0);
//            //glEnd();
//            //glBegin(GL_LINE_STRIP);
//            //for (int i = 0; i <= 30; i++)
//            //	glEvalCoord2f((GLfloat)j / 8.0, (GLfloat)i / 30.0);
//            //glEnd();
//            //}
//
//            for (int j = 0; j < 8; j++) {
//
//                glBegin(GL_QUAD_STRIP);
//                for (int i = 0; i <= 30; i++){
//                    glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)j / 8.0);
//                    glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)(j + 1) / 8.0);
//                }
//                glEnd();
//
//            }
//        }
//    }
//}

//void splitCurve(vector <Vector3f> &curve, int curveIndx, int degree, int &subNum){
//
//}

//void deleteCurve(vector <Vector3f> &curve, int curveIndx, int degree, int &subNum){}

//void makeStraight(vector <Vector3f> &curve, int curveIndx, int degree){
//}

//void derevativeContinuity(vector <Vector3f> &curve, int vertexIndx, int degree){
//}

//void startPicking(GLuint *selectionBuf){
//    GLint hits;
//    glSelectBuffer(maxSize * 12, selectionBuf); //declare buffer for input in selection mode
//    glRenderMode(GL_SELECT); //change to selecting mode
//    glInitNames();			//initialize names stack
//    glPushName(-1);			//push name
//    //======================
//    glMatrixMode(GL_PROJECTION);
//    glPushMatrix();
//    glLoadIdentity();
//    //    gluPickMatrix(x, y, 1.0, 1.0, view);
//    //    gluPerspective(FOV, 1.0, Z_NEAR, Z_FAR);
//    glMatrixMode(GL_MODELVIEW);
//    //    drawObjects(GL_SELECT);
//    glMatrixMode(GL_PROJECTION);
//    glPopMatrix();
//    hits = glRenderMode(GL_RENDER);
//    //    processPicks(hits, selectionBuf);
//    //    display();
//
//}

//void processHits(GLint hits, GLuint *buffer){
//}

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

bool inConvexHull(){
    return false;
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawAxes();
    //    drawCurve(curve, GL_RENDER, ROTATION_DEGREE, control_points_num);
    drawCurves();
    drawControlPoints(GL_RENDER);
    glFlush();
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

void processPicks(GLint hits, GLuint *names) {
    for (int i = 0; i < hits; i++) {
        pickedControlPoints.push_back((GLubyte)names[i * 4 + 3]);
    }
}

void pickObjects(int x, int y){
    GLuint *buff = new GLuint[controlPoints.size()];;
    GLint hits, view[4];
    glSelectBuffer(64, buff);
    glGetIntegerv(GL_VIEWPORT, view);
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
    gluPickMatrix(x, WINDOW_HEIGHT - y, 1.0, 1.0, view);
    gluPerspective(camAngle, WINDOW_WIDTH / WINDOW_HEIGHT, Z_NEAR, Z_FAR);
    glMatrixMode(GL_MODELVIEW);
    drawControlPoints(GL_SELECT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    hits = glRenderMode(GL_RENDER);
    processPicks(hits, buff);
    listHits(hits, buff);
    display();
}

void mouseClick(int button, int state, int x, int y){
    //    GLint viewport[4];
    //    GLuint selectionBuf[bufSize];
    //    glGetIntegerv(GL_VIEWPORT,viewport); //reading viewport parameters
    //    //    press=!press;
    //    //		  if(press){
    //    //              glMatrixMode(GL_PROJECTION);
    //    //              glPushMatrix();	//saves current projection matrix
    //    //              glLoadIdentity();
    //    //              startPicking(selectionBuf); //preper selection mode
    //    //              gluPickMatrix((GLdouble) x,(GLdouble) viewport[3]-y,1,1,viewport);
    //    //              gluPerspective(camAngle,1,near,far);
    //    //              glMatrixMode(GL_MODELVIEW);
    //    ////              drawCurve(<#vector<Vector3f> &curve#>, GL_SELECT, <#int degree#>, <#int subNum#>);
    //    //              hits=glRenderMode(GL_RENDER); //gets hits number
    //    //              glMatrixMode(GL_PROJECTION);
    //    //              glPopMatrix(); //restores projection matrix
    //    //              glMatrixMode(GL_MODELVIEW);
    //    //              processHits(hits,selectionBuf); //check hits
    //    //          }
    //    //
    switch (button) {
        case GLUT_LEFT_BUTTON:
            left_button_pressed = !left_button_pressed;
            break;
        case GLUT_MIDDLE_BUTTON:
            middle_button_pressed = !middle_button_pressed;
            break;
        case GLUT_RIGHT_BUTTON:
            right_button_pressed = !right_button_pressed;
            if (right_button_pressed) {
                pickObjects(x, y);
            }
            else
                pickedControlPoints.clear();
    }
    
}

void mouseMotion(int x, int y){
    if (left_button_pressed) {
        if (old_x - x > 0){
            glRotatef(-ROTATION_DEGREE, 0, 1, 1);
            display();
            old_x = x;
        }
        else if (old_x - x < 0){
            glRotatef(ROTATION_DEGREE, 0, 1, 1);
            display();
            old_x = x;
        }
        else if (old_y - y > 0){
            glRotatef(-ROTATION_DEGREE, 1, 0, 1);
            display();
            old_y = y;
        }
        else if (old_y - y < 0){
            glRotatef(ROTATION_DEGREE, 1, 0, 1);
            display();
            old_y = y;
        }
    }
    else if (middle_button_pressed){}
    else if (right_button_pressed){
        if (pickedControlPoints.size() > 0) {
            for (vector<int>::iterator pickedPoint = pickedControlPoints.begin(); pickedPoint != pickedControlPoints.end(); pickedPoint++) {
                int deltaX = old_x - x;
                int deltaY = old_y - y;
                if (abs(deltaX) > 3){
                    old_x = x;
                }
                if (abs(deltaY) > 3){
                    old_y = y;
                }
                controlPoints.at(*pickedPoint)->translate(-0.5 * deltaX, 0.5 * deltaY);
            }
            display();
        }
        else if (inConvexHull()){
            // TODO implement
        }
        else{
            if (old_y - y > 3){
                camAngle = fmax(15, camAngle - 0.5);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(camAngle, WINDOW_WIDTH / WINDOW_HEIGHT, Z_NEAR, Z_FAR);
                glMatrixMode(GL_MODELVIEW);
                display();
                old_y = y;
            }
            else if (old_y - y < -3){
                camAngle = fmin(140, camAngle + 0.5);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(camAngle, WINDOW_WIDTH / WINDOW_HEIGHT, Z_NEAR, Z_FAR);
                glMatrixMode(GL_MODELVIEW);
                display();
                old_y = y;
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
//void calcSurface(vector <Vector3f> &contour, vector <Vector3f> &contSurface, int degree, int degree2,int subNum){
//    for (int i = 0; i < contour.size(); i++)
//        printf("y(%d) = %f, ", i, contour[i].y);
//    printf("\n");
//
//    contSurface.clear();
//    float r, R;
//    float const alpha = M_PI / 2.0 / (float)(degree2 - 1);
//    float const rathio = 1.0 / cos(alpha);
//    for (int k = 0; k < subNum; k++)
//    {
//        for (int m = 0; m < 4; m++) //full ring
//        {
//
//            for (int i = 1, j = 0; j < degree2; i += degree - 1, j++) // square
//            {
//
//                for (int l = 0; l < degree; l++) // part of ring
//                {
//                    Vector3f p1, n1;
//
//                    if (l == 0 || l == degree - 1)
//                    {
//                        r = contour[(degree - 1) * k + l].y;
//                        if (j == 0 || j == degree2 - 1)
//                            p1 = Vector3f(contour[(degree - 1) * k + l].x, r*sin((float)((degree2 - 1) * m + j)*alpha), r*cos((float)((degree2 - 1) * m + j)*alpha));
//                        else
//                        {
//                            R = r*rathio;
//                            p1 = Vector3f(contour[(degree - 1) * k + l].x, R*sin((float)((degree2 - 1) * m + j)*alpha), R*cos((float)((degree2 - 1) * m + j)*alpha));
//                        }
//
//                    }
//                    else
//                    {
//                        if (j >0 && j < degree2 - 1)
//                            R = contour[(degree - 1) * k + l].y*rathio;
//                        else R = contour[(degree - 1) * k + l].y;
//
//                        p1 = Vector3f(contour[(degree - 1) * k + l].x, R*sin((float)((degree2 - 1) * m + j)*alpha), R*cos((float)((degree2 - 1) * m + j)*alpha));
//                    }
//                    contSurface.push_back(p1);
//
//                }
//            }
//        }
//    }
//}

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
    old_x = WINDOW_WIDTH / 2;
    old_y = WINDOW_HEIGHT / 2;
    numOfControlPointsPerCurve = 4;
    numOfCurves = 3;
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camAngle, 1, Z_NEAR, Z_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -100);
    //    glLineWidth(4);
    //    glClearColor(0, 0, 0, 1);
    //    last_Point_X = 0;
    //    firstPoint = Vector3f(0,0,0);
    //    lastPoint = Vector3f(last_Point_X, 0, 0);
    //    glEnable(GL_MAP2_VERTEX_3);
    //glEnable(GL_MAP1_VERTEX_3);
    //    glEnable(GL_AUTO_NORMAL);
    //    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    //    glEnable(GL_LIGHTING);
    //    glEnable(GL_LIGHT0);
    //GLfloat light_direction[]={0,-1,0};
    //    GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1.0};
    //    GLfloat light_diffuse[] = {0.0, 1.0, 0.5, 1.0};
    //    GLfloat light_specular[] = {0.0, 0.0, 0.5, 1.0};
    //    GLfloat light_position[] = { 0,0.0,1,0 };
    //GLfloat angle[] = {20.0};
    //    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    //    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    //    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    //    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    //    GLfloat mat_a[] = { 0.3, 0.4, 0.5, 1.0 };
    //    GLfloat mat_d[] = { 0.0, 0.6, 0.7, 1.0 };
    //    GLfloat mat_s[] = { 0.0, 0.0, 0.8, 1.0 };
    //    GLfloat low_sh[] = { 5.0 };
    //    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
    //    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
    //    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
    initLights();
    generateCurves();
}

//void timerFunc(int value){
//
//}

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
}

void readKey(unsigned char key, int x, int y){
    if ('0' <= key && key <= '9') {
        cout << "Design mode. Number of inner control points per curve: " << key << endl;
        design_mode = true;
        numOfControlPointsPerCurve = key - 48 + 2; // map key's ASCII value to its symbolic value, add the two outer points
        clearVectors();
        numOfCurves = 3;
        generateCurves();
        display();
        return;
    }
    if (key == 'd') {
        design_mode = false;
        cout << "3D object mode" << endl;
        //3d mode
        //            calcSurface(<#vector<Vector3f> &contour#>, <#vector<Vector3f> &contSurface#>, <#int degree#>, <#int degree2#>, <#int subNum#>)
        //            drawSurface(<#vector<Vector3f> &surface#>, <#int degree#>, <#int degree2#>, <#int subNum#>)
        return;
    }
    if (key == 'r' || key == 'R') {
        cout << "RESETTING SCENE" << endl;
        clearVectors();
        init();
        //        glLoadIdentity();
        //        glTranslatef(0.0f, 0, -100.0f);
        display();
    }
    if (key == ESC) {
        exit(0);
    }
}

//void reshape(int width, int height){
//    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(camAngle, (GLfloat)width / (GLfloat)height, near, far);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glTranslatef(0.0f, 0, -100.0f);
//}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
//    glutInitWindowPosition(150, 50);
    glutCreateWindow("Bezier's playground");
    init();
    glutKeyboardFunc(readKey);
    glutDisplayFunc(display);
    //    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseClick);
    //    glutTimerFunc(5, timerFunc, 0);
    glutMainLoop();
    return 0;
    
}