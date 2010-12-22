//
//  HiTelegram.h
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#pragma once

#include<iostream>
#include<math.h>

#include "HiObject.h"


namespace HiKernel {
	
	class HiTelegram : public HiObject
	{
	public:
		//the entity that sent this HiTelegram
		int          Sender;
		
		//the entity that is to receive this HiTelegram
		int          Receiver;
		
		//the message itself. These are all enumerated in the file
		//"MessageTypes.h"
		int          Msg;
		
		//messages can be dispatched immediately or delayed for a specified amount
		//of time. If a delay is necessary this field is stamped with the time 
		//the message should be dispatched.
		double       DispatchTime;
		
		//any additional information that may accompany the message
		void*        ExtraInfo;
		
		
		HiTelegram();

		HiTelegram(double time, int sender, int receiver, int msg, void* info = NULL);		
	};
	
	
	//these telegrams will be stored in a priority queue. Therefore the >
	//operator needs to be overloaded so that the PQ can sort the telegrams
	//by time priority. Note how the times must be smaller than
	//SmallestDelay apart before two Telegrams are considered unique.
	const double SmallestDelay = 0.25;
	inline bool operator==(const HiTelegram& t1, const HiTelegram& t2)
	{
		return ( fabs(t1.DispatchTime-t2.DispatchTime) < SmallestDelay) &&
		(t1.Sender == t2.Sender)        &&
		(t1.Receiver == t2.Receiver)    &&
		(t1.Msg == t2.Msg);
	}

	inline bool operator<(const HiTelegram& t1, const HiTelegram& t2)
	{
		if (t1 == t2) // chpark <<- ?„ì— ?ˆëŠ” ?°ì‚°?ë? ?¸ì¶œ?´ì•¼ ?¬ë°”ë¦? ?”ë²„ê·?ë°˜ë“œ???„ìš” 
		{
			return false;
		}
		
		else
		{
			return  (t1.DispatchTime < t2.DispatchTime);
		}
	}
	inline std::ostream& operator<<(std::ostream& os, const HiTelegram& t)
	{
		os << "time: " << t.DispatchTime << "  Sender: " << t.Sender
		<< "   Receiver: " << t.Receiver << "   Msg: " << t.Msg;
		
		return os;
	}
	//handy helper function for dereferencing the ExtraInfo field of the HiTelegram 
	//to the required type.
	template <class T>
	inline T DereferenceToType(void* p)
	{
		return *(T*)(p);
	}
}

