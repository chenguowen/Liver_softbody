#define _CRT_SECURE_NO_WARNINGS
#define  GLEW_STATIC
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include <Windows.h>
#include <iostream>
#include "glsl_loader.h"
#include "Draw_C.h" 
//  
// ********************* ������culling  ********************* //  
//   
Draw_multiple_obj *Draw_multiple_obj_real = new Draw_multiple_obj() ; 
// 
void myDisplay()                                 { Draw_multiple_obj_real->myDisplay()                  ;  }
void Mouse_motion(int x,int y )                  { Draw_multiple_obj_real->m_baiscobj->Mouse_motion(x,y);  }
void mouse(int botton,int state,int x,int y)     { Draw_multiple_obj_real->mouse(botton,state,x,y) ; }
void keyboard(unsigned char key, int x, int y)   { Draw_multiple_obj_real->Keyboard(key,x,y);    }  
void Idle()                                      { Draw_multiple_obj_real->Idle();        }
void Reshape(GLsizei w,GLsizei h)                { Draw_multiple_obj_real->Reshape(w,h);  } 
void menu(int num)                               { Draw_multiple_obj_real->menu_display = num ;  }
//  
int main(int argc, char** argv)
 {
	glutInit(&argc, argv);
	// Meffy: ����freeglut�еĺ�������opengl���ڱ��رպͽ�������ʱ���Լ���ִ��glutMainLoop����Ĳ���  
	// glutSetOption(GLUT_MULTISAMPLE, 4 );   
	Draw_multiple_obj_real->glsl_obj_softbody->GLERR() ;
	Draw_multiple_obj_real->InitGL();  
	Draw_multiple_obj_real->glsl_obj_softbody->GLERR() ;
	glutDisplayFunc (&myDisplay   );
	glutMotionFunc  ( Mouse_motion);  // ���������ƶ��� �����
	glutMouseFunc   ( mouse       );  // ������̧����� �����
	glutReshapeFunc ( Reshape     );  // ������̧����� �����
	glutKeyboardFunc( keyboard    );  // ������̧����� �����
	glutIdleFunc    ( Idle );
	glutCreateMenu(menu);
	glutAddMenuEntry("[ ] Normal Display", '0');
	glutAddMenuEntry("[ ] Single lens Display", '1');
	glutAddMenuEntry("[+] Display with back", '2');
	glutAddMenuEntry("[-] Display with front ", '3');
	glutAddMenuEntry("[w] IV Display with back ", '4');
	glutAddMenuEntry("[ ] IV Display with Front ", '5');
	glutAddMenuEntry("[ ] Clipping new", '6');
	glutAddMenuEntry("[ ] Interaction", '7');
	glutAttachMenu(GLUT_RIGHT_BUTTON)  ; //   
	glutMainLoop()  ;  
	return 0;
}