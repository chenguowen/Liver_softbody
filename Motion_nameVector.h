#pragma once
#ifndef __MOTION_NAME_VECTOR_H__
#define __MOTION_NAME_VECTOR_H__

#include "Motion_name.h" 
//  
class Motion_nameVector
{
public:
	std::vector<Motion_name>  name_vector  ;
	int NUM_OF_NAME  ;
	char* text_save_filename ;
	Motion_nameVector()   ;
	~Motion_nameVector()  ;
	Motion_name  Motion_name_obj ; // 实例化一个Motion_name的对象 
	FILE *fp; 
	bool  First_Write; 
	bool  First_read ;
	bool  Write_end_Flag ;

	void Initlize()       ; 
	void Made_From_Key()  ;
	void Save_Text(float myFrameKnob)      ;
	void Read_from_File(); 
protected:

private:

};
#endif
