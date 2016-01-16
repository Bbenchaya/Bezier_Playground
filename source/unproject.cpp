//#include <GLUT/GLUT.h>
//int xpos = 0;   // current x position
//int ypos = 0;   // current y position
//int orig_xpos = 0; //where the user first clicked x
//int orig_ypos = 0; //where the use first clicked y
//GLdouble newx, newy, newz; //the new object space coordinates
//GLdouble mx, my, mz; // move with this vector
//
//GLdouble model_matrix[16];
//GLdouble proj_matrix[16];
//GLint view_matrix[4];
//
//void setMatrixes()
//{
//    glGetDoublev(GL_MODELVIEW_MATRIX, model_matrix);
//    glGetDoublev(GL_PROJECTION_MATRIX, proj_matrix);
//    glGetIntegerv(GL_VIEWPORT, view_matrix);
//}
//
//void GLFWCALL onMouseButton( int button, int action )
//{
//    if( button == GLFW_MOUSE_BUTTON_LEFT )
//    {
//        dragging = action == GLFW_PRESS;
//        glfwGetMousePos(&xpos, &ypos);
//        
//        if(orig_xpos == 0 && orig_ypos == 0)
//        {
//            orig_xpos = xpos;
//            orig_ypos = ypos;
//        }
//        gluUnProject(xpos, ypos, 0, model_matrix, proj_matrix, view_matrix, &newx, &newy, &newz);
//    }
//}
//
//void moveObject(int dx, int dy)
//{
//    GLdouble nx, ny, nz;
//    gluUnProject(dx, dy, 0, model_matrix, proj_matrix, view_matrix, &nx, &ny, &nz);
//    
//    mx = newx - nx;
//    my = newy - ny;
//    mz = newz - nz;
//}
//
//void GLFWCALL onMouseMove(int x, int y)
//{
//    if(dragging)
//    {
//        moveObject(orig_xpos - x, orig_ypos - y);
//    }
//}
//
//int main()
//{
//    init();
//    
//    glfwSetMouseButtonCallback(onMouseButton);
//    glfwSetMousePosCallback(onMouseMove);
//    glEnable(GL_DEPTH_TEST);
//    
//    glClearColor( 1,1,1,1 );
//    
//    while(running())
//    {
//        glClear(GL_COLOR_BUFFER_BIT + GL_DEPTH_BUFFER_BIT);
//        glLoadIdentity();
//        gluLookAt(15, 0, 5, 0, 0, 0, 0, 0, 1 );
//        setMatrixes();
//        
//        /* A simple sphere, but should work with more simple objects */
//        glColor3ub(0, 0, 0);
//        glPushMatrix();
//        glTranslated(mx, my, mz);
//        drawSmoothUnitySphere();
//        glPopMatrix();
//        
//        glfwSwapBuffers();
//    }
//    
//    glfwTerminate();
//    return 0;
//}