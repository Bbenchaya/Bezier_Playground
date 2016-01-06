//
//  bezier.cpp
//  CG_ex5
//
//  Created by Ben Benchaya on 06/01/16.
//  Copyright © 2016 Bbenchaya. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "Vector3f.h"
#include <GLUT/GLUT.h>

#define ESC 27

#define far 200.0
#define near 2.0
#define bufSize 128
#define maxSize 512 //what is maxsize?
bool press;
float camAngle;
GLint hits;

using namespace std;

void drawAxis(){
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

void changePointPosition(vector <Vector3f> &control, Vector3f diff){
    
}

void drawCurve(vector <Vector3f> &curve, GLenum mode, int degree, int subNum){
    
}

/////////////////////////////
//surface: 2D NURBS
//degree is the degree of each polynom in u
//degree2 is the degree of each polynom in v
//subNum is number of sub curve, how many parts in the whole curve
//////////////////////////////
void drawSurface(vector <Vector3f> &surface,int degree, int degree2, int subNum)
{
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
                for (int i = 0; i <= 30; i++)
                {
                    glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)j / 8.0);
                    glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)(j + 1) / 8.0);
                }
                glEnd();
                
            }
            
            
        }
    }
}

void drawShape(){
    float red[4] = {1, 0, 0, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
    glutSolidSphere(1.0, 50, 50);
}

void splitCurve(vector <Vector3f> &curve, int curveIndx, int degree, int &subNum)
{
    
}

void deleteCurve(vector <Vector3f> &curve, int curveIndx, int degree, int &subNum){}

void makeStraight(vector <Vector3f> &curve, int curveIndx, int degree){
}

void derevativeContinuity(vector <Vector3f> &curve, int vertexIndx, int degree){
}

void startPicking(GLuint *selectionBuf){
    GLint hits;
    glSelectBuffer(maxSize * 12, selectionBuf); //declare buffer for input in selection mode
    glRenderMode(GL_SELECT); //change to selecting mode
    glInitNames();			//initialize names stack
    glPushName(-1);			//push name
    //======================
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    //    gluPickMatrix(x, y, 1.0, 1.0, view);
    //    gluPerspective(FOV, 1.0, Z_NEAR, Z_FAR);
    glMatrixMode(GL_MODELVIEW);
    //    drawObjects(GL_SELECT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    hits = glRenderMode(GL_RENDER);
    //    processPicks(hits, selectionBuf);
    //    display();
    
}

void processHits(GLint hits, GLuint *buffer){
}

void mouse(int button, int state, int x, int y){
    GLint viewport[4];
    GLuint selectionBuf[bufSize];
    glGetIntegerv(GL_VIEWPORT,viewport); //reading viewport parameters
    
    press=!press;
		  if(press){
              glMatrixMode(GL_PROJECTION);
              glPushMatrix();	//saves current projection matrix
              glLoadIdentity();
              startPicking(selectionBuf); //preper selection mode
              gluPickMatrix((GLdouble) x,(GLdouble) viewport[3]-y,1,1,viewport);
              gluPerspective(camAngle,1,near,far);
              glMatrixMode(GL_MODELVIEW);
//              drawCurve(<#vector<Vector3f> &curve#>, GL_SELECT, <#int degree#>, <#int subNum#>);
              hits=glRenderMode(GL_RENDER); //gets hits number
              glMatrixMode(GL_PROJECTION);
              glPopMatrix(); //restores projection matrix
              glMatrixMode(GL_MODELVIEW);
              processHits(hits,selectionBuf); //check hits
          }
}

void motion(int x, int y){
}

/////////////////////////////
//contour: vector contains your 1D curve. The coordinates of Pn and P0 of consecutive sub curves
//appear just once in the vector
//contSurface, function output, contains the 2D surface calculate from the 1D curve
//degree is the degree of the each polynom in t
//degree2 is the degree of polynom in another dymention
//subNum is number of sub curve, how many parts in the whole curve
//////////////////////////////
void calcSurface(vector <Vector3f> &contour, vector <Vector3f> &contSurface, int degree, int degree2,int subNum)
{
    for (int i = 0; i < contour.size(); i++)
        printf("y(%d) = %f, ", i, contour[i].y);
    printf("\n");
    
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

void init(){
    
    glLineWidth(4);
    glClearColor(0, 0, 0, 1);  //black
    camAngle = 60.0;
    press = false;
    
    glEnable(GL_MAP2_VERTEX_3);
    //glEnable(GL_MAP1_VERTEX_3);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
    //GLfloat light_direction[]={0,-1,0};
    //GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 }; //color
    GLfloat light_diffuse[] = { 0.0, 1.0, 0.5, 1.0 }; //color
    GLfloat light_specular[] = { 0.0, 0.0, 0.5, 1.0 };
    GLfloat light_position[] = { 0,0.0,1,0 };
    //GLfloat angle[] = {20.0};
    //glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    GLfloat mat_a[] = { 0.3, 0.4, 0.5, 1.0 };
    GLfloat mat_d[] = { 0.0, 0.6, 0.7, 1.0 };
    GLfloat mat_s[] = { 0.0, 0.0, 0.8, 1.0 };
    GLfloat low_sh[] = { 5.0 };
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_sh);
}

void timerFunc(int value){
    
}

void readKey(unsigned char key, int x, int y){
    switch (key){
        case '1':
            break;
        case '2':
            break;
        case '3':
            break;
        case '4':
            break;
        case '5':
            break;
        case '6':
            break;
        case '7':
            break;
        case '8':
            break;
        case '9':
            break;
        case 'd':
            //3d mode
            break;
        case ESC:
            exit(0);
    }

}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glFlush();
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
    glutTimerFunc(5, timerFunc, 0);
    glutMainLoop();
    
    return 0;
}