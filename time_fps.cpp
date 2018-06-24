//#include "StdAfx.h"
#include "time_fps.h"

time_fps::time_fps()
{
	counter = 0;
	fps     = 0; 
	iter_num= 0;
	sum_all1 =0;  
	show_flag = false ;
	FirstFLag = 1 ;
	fp111       =   fopen("Frame_test.txt","w") ;
}

time_fps::~time_fps()
{ }

void time_fps::frame_display() 
{
	if ( fps > 0 && show_flag )
	{
		// printf("FPS  = %f \n",fps);
		show_flag    = false ; 
	}
}


// 
void time_fps::frame_calculate() 
{
	if(counter==0)
	{
		cl=clock();
	}
	if(counter++==100)
	{
		iter_num ++ ;
		
		counter  = 0  ;
		fps      = 100000.0f/(clock()-cl) ;  
		sum_all1 = sum_all1 + fps ;
		cl       = clock() ;
		show_flag = true ;
	}
}

// 
void time_fps::Frame_Save(double fps,double pitch_iter )
{
	//fprintf( fp111 , "pitch = %f fps = %f \n" , pitch_iter , fps );  
}