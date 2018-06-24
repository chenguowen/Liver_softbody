#pragma once 
#ifndef ___ROATION_CONTROL__
#define ___ROATION_CONTROL__

#define GLEW_STATIC  //  ��Ϊ�õ�����չ��Ŀ 
// ���Ӿ�̬�⣬�����ȶ���GLEW_STATIC  
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
	
	//  �������ȫ����� //   Leap motion�ļ�������  
	//  ��ȫ��һ��������������ֵĸ������ֵ�λ��
	//  
	SampleListener   *  listener ; // �������еı��� 
	
	
	float   Move_Z  ; 
	GLfloat spin_x  ; // ������ת������ 
	GLfloat spin_y  ; // ������ת������ 

	int     Latest_State  ;
	float   Rotation_index ;

	double  rotxyz[3] ;   
	int     Last_xy[2],Now_xy[2] ;  
	int     Mesh_Size;

	GLfloat * Now_Matrix_Mouse  ;  // ʵʱ��꽻���豸��������ת
	GLfloat * Now_Matrix_Leap   ;  // Leap motion�����豸��������ת
	GLfloat * Indentity ; 


	GLfloat * Transform_Matrix_Leap  ; // ��ת���ƾ���          //  
	
	GLfloat * Transform_Matrix_Mouse ; // ��ת���ƾ���          //  
	GLfloat * Transform_Matrix_Left_hand   ; // ��ת���ƾ���          //  
	GLfloat * Transform_Matrix_Right_hand  ; // ��ת���ƾ���          //  


	
	GLfloat * Init_Grasp_Inv_Mat ; //  ��ʼץȡ�Ƕȵ������ 
	GLfloat * Init_Grasp_Inv_Mat_Left  ; //  ��ʼץȡ�Ƕȵ������ 
	GLfloat * Init_Grasp_Inv_Mat_Right ; //  ��ʼץȡ�Ƕȵ������ 


	GLfloat * Mag_Rotation_Mat ; 
	GLfloat * Backup_Rotation_Matrix  ; // ��λ����       //  
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