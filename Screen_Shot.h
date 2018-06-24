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
// �÷� ��Ҫ����һЩ�������洢δ����IV����
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
	// ֻ�Ƿ����ͼ��ȡ���� 
	GLuint window_image  ;   // ���մ洢IVͼ�������ͼ�� �󶨵�������ͼƬ��ID������   
	GLuint window_fbo    ;   // ���մ洢֡��������ID����
	GLuint window_oid    ;   // ��Ⱦ��������id��
	//
	//  ���Լ�����ͨ������MSAA_fbo ������msaa��FBO   // 
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