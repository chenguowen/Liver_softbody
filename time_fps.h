#pragma once 
#ifndef __TIMER_FPS__
#define __TIMER_FPS__
#include "time.h"
#include <stdio.h>

class time_fps 
{
public:
	int    counter ;  // ����
	float  cl      ;  // clock��ʼʱ��ʱ��  
	
	float  fps      ; // ֡��
	float  aver_fps ; // ֡��
	FILE    *fp111  ; // �ļ� 
	int     FirstFLag ;

	float  sum_all1   ; // 
	bool   show_flag  ; //  
	int    iter_num   ; // 
	// 
	time_fps(); 
	~time_fps(); 

	void  time_fps::Frame_Save(double fps,double pitch_iter ) ;
	void  time_fps::frame_display() ; 
	void  time_fps::frame_calculate() ; 

};
#endif