#pragma once

#ifndef _TXTREAD_H_
#define _TXTREAD_H_

#include "stdlib.h"
#include <iomanip>
#include <iostream> 
#include <GL/freeglut.h>
#include <gl/gl.h>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <cmath>
using namespace std;

class txt_Draw
{ // 
public:
	txt_Draw::txt_Draw() ;
	txt_Draw::~txt_Draw() ;
	void   txt_Draw::drawtext();
	void   txt_Draw::Set_text(char * str , double x ,double y );
	GLvoid txt_Draw::glPrint (char *fmt,...) ;
	// 
private:
	char * text_display ; // 
	double ROW        ;
	double VOL        ;
	GLuint base       ; 
	bool  t           ;
	bool  incline     ; 
	bool  underline   ;
	int   Fontsize    ;
	GLfloat fcolorr   ; 
	GLfloat fcolorg   ;
	GLfloat fcolorb   ;
	string  myfont    ;
	int     Weight    ;
}; // 
#endif