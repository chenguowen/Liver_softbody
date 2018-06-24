// 一个全局的IP头文件
// 一个全局的IP头文件
#pragma once

#ifndef  _MULTIPE_VIEW_H_
#define  _MULTIPE_VIEW_H_
#include <omp.h>
#include <glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include "glm/glm.hpp" // glm 
#include "LoadObj.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"

class  Multiple_View
{
public:
	double  PITCH ; 
	double  PITCH_Y ; 
	double  GAP   ;
	double  PPI   ; 
	double  dx;
	int MLA_NUMBDER_HEXAGON ; 
	int MLA_NUMBDER_RECTANGLE ; 
	int double_of_pixel ;

	float **Multiple_Lens_Project_Matrix_Front; 
	float **Multiple_Lens_Project_Matrix_Back; 

	int  ***Multiple_Viewport    ; //  
	float***View_Center_Back     ; // 
	float***View_Center_Front    ; // 
	// 
	bool  *** View_Render_Flag   ;  //  是否渲染的东西  // 

	int     Radius_X     ;
	int     Radius_Y     ;
	int     IsFirst_Flag ;  

	int     IF_Square    ;

	int WIDTH  ;  // 图像宽度
	int HEIGHT ;  // 图像高度  

	/*
	  int TEXTURE_WIDTH_Set ;
	  int TEXTURE_HEIGHT_Set;
	*/

	int Perspective_Pixel_num_x  ;
	int Perspective_Pixel_num_y  ;
	//
	int LENS_NUM_Y ;
	int LENS_NUM_X ; 
	// 
	glm::mat4 Identity_Matrix ;  // 
	float3 Bound_box_Init[2]  ;  // 
	// 
	// 
public:
	Multiple_View()  ;
	void Multiple_View_Set(double pitch, double gap, double ppi , double width, double height , int Flag ) ;
	void Cross_Matrix( float*Matrix ,float*Matrix1,float*Matrix2);
	void Multiple_Lens_Project_Matrix_Generation();
	void Pitch_Reset_fcn();

	void Multiple_View::View_Render_Flag_Launch ( glm::mat4 Model_View_Matrix , float3* Bound_box    ) ; //  ;
	void Multiple_View::View_Render_Flag_Free  (   ) ; // 
	void Multiple_View::View_Render_Flag_Update(   ) ; // 

	void Multiple_View::Multiple_Viewport_Set() ;
	void Multiple_View::Multiple_Viewport_Free() ;    //
	void Multiple_View::Multiple_View_Center_Free() ; // 
	void Multiple_View::Multiple_Lens_Project_Matrix_Free() ; //
	~Multiple_View() ; 
}; 
#endif