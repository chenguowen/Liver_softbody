//   适应纹理合成 // 
#include "leap/Leap.h"
#include <assert.h>   /* for assert */
#include <iostream>
#include <cstring>
//#include "Motion_name.h"
// #include "StdAfx.h" 
#include "Motion_nameVector.h"


using namespace Leap;


class SampleListener : public Listener {
public:
	Vector normal    ;   // 
	Vector direction   ; //
	// 
	Vector position    ; //
	Vector Ori_postion ; //
	//
	Vector tool_direction    ; //
	Vector tool_Ori_postion ; //
	// 
	Vector finger_poistion    ; //
	Vector finger_speed   ; //
	//
	Vector pos_1 ; 
	Vector pos_2 ;
	Vector speed_cgw; 
	//
	Vector Left_position   ;
	Vector Right_position  ;

	float radius_hand_left  ; 
	float radius_hand_right ; 

	int   if_left_exist ;
	int   if_right_exist ;
	int   if_Grasped_Flag  ;

	int   if_Begin_Grasp_Flag     ; 
	int   if_Loss_Flag            ; 
	float  Ori_dist_between_hand  ;  
	float  Now_dist_between_hand  ; 
	
	float  Ori_radius_of_hand  ; 

	int    Two_hand_frame_counts  ; 
	int    One_hand_frame_counts  ;


	float Grasp_radius_Thre ;

	float myHandRoll     ;    /* Angle in radians eye rotates around knight. */
	float myHandRollago  ;    /* Angle in radians eye rotates around knight. */

	float myHandYaw    ;    /* Angle in radians eye rotates around knight. */
	float myHandYawago ;    /* Angle in radians eye rotates around knight. */ 

	int   HandNum      ;    //  

	float   myFrameKnob ; // 动画第几帧 

	Controller         controller2   ; 
	Motion_nameVector  Vector_obj    ; 

	SampleListener::SampleListener() ;
	SampleListener::SampleListener(SampleListener &self) ;

	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
	virtual void onDeviceChange(const Controller&);
	virtual void onServiceConnect(const Controller&);
	virtual void onServiceDisconnect(const Controller&);
private:

};
