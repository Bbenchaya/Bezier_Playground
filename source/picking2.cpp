//#include <cstdio>
//#include <cmath>
//#include <cstdlib>
//#include <GLUT/GLUT.h>
//#define bufSize 128
//#define camAngle 45.0
//#define far 20.0
//#define near 2.0
//
//bool play;
//bool press;
//bool rot;
//float rotY;
//float modelMatrix[16],projectionMatrix[16];
//int ii;
//int jj;
//float transX[4][4];
//float transY[4][4];
//float transZ[4][4];
//float zValue[4];
//GLint hits;
//GLfloat pixels[512*512];
//int xx;
//int yy;
//bool depth;
//bool isPick;
//bool zMove;
//
//void printModel() //prints modelview matrix
//{
//    glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
//    printf("Modelview matrix: \n");
//    for(int i=0;i<4;i++)
//    {
//        for(int j=0;j<4;j++)
//            printf("%f ",modelMatrix[j*4+i]);
//        printf("\n");
//    }
//}
//
//void printProj() //prints projection matrix
//{
//    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
//    printf("projection matrix: \n");
//    for(int i=0;i<4;i++)
//    {
//        for(int j=0;j<4;j++)
//            printf("%f ",projectionMatrix[j*4+i]);
//        printf("\n");
//    }
//}
//
//
//void readDepth(bool b) // read depth buffer to pixels
//{
//    glPixelTransferf(GL_DEPTH_SCALE,1);
//    glReadPixels(0,0,512,512,GL_DEPTH_COMPONENT,GL_FLOAT,pixels);
//    if(b)
//    { //print depth map gray scale
//        glDrawPixels(512,512,GL_LUMINANCE,GL_FLOAT,pixels);
//    }
//    
//}
//
//void drawBoard(GLenum mode) //draws square
//{
//    //glAlphaFunc(GL_LESS,0.5);
//    for(float i=-0.5;i<=1;i+=0.5)
//    {
//        if(mode==GL_SELECT)
//            glLoadName( i*2+2);
//        for(float j=-0.5;j<=1;j+=0.5)
//        {
//            if(mode==GL_SELECT) //pushes a name to names stack
//                glPushName(j*2+2);
//            
//            glColor4f(0.5+i/2.0,0.5+j/2.0,(1-i-j)/3.0,(1-i-j)/3.0);
//            
//            glPushMatrix();
//            //translate according to mouse
//            glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
//            glTranslatef(transX[(int)(i*2+1)][(int)(j*2+1)]*modelMatrix[0],transX[(int)(i*2+1)][(int)(j*2+1)]*modelMatrix[4],transX[(int)(i*2+1)][(int)(j*2+1)]*modelMatrix[8]);
//            glTranslatef(transY[(int)(i*2+1)][(int)(j*2+1)]*modelMatrix[1],transY[(int)(i*2+1)][(int)(j*2+1)]*modelMatrix[5],transY[(int)(i*2+1)][(int)(j*2+1)]*modelMatrix[9]);
//            glTranslatef(transZ[(int)(i*2+1)][(int)(j*2+1)]*modelMatrix[2],transZ[(int)(i*2+1)][(int)(j*2+1)]*modelMatrix[6],transZ[(int)(i*2+1)][(int)(j*2+1)]*modelMatrix[10]);
//            
//            
//            glBegin(GL_QUADS);
//            glVertex3f(i,j,-1+(i+j)/2.0);
//            glVertex3f(i-0.5,j,-1+(i+j)/2.0);
//            glVertex3f(i-0.5,j-0.5,-1+(i+j)/2.0);
//            glVertex3f(i,j-0.5,-1+(i+j)/2.0);
//            glEnd();
//            
//            glPopMatrix();
//            
//            if(mode==GL_SELECT)
//                glPopName();
//        }
//    }
//}
//
//void mydisplay(void)
//{ //draws static scene
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    
//    if(play)
//    {
//        glRotatef(rotY,0,1,0);
//    }
//    
//    drawBoard(GL_RENDER);
//    if(depth)
//        readDepth(true);
//    
//    glFlush();
//}
//
//void init()
//{
//    glMatrixMode(GL_PROJECTION); /* switch matrix mode */
//    glLoadIdentity();		//load Identity matrix
//    glClearColor(0,0,0,1);  //black
//    
//    gluPerspective(camAngle,1,near,far);
//    
//    
//    glMatrixMode(GL_MODELVIEW); /* switch matrix mode */
//    glLoadIdentity();		//load Identity matrix
//    glTranslatef(0,0,-3);
//    //enable tests
//    glEnable(GL_ALPHA_TEST);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_STENCIL_TEST);
//    
//    rotY=0.05;
//    play=false;
//    depth=false;
//    press=false;
//    rot=false;
//    isPick=false;
//    zMove=false;
//    ii=0;
//    jj=0;
//    for(int i=0;i<4;i++)
//    {
//        for(int j=0;j<4;j++)
//        {
//            transX[i][j]=0;
//            transY[i][j]=0;
//            transZ[i][j]=0;
//        }
//    }
//}
//
//void startPicking(GLuint *selectionBuf)
//{
//    glSelectBuffer(bufSize,selectionBuf); //declare buffer for input in selection mode
//    glRenderMode(GL_SELECT); //change to selecting mode
//    glInitNames();			//initialize names stack
//    glPushName(-1);			//push name
//}
//
//void processHits(GLint hits, GLuint *buffer)
//{
//    float z1,z2;
//    printf("\nbuffer\n");
//    for(int i=0;buffer[i]>0;i+=5)
//    {
//        z1=buffer[i+1]/4294967295.0;
//        z2=buffer[i+2]/4294967295.0;
//        
//        printf("z1 = %f ,z2 = %f zValue = %f\n",z1,z2,zValue[0]);
//        
//        
//        
//        if((zValue[0]<=z1+0.0001 && zValue[0]>=z2-0.0001) || (zValue[0]>=z1-0.0001 && zValue[0]<=z2+0.0001))
//        {	//try to locate which name is correlated with the pressed pixel according to z value
//            ii=buffer[i+3];
//            jj=buffer[i+4];
//        }
//        
//    }
//    
//}
//
//void readKey (unsigned char key, int xmouse, int ymouse)
//{
//    switch(key)
//    {
//        case ' ':
//            //pause
//            play=!play;
//            break;
//        case 27: //escape
//            exit(0);
//            break;
//        case '1': //depth map
//            depth=!depth;
//            break;
//        case '0':
//            printModel();
//            printProj();
//            break;
//    }
//}
//
//void disp(int value)
//{
//    glutPostRedisplay(); //call to mydisplay functoin
//    glutTimerFunc(3,disp,0);
//}
//
//void mouse(int button, int state, int x, int y)
//{
//    GLint viewport[4];
//    GLuint selectionBuf[bufSize];
//    
//    float pix[4];
//    
//    
//    for(int i=0;i<bufSize;i++)
//    {
//        selectionBuf[i]=0;
//    }
//    glGetIntegerv(GL_VIEWPORT,viewport); //reading viewport parameters
//    
//		  press=!press;
//		  if(press)
//          {   //use selection mode to pick
//              
//              glReadPixels(x,viewport[3]-y,1,1,GL_RGBA,GL_FLOAT,pix);
//              
//              //	  printf("depth = %f, x = %d, y = %d\n",pixels[(viewport[3]-y)*512+x],x,viewport[3]-y);
//              
//              
//              glMatrixMode(GL_PROJECTION);
//              
//              glReadPixels((GLdouble) x,(GLdouble) viewport[3]-y,2,2,GL_DEPTH_COMPONENT,GL_FLOAT,zValue);
//              
//              glPushMatrix();	//saves current projection matrix
//              glLoadIdentity();
//              
//              startPicking(selectionBuf); //preper selection mode
//              
//              gluPickMatrix((GLdouble) x,(GLdouble) viewport[3]-y,1,1,viewport); //change matrices so only the area of the picking pixel can be seen.
//              gluPerspective(camAngle,1,near,far); //return to perspective state
//              
//              glMatrixMode(GL_MODELVIEW);
//              drawBoard(GL_SELECT); //draws board on background
//              
//              
//              hits=glRenderMode(GL_RENDER); //gets hits number
//              
//              glMatrixMode(GL_PROJECTION);
//              glPopMatrix(); //restores projection matrix
//              
//              glMatrixMode(GL_MODELVIEW);
//              processHits(hits,selectionBuf); //check hits
//              
//              //  printf("depth %f hits: %d\n\n",pixels[(viewport[3]-y)*512+x], hits);
//              if(zValue[0]<1.0)
//              {
//                  isPick=true;
//                  xx=x;
//                  yy=y;
//                  if(button== GLUT_RIGHT_BUTTON)
//                      zMove=true;
//                  else zMove=false;
//              }
//              
//          }
//          else isPick=false;	
//    
//}
//
//void motion(int x,int y)
//{
//    
//    if(isPick)
//    {
//        if(zMove)
//        {
//            transZ[ii-1][jj-1]-=(xx-x)/100.0;
//            xx=x;
//        }
//        else
//        {
//            GLint viewport[4];
//            glGetIntegerv(GL_VIEWPORT,viewport);
//            float z=far+zValue[0]*(near-far);
//            //calculate the difference in x and y from the last call
//            transX[ii-1][jj-1]-=(xx-x)/(float) (viewport[2])*near*2.0*tan(camAngle*3.14159/360.0)*(far/z);
//            transY[ii-1][jj-1]+=(yy-y)/(float) (viewport[3])*near*2.0*tan(camAngle*3.14159/360.0)*(far/z);
//            //update x and y
//            xx=x;
//            yy=y;
//        }
//    }
//}
//
//int main(int  argc,  char** argv) 
//{
//    
//    glutInit (& argc, argv) ;
//    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH) ; //double buffering
//    glutInitWindowSize ( 512,512) ;
//    glutCreateWindow("Picking") ;
//    init();
//    
//    glutMotionFunc(motion);
//    glutKeyboardFunc(readKey);
//    glutMouseFunc(mouse);
//    glutDisplayFunc(mydisplay);
//    glutTimerFunc(5,disp,0);
//    glutMainLoop () ; 
//    
//    
//}