#pragma once
#ifndef __MOTION_NAME__H__
#define __MOTION_NAME__H__

#include <vector>
#include <iostream>    /* for printf and NULL */
#include <string>    /* for printf and NULL */
#include <stdio.h>    /* for printf and NULL */
#include <stdlib.h>   /* for exit */
#include <math.h>     /* for sqrt, sin, cos, and fabs */
#include <assert.h>   /* for assert */
/*
A header to capture frames of movies 
Final result is saved in a 
*/

class Motion_name
{
public:
	char action_name[128];
	float Frame_begin  ;
	float Frame_end    ;
	int   Active_Flag  ;
	Motion_name();
	~Motion_name();
	void Initlize(); 
	void Get_Status(float frame_now);
protected:

private:

};

#endif