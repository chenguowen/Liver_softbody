#pragma once 
#ifndef ___ROATION_CONTROL__
#define ___ROATION_CONTROL__

#define GLEW_STATIC  //  因为用到了扩展项目 
// 链接静态库，必需先定义GLEW_STATIC  
#include "GL/glew.h"
#include <GL/freeglut.h>
#include <cmath>
#include <stdio.h>
#include "stdlib.h"
#include "math.h"
#include <algorithm>
#include <GL/glaux.h>
#include "controler.h"
#include <iostream>

using namespace std ; 

class Obj_control
{
public:

	Obj_control();
	Obj_control(GLfloat a , GLfloat b, GLfloat c );
	~Obj_control();
	
	//  面向对象全部完成 //   Leap motion的监听部分  
	//  完全是一个对象，里面包含手的个数，手的位置
	//  
	SampleListener   *  listener ; // 包含所有的变量 
	
	
	float   Move_Z  ; 
	GLfloat spin_x  ; // 关于旋转的事情 
	GLfloat spin_y  ; // 关于旋转的事情 

	int     Latest_State  ;
	float   Rotation_index ;

	double  rotxyz[3] ;   
	int     Last_xy[2],Now_xy[2] ;  
	int     Mesh_Size;

	GLfloat * Now_Matrix_Mouse  ;  // 实时鼠标交互设备产生的旋转
	GLfloat * Now_Matrix_Leap   ;  // Leap motion交互设备产生的旋转
	GLfloat * Indentity ; 


	GLfloat * Transform_Matrix_Leap  ; // 旋转控制矩阵          //  
	
	GLfloat * Transform_Matrix_Mouse ; // 旋转控制矩阵          //  
	GLfloat * Transform_Matrix_Left_hand   ; // 旋转控制矩阵          //  
	GLfloat * Transform_Matrix_Right_hand  ; // 旋转控制矩阵          //  


	
	GLfloat * Init_Grasp_Inv_Mat ; //  初始抓取角度的逆矩阵 
	GLfloat * Init_Grasp_Inv_Mat_Left  ; //  初始抓取角度的逆矩阵 
	GLfloat * Init_Grasp_Inv_Mat_Right ; //  初始抓取角度的逆矩阵 


	GLfloat * Mag_Rotation_Mat ; 
	GLfloat * Backup_Rotation_Matrix  ; // 单位矩阵       //  
	GLfloat * Backup_Translation  ; 
	GLfloat   x  , y  , z ;      //  
	
	void Obj_control::Matrix_Vector_Reset() ;
	void Obj_control::Matrix_Vector_mul( GLfloat*Vector2 ,GLfloat*Matrix1,GLfloat*Vector1) ;
	void Obj_control::Refresh_4f_Transform_Matrix(GLfloat* Transform_Matrix, GLfloat* RotationMatrix);
	void Obj_control::Cross_Matrix( GLfloat*Matrix ,GLfloat*Matrix1,GLfloat*Matrix2) ;
	void Obj_control::Roderigues(GLfloat* RotationMatrix,GLfloat *NowRotation_Matrix , double* rotxyz) ;
	void Obj_control::spinDisplay(void) ;
	void Obj_control::Process_Leap_Motion (void) ;
	void Obj_control::Mouse_motion(int x,int y ) ;
	void Obj_control::keyboard(unsigned char key, int x, int y)  ;
	void Obj_control::Idle()  ;
	void Obj_control::mouse(int botton,int state,int x,int y) ; 
	void Obj_control::Matrix_Equal_3by3 ( GLfloat*Matrix1 , GLfloat*Matrix2  ) ;
	void Obj_control::Matrix_Equal_4by4 ( GLfloat*Matrix1 , GLfloat*Matrix2  ) ; 
	void Obj_control::Matrix_Equal_Inv_4by4 ( GLfloat*Matrix1 , GLfloat*Matrix2  ) ;

	void Obj_control::Process_Leap_Motion_left  (  void )  ;
	void Obj_control::Process_Leap_Motion_right (  void )  ; 


protected:
private:

	bool Full_Screen_Flag ;
};
#endif