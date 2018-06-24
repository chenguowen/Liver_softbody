#include "controler.h"

SampleListener::SampleListener()
{
	Vector_obj.Initlize();        // Create a sample listener and controller
	controller2.addListener( *this );
}


SampleListener::SampleListener(SampleListener &self)
{
	Vector_obj.Initlize();        // Create a sample listener and controller
	controller2.addListener(self); // Have the sample listener receive events from the controller
}

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
	const Frame frame = controller.frame(); 
	HandNum = frame.hands().count() ;

	if ( HandNum ==2) 
	{ //   �ֵ�����  // 
		myFrameKnob =182 ;
	}

	HandList hands = frame.hands(); //   �����ж�����һֻ��  //  
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {  
		// Get the first hand
		const Hand hand = *hl  ; //  //std::string handType = hand.isLeft() ? "Left hand" : "Right hand"
		// Vector
		normal    = hand.palmNormal(); //std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
		direction = hand.direction ();  // << ", palm position: " << hand.palmPosition() << std::endl;
		position  = hand.palmPosition(); // Get the hand's normal vector and direction



		// std::cout<< "[x,y,z]=["<<this->position.x<<","<<this->position.y<<","<<this->position.z<<"]"<<std::endl ; 
		//std::cout<< "normal [x,y,z]=["<<this->normal.x<<","<<this->normal.y<<","<<this->normal.z<<"]"<<std::endl ; 
		//std::cout<< "direction [x,y,z]=["<<this->direction.x<<","<<this->direction.y<<","<<this->direction.z<<"]"<<std::endl ; 
		////  Calculate the hand's pitch, roll, and yaw angles
	/*	std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
			<< "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
			<< "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl; */
		//Translation_Matrix[0+4*0] = cos(direction.yaw()) ;
		//Translation_Matrix[2+4*0] = sin(-direction.yaw());
		////Translation_Matrix[0+4*1] = ;
		////Translation_Matrix[2+4*1] = ;
		//Translation_Matrix[0+4*2] = sin(direction.yaw()) ;
		//Translation_Matrix[2+4*2] = cos(direction.yaw());

		myHandRollago =  myHandRoll      ;
		myHandRoll    =  normal.roll()   ;
		myHandYawago  =  myHandYaw       ; 
		myHandYaw     =  direction.yaw() ; 

		// Get the Arm bone
		Arm arm = hand.arm();
		//std::cout << std::string(2, ' ') <<  "Arm direction: " << arm.direction()
		//	<< " wrist position: " << arm.wristPosition()
		//	<< " elbow position: " << arm.elbowPosition() << std::endl;

		// Get fingers
		const FingerList fingers = hand.fingers();
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;
			//std::cout << std::string(4, ' ') <<  fingerNames[finger.type()]
			//<< " finger, id: " << finger.id()
			//	<< ", length: " << finger.length()
			//	<< "mm, width: " << finger.width() << std::endl;

			// Get finger bones
			for (int b = 0; b < 4; ++b) {
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
				//std::cout << std::string(6, ' ') <<  boneNames[boneType]
				//<< " bone, start: " << bone.prevJoint()
				//	<< ", end: " << bone.nextJoint()
				//	<< ", direction: " << bone.direction() << std::endl;
			}
		}
	}

	// Get tools
	const ToolList tools = frame.tools();
	for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
		const Tool tool = *tl;
		//std::cout << std::string(2, ' ') <<  "Tool, id: " << tool.id()
		//	<< ", position: " << tool.tipPosition()
		//	<< ", direction: " << tool.direction() << std::endl;
	}

	// Get gestures
	const GestureList gestures = frame.gestures(); 
	for (int g = 0; g < gestures.count(); ++g) {
		Gesture gesture = gestures[g];

		switch (gesture.type()) {
		case Gesture::TYPE_CIRCLE:
			{
				CircleGesture circle = gesture;
				std::string clockwiseness;

				if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
					clockwiseness = "clockwise";
				} else {
					clockwiseness = "counterclockwise";
				}

				// Calculate angle swept since last frame
				float sweptAngle = 0;
				if (circle.state() != Gesture::STATE_START) {
					CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
					sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI; 
					//  sweptAngle; ��ת�Ƕ�  
					//Translation_Matrix[0+4*0] = cos(sweptAngle) ;
					//Translation_Matrix[2+4*0] = sin(sweptAngle);
					////Translation_Matrix[0+4*1] = ;
					////Translation_Matrix[2+4*1] = ;
					//Translation_Matrix[0+4*2] = sin(-sweptAngle) ;
					//Translation_Matrix[2+4*2] = cos(sweptAngle);
				}
				/*	std::cout << std::string(2, ' ')
				<< "Circle id: " << gesture.id()
				<< ", state: " << stateNames[gesture.state()]
				<< ", progress: " << circle.progress()
				<< ", radius: " << circle.radius()
				<< ", angle " << sweptAngle * RAD_TO_DEG
				<<  ", " << clockwiseness << std::endl;*/
				break;
			}
		case Gesture::TYPE_SWIPE:
			{
				SwipeGesture swipe = gesture;
				//std::cout << std::string(2, ' ')
				//	<< "Swipe id: " << gesture.id()
				//	<< ", state: " << stateNames[gesture.state()]
				//<< ", direction: " << swipe.direction()
				//	<< ", speed: " << swipe.speed() << std::endl;
				break;
			}
		case Gesture::TYPE_KEY_TAP:
			{
				KeyTapGesture tap = gesture;
				/*	std::cout << std::string(2, ' ')
				<< "Key Tap id: " << gesture.id()
				<< ", state: " << stateNames[gesture.state()]
				<< ", position: " << tap.position()
				<< ", direction: " << tap.direction()<< std::endl;*/
				break;
			}
		case Gesture::TYPE_SCREEN_TAP:
			{
				ScreenTapGesture screentap = gesture;
				/*std::cout << std::string(2, ' ')
				<< "Screen Tap id: " << gesture.id()
				<< ", state: " << stateNames[gesture.state()]
				<< ", position: " << screentap.position()
				<< ", direction: " << screentap.direction()<< std::endl;*/
				break;
			}
		default:
			std::cout << std::string(2, ' ')  << "Unknown gesture type." << std::endl;
			break;
		}
	}

	if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
		// std::cout << std::endl; 
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

	for (int i = 0; i < devices.count(); ++i) {
		//std::cout << "id: " << devices[i].toString() << std::endl;
		//std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void SampleListener::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}
