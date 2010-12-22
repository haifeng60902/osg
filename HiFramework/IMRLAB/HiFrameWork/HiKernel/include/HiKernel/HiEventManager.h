//
//  HiEventManager.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//


#pragma once

#include "HiObject.h"
#include <set>

namespace HiKernel {
	
	
	
	#define Dispatch HiEventManager::Instance()
	
	class HiModule;
	class HiTelegram;
	
	
	const double SEND_MSG_IMMEDIATELY = 0.0f;
	const int   NO_ADDITIONAL_INFO   = 0;
	
	
	class HI_DLLEXPORT HiEventManager  : public HiObject
	{
	private:  
		
		std::set<HiTelegram> PriorityQ;
		void Discharge(HiModule* pReceiver, const HiTelegram& msg);
		
		HiEventManager();
		
		HiEventManager(const HiEventManager&);
		HiEventManager& operator=(const HiEventManager&);
		
	public:
				
		static HiEventManager* Instance();
		
		void GiveMessage(double  delay,
							 std::string    sender,
							 std::string    receiver,
							 int    msg,
							 void*  ExtraInfo);
		
		
#ifdef WIN32
		void DispatchDelayedMessages();
#endif
	};
}