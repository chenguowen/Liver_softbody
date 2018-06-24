#include "Motion_nameVector.h"
///////////////////////////////  Motion_nameVector   ///////////////////////////////////////

Motion_nameVector::Motion_nameVector()
{
	NUM_OF_NAME = 0  ;
	text_save_filename = "data.txt" ;
	First_Write = true ;
	First_read  = true ;
	Write_end_Flag = false ;
	Motion_name_obj.Initlize();
}

Motion_nameVector::~Motion_nameVector()
{  }

void Motion_nameVector::Initlize()
{
	NUM_OF_NAME = 0  ;
	text_save_filename = "data.txt" ; // 
	First_Write = true ;
	First_read  = true ; 
	Write_end_Flag = false ;
	Motion_name_obj.Initlize();
}
void Motion_nameVector::Save_Text(float myFrameKnob)
{
	if (First_Write)
	{
		First_Write = false; // 首次写入符号
		fp  =  fopen(text_save_filename,"w");  
		std::cout<<"First begin of file "<<std::endl;
	}
	// std::cout<<myFrameKnob<<std::endl;

	Motion_name_obj.Get_Status( myFrameKnob );  // 已经做好了保存的工作 

	if (Motion_name_obj.Active_Flag==1)
	{
		fprintf( fp,"%s ", Motion_name_obj.action_name );  
		//fprintf( fp,"%s ", Motion_name_obj.action_name );  
		// 
	}
	// std::cout<<myFrameKnob<<std::endl;


	fprintf(fp,"%f ", myFrameKnob );  // 保存所有的东西 
	if (Motion_name_obj.Active_Flag==2)
	{
		name_vector.push_back( Motion_name_obj ); 
		//s std::cout<<"End _ one gesture "<<std::endl;
		//std::cout<<" "<<std::endl;
		if ( Write_end_Flag == true )
		{
			fclose(fp);
		}
	}
}

void Motion_nameVector::Read_from_File()
{
	if (First_read==true)
	{
		First_read = false; // 首次写入符号
		fp  =  fopen(text_save_filename,"r");  
		std::cout<<"First read of file "<<std::endl;
		int jishu  =  0 ; 
		char  aa[120] ;
		int   begin_dd = 0 ;  
		int   end_dd   = 0 ;   
		int   FLAG  = 1 ; 

		while(FLAG)
		{
			switch (jishu)
			{
			case 0 :
				if (fscanf(fp,"%s ",aa) )
				{
					printf(Motion_name_obj.action_name,aa)  ; 
					jishu++;
				}
				else
				{
					FLAG = 0 ;
				}
				break;
			case 1:
				if (fscanf(fp,"%d ",begin_dd))
				{
					Motion_name_obj.Frame_begin = begin_dd ;
					jishu++; 
				}
				else
				{
					FLAG = 0 ;
				}

				break;
			case 2:
				if (fscanf(fp,"%d ",end_dd) )
				{
					Motion_name_obj.Frame_end = end_dd ;
					name_vector.push_back( Motion_name_obj );
					jishu = 0; 
				}
				else
				{
					FLAG = 0 ;
				}
				break;
			}	
		}
		fclose(fp);
	}
} 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////