#include "leap/Leap.h"
#include <assert.h>   /* for assert */
#include <iostream>
#include <cstring>
//#include "Motion_name.h"
#include "Motion_nameVector.h"
using namespace Leap;

class SampleListener : public Listener {
public:
	Vector normal   ;
	Vector direction ;
	Vector position  ;

	float myHandRoll     ;    /* Angle in radians eye rotates around knight. */
	float myHandRollago  ;    /* Angle in radians eye rotates around knight. */

	float myHandYaw    ;    /* Angle in radians eye rotates around knight. */
	float myHandYawago ;    /* Angle in radians eye rotates around knight. */ 



	int     HandNum     ;    //  
	float   myFrameKnob ; // 动画第几帧 
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
