// 
#pragma once
#ifndef _SCREEN_SHOT_HEADER_
#define _SCREEN_SHOT_HEADER_
#include <glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <algorithm>
#include <stdio.h>
#include "stdlib.h"
#include "math.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>   // include important windows stuff

#define BITMAP_ID            0x4D42 // universal id for a bitmap
// 
// 用法 需要设置一些区域来存储未来的IV区域
// 
using namespace std;

class SCREEN_SHOT
{
public:
	GLbyte      * data  ;
	int   If_SaveImage  ;
	int   First_Flag    ;
	char* filename_BMP  ;



	bool   Msaa_Flag  ; 
	GLuint  tex ,MSAA_fbo   , m_renderBufferColorMS , m_renderBufferDepthMS ;
	int num_samples ;
	// 只是方便把图像取出来 
	GLuint window_image  ;   // 最终存储IV图像的纹理图像 绑定到该纹理图片的ID上面了   
	GLuint window_fbo    ;   // 最终存储帧缓冲区的ID号码
	GLuint window_oid    ;   // 渲染缓冲区的id号
	//
	//  可以加入普通的那种MSAA_fbo 或者是msaa的FBO   // 
	// 
	int Texture_width  ;
	int Texture_height ;
	// 
	SCREEN_SHOT::SCREEN_SHOT(); 
	SCREEN_SHOT::~SCREEN_SHOT(); 

	void SCREEN_SHOT::Disable_Save()   ;
	void SCREEN_SHOT::Process_Key()   ;
	void SCREEN_SHOT::GET_Texture( GLuint texture_id , int Size_X , int Size_Y)   ;
	bool SCREEN_SHOT::writeBMP   ( const char filename[] , unsigned int w, unsigned int h);  
	void SCREEN_SHOT::InitFBO    (int WIDTH , int HEIGHT)  ; 
	bool SCREEN_SHOT::screenshot( char* filename) ; 
	bool SCREEN_SHOT::writeBMP_from_Data(const char filename[], unsigned char* data, unsigned int w, unsigned int h) ; 
	bool SCREEN_SHOT::writeSingle_BMP_from_Data( const char filename[], unsigned char* data, unsigned int w, unsigned int h) ;

protected: 

private: 

};

#endif