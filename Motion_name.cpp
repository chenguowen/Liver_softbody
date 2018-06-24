#include "Motion_name.h"

Motion_name::Motion_name()
{
	Active_Flag = 0 ; // action_name = (char* )malloc(100*sizeof(char)) ;
}
Motion_name::~Motion_name()
{ }

void Motion_name::Initlize()
{
	Active_Flag  = 0;
	// action_name  = (char* )malloc(100*sizeof(char))  ;
}
// 
void Motion_name::Get_Status(float frame_now)
{
	if (Active_Flag==0|| Active_Flag==2)
	{
		// std::cout<<" "<<std::endl;
		// std::cout<<"Begin _ one gesture "<<std::endl;
		Frame_begin  = frame_now     ;
		scanf_s ("%s",  action_name ,128 ) ;
		// printf( "%s\n", action_name);
		Active_Flag=1;
	} 
	else // (Active_Flag==1)
	{
		Frame_end =frame_now;
		Active_Flag = 2 ;
	}
}

