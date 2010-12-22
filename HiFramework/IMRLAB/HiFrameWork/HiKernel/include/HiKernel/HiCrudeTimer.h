/*!
 * HiCrudeTimer.h
 * 
 * Copyright (c) 2005 by 황대웅
 */

#pragma once
//
//#ifndef __IPHONE_TOUCH__
//#pragma comment(lib, "winmm.lib")

#ifdef WIN32
#include<windows.h>
#endif




#define Clock CrudeTimer::Instance()

/*!
 * \brief
 * CrudeTimer
 * 
 * Message사용때 시간 관리하는 Class.
 * 
 * \remarks
 * Write remarks for CrudeTimer here.
 * 
 * \see
 * Separate items with the '|' character.
 */

//
#ifdef WIN32
class CrudeTimer
{
private:
	//set to the time (in seconds) when class is instantiated
	double m_dStartTime;

	//set the start time
	CrudeTimer(){m_dStartTime = timeGetTime() * 0.001;}

	//copy ctor and assignment should be private
	CrudeTimer(const CrudeTimer&);
	CrudeTimer& operator=(const CrudeTimer&);

public:

	static CrudeTimer* Instance();

	//returns how much time has elapsed since the timer was started
	double GetCurrentTime(){return timeGetTime() * 0.001 - m_dStartTime;}

};

#endif