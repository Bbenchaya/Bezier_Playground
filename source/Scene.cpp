//
//  bezier.cpp
//  CG_ex5
//
//  Created by Ben Benchaya on 06/01/16.
//  Copyright © 2016 Bbenchaya. All rights reserved.
//

#include "Bezier.hpp"

#define ESC 27

#define far 200.0
#define near 2.0
#define bufSize 128
#define maxSize 512 //what is maxsize?
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define ROTATION_DEGREE 1
#define RESOLUTION 50
#define POINT_RADIUS 1

using namespace std;

vector <Bezier> curves;
int old_x;
int old_y;
int dotIndex = -1;
int control_points_num;
bool left_button_pressed;
bool middle_button_pressed;
bool right_button_pressed;
bool design_mode;
float camAngle;
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
    for (vector<Bezier>::iterator curve = curves.begin(); curve != curves.end(); curve++) {
        for (float i = 0; i <= RESOLUTION; i++) {
            Vector3f point = curve->getTinXYZ(i / RESOLUTION);
            glVertex3f(point.x, point.y, point.z);
        }
    }
    glEnd();
}

void drawControlPoints(){
    float blue[4] = {0, 0, 1, 1};
    float white[4] = {1, 1, 1, 1};
    //    for (int i = 0; i < 4; i++) {
    //        glTranslatef(1, 1, 0);
    //        glutSolidSphere(1, 20, 20);
    //    }
    for (vector<Bezier>::iterator curve = curves.begin(); curve != curves.end(); curve++) {
        Vector3f point = curve->getPoint(0);
        glLoadIdentity();
        glTranslatef(point.x, point.y, point.z);
        //        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
        printf("x: %f y: %f z: %f\n", point.x, point.y, point.z);
        glutSolidSphere(POINT_RADIUS, 20, 20);
        for (int i = 1; i < curve->getNumOfPoints() - 1; i++) {
            point = curve->getPoint(i);
            glLoadIdentity();
            glTranslatef(point.x, point.y, point.z);
            //            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
            printf("x: %f y: %f z: %f\n", point.x, point.y, point.z);
            glutSolidSphere(POINT_RADIUS, 20, 20);
        }
        if (curve->isRightmost()) {
            Vector3f point = curve->getPoint(curve->getNumOfPoints() - 1);
            glLoadIdentity();
            glTranslatef(point.x, point.y, point.z);
            //            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
            printf("x: %f y: %f z: %f\n", point.x, point.y, point.z);
            glutSolidSphere(POINT_RADIUS, 20, 20);
        }
    }
}


void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawAxes();
    //    drawCurve(curve, GL_RENDER, ROTATION_DEGREE, control_points_num);
    drawCurves();
    drawControlPoints();
    glFlush();
}

void mouse(int button, int state, int x, int y){
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
    }
    
}

void motion(int x, int y){
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
        if (old_y - y > 0){
            glTranslatef(0, 0, 5);
            display();
            old_y = y;
        }
        else{
            glTranslatef(0, 0, -5);
            display();
            old_y = y;
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

void init(){
    glLineWidth(4);
    glClearColor(0, 0, 0, 1);  //black
    camAngle = 60.0;
    left_button_pressed = false;
    middle_button_pressed = false;
    right_button_pressed = false;
    design_mode = true;
    old_x = WINDOW_WIDTH / 2;
    old_y = WINDOW_HEIGHT / 2;
    control_points_num = 0;
    //    last_Point_X = 0;
    //    firstPoint = Vector3f(0,0,0);
    //    lastPoint = Vector3f(last_Point_X, 0, 0);
    glEnable(GL_MAP2_VERTEX_3);
    //glEnable(GL_MAP1_VERTEX_3);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //GLfloat light_direction[]={0,-1,0};
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 }; //color
    GLfloat light_diffuse[] = { 0.0, 1.0, 0.5, 1.0 }; //color
    GLfloat light_specular[] = { 0.0, 0.0, 0.5, 1.0 };
    //    GLfloat light_position[] = { 0,0.0,1,0 };
    //GLfloat angle[] = {20.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    //    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    GLfloat mat_a[] = { 0.3, 0.4, 0.5, 1.0 };
    GLfloat mat_d[] = { 0.0, 0.6, 0.7, 1.0 };
    GLfloat mat_s[] = { 0.0, 0.0, 0.8, 1.0 };
    //    GLfloat low_sh[] = { 5.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
    //    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_sh);
    // start with 3 cubic Beziers
    Vector3f v1[4] = {Vector3f(0, 0, 0), Vector3f(4, 8, 0), Vector3f(8, 12, 0), Vector3f(12, 13, 0)};
    Vector3f v2[4] = {Vector3f(12, 13, 0), Vector3f(16, 13, 0), Vector3f(20, 13, 0), Vector3f(24, 13, 0)};
    Vector3f v3[4] = {Vector3f(24, 13, 0), Vector3f(28, 12, 0), Vector3f(32, 8, 0), Vector3f(36, 0, 0)};
    Bezier b1(4, v1);
    Bezier b2(4, v2);
    Bezier b3(4, v3);
    b1.setExtremum(LEFTMOST);
    b3.setExtremum(RIGHTMOST);
    curves.push_back(b1);
    curves.push_back(b2);
    curves.push_back(b3);
}

//void timerFunc(int value){
//
//}

void readKey(unsigned char key, int x, int y){
    switch (key){
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            design_mode = true;
            control_points_num = key;
            display();
            break;
        case 'd':
            design_mode = false;
            //3d mode
            //            calcSurface(<#vector<Vector3f> &contour#>, <#vector<Vector3f> &contSurface#>, <#int degree#>, <#int degree2#>, <#int subNum#>)
            //            drawSurface(<#vector<Vector3f> &surface#>, <#int degree#>, <#int degree2#>, <#int subNum#>)
            break;
        case 'r':
            init();
            glLoadIdentity();
            glTranslatef(0.0f, 0, -100.0f);
            display();
            break;
        case ESC:
            exit(0);
    }
    
}

void reshape(int width, int height){
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camAngle, (GLfloat)width / (GLfloat)height, near, far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0, -100.0f);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    //glutInitWindowPosition(150, 50);
    glutCreateWindow("curves");
    init();
    glutKeyboardFunc(readKey);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    //    glutTimerFunc(5, timerFunc, 0);
    glutMainLoop();
    return 0;
    
}