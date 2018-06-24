// #include "StdAfx.h" 
#include "controler.h"

SampleListener::SampleListener()
{ // 
	Vector_obj.Initlize();        // Create a sample listener and controller
	controller2.addListener( *this ); 
	if_left_exist      = 0  ;
	if_right_exist     = 0  ; 
	Grasp_radius_Thre  = 60 ; // 抓取的半径阈值 
	if_Grasped_Flag    = 0  ; //  0代表空 1 代表右手 2 代表左手 3代表走右手同时  
	Ori_dist_between_hand = -100 ; // 
	Now_dist_between_hand = -100 ; // 
	Two_hand_frame_counts = -0   ; // 
	One_hand_frame_counts = -0   ; // 
	Ori_radius_of_hand    = -100 ; // 
	if_Loss_Flag          =  0   ; // w
	if_Begin_Grasp_Flag   =  0   ; // 最开始没有抓到
}


SampleListener::SampleListener(SampleListener &self)
{
	Vector_obj.Initlize() ;       // Create a sample listener and controller
	controller2.addListener(self); // Have the sample listener receive events from the controller 
	if_left_exist      = 0  ;
	if_right_exist     = 0  ;
	Grasp_radius_Thre  = 60 ;
	if_Grasped_Flag    = 0  ; //  //  0代表空 1 代表右手 2 代表左手 3代表走右手同时 
	Ori_dist_between_hand = -100 ; // 
	Now_dist_between_hand = -100 ; // 
	Two_hand_frame_counts = -0   ;
	Ori_radius_of_hand    = -100 ; // 
	if_Loss_Flag          =  0   ; // w
	if_Begin_Grasp_Flag   =  0   ; // 最开始没有抓到
}
//   //
void SampleListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
	// controller2.addListener(controller); // Have the sample listener receive events from the controller
}

void SampleListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
	std::cout << "Exited" << std::endl;
}


void SampleListener::onFrame(const Controller& controller) {
	

	const Frame frame = controller.frame(); // 
	
	//Leap::ToolList test = frame.tools()   ; //
	//Leap::Tool     tool = test.leftmost() ; //
	////tool_direction      = tool.direction()   ; // 
	////tool_Ori_postion    = tool.tipPosition() ; //  
	//// 
	//Tool frontTool = frame.tools().frontmost();
	//// Tool firstToolInList = frame.tools().get(0);
	//Pointable pointable        = frame.pointables().frontmost();
	//tool_direction      = pointable.direction()   ; // D
	//tool_Ori_postion    = pointable.tipPosition() ; // D
	//float length        = pointable.length() ;  // D

	//std::cout<<std::endl<<"length = "<<length<<std::endl ; 

	//if (pointable.isTool()) { 
	//	std::cout<<std::endl<<"tool tool  = "<<length<<std::endl ; 
	//	Tool* tool = new Tool(pointable);
	//} else 
	//{
	//	std::cout<<std::endl<<"hand hand  = "<<length<<std::endl ; 
	//	Finger* finger = new Finger(pointable);
	//}
	// 
	// tool. 
	
	HandNum = frame.hands().count()  ; // 
	//
	if ( HandNum == 2 )   //   手的数量  //  
	{   myFrameKnob =182 ; 	}

	HandList hands = frame.hands(); //   可以判断是哪一只手  //  
	
	Leap::Hand hand = frame.hands().rightmost() ; 


	speed_cgw = hand.palmVelocity(); // 
	if_left_exist  = 0 ;
	if_right_exist = 0 ;
	if_Grasped_Flag = 0; 

	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl ) {  

		const Hand hand = *hl          ;  //  Get the first hand
		normal    = hand.palmNormal()  ;  //  Get the hand's normal vector
		direction = hand.direction ()  ;  //  palm direction 
		// position  = hand.palmPosition();  //  palm position 
		position = hand.stabilizedPalmPosition() ;  //  palm position  
		// 
		Leap::Pointable pointable = frame.pointables().frontmost();

		// 
		// position = pointable.tipPosition();
		// 
		//  hand.stabilizedPalmPosition;  
		Vector position    ; //
		Vector Ori_postion ; //  

	
		// Leap::Vector stabilizedPosition = pointable.stabilizedTipPosition();
		finger_poistion = pointable.tipPosition();
		finger_speed    = pointable.tipVelocity();
		// 
		if( hand.isLeft() )
		{
			if_left_exist = 1 ;
			Left_position = position  ;
			radius_hand_left = hand.sphereRadius();  
		}
		if( hand.isRight() )
		{
			if_right_exist =   1       ; // 
			Right_position = position  ;
			radius_hand_right = hand.sphereRadius();  
		}

		myHandRollago =  myHandRoll      ;
		myHandRoll    =  normal.roll()   ;
		myHandYawago  =  myHandYaw       ; 
		myHandYaw     =  direction.yaw() ; 
	}

	if( (if_right_exist && radius_hand_right< Grasp_radius_Thre) ) 
	{
		if_Grasped_Flag = 1 ;// 任何一个手抓到了都算抓到了 
	}


	if( ( if_left_exist && radius_hand_left< Grasp_radius_Thre)   )
	{
		if_Grasped_Flag = if_Grasped_Flag +  2 ; //  0代表空 1 代表右手 2 代表左手 3代表走右手同时 
	}
	// 
	if( if_Grasped_Flag == 3 ) 
	{
		Two_hand_frame_counts = Two_hand_frame_counts  + 1 ;  
		std::cout<< "<GET" << std::endl; 
		Now_dist_between_hand =     Right_position.distanceTo(Left_position)  ; 

		if(  Two_hand_frame_counts == 1  )
		{
			Ori_dist_between_hand  =  Now_dist_between_hand    ;
		}
		
	}
	else
	{
		Two_hand_frame_counts = 0 ; // 如果只有一只手 清零 
	}
	///system("pause") ; 
	// 
	if ( (  if_right_exist && radius_hand_right < Grasp_radius_Thre)  ) // 右手抓到了   
	{
		One_hand_frame_counts = One_hand_frame_counts + 1 ;
		if(  abs( One_hand_frame_counts - 1 ) < 0.01  )
		{
			if_Begin_Grasp_Flag    =  1 ; 
			Ori_dist_between_hand  =  Now_dist_between_hand    ;
			Ori_radius_of_hand     =  radius_hand_right ;  
			Ori_postion            =  Right_position    ;  
			// pos_1 = Right_position ;  
			pos_2 = position;
			//  speed_cgw = (pos_2 - pos_1) * 600.0 * 1000 ;    
			// system("pause");  
			//std::cout << std::endl;
			//std::cout << "speed_cgw.x = " << speed_cgw.x << " " << speed_cgw.y << " " << speed_cgw.x << std::endl; 
		} 
		else
		{   // 
			pos_2     = position; // 
				//speed_cgw = (  pos_2 - pos_1  ) * 60.0   ; // 
			pos_1       = pos_2 ;  //  
				//std::cout << std::endl;
			//std::cout << "speed_cgw.x = " << speed_cgw.x << " " << speed_cgw.y << " " << speed_cgw.x << std::endl; 
		}
		if_Loss_Flag = 0  ;  
	}
	else // ********  右手没抓到 ******** // 
	{
		if( One_hand_frame_counts > 0.1 ) // 如果这时候我失去了控制权  
		{
			if_Loss_Flag          =  1    ; // 声明我失去了控制权 
			One_hand_frame_counts =  0    ; // 
			Ori_radius_of_hand    = -100  ; //  
			pos_1 = pos_2;  
			// speed_cgw = (pos_2 - pos_1) * 60.0;
		}
		else
		{
			One_hand_frame_counts =  0    ; //  
			// if_Loss_Flag          =  1    ; //    这个里面存在帧率不统一 而 if_Loss_Flag是否该一瞬间为1  
		} // 
	}


}

void SampleListener::onFocusGained(const Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
	std::cout << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();
	for (int i = 0; i < devices.count(); ++i) { }
}

void SampleListener::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}
