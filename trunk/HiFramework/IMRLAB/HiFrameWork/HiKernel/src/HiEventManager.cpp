//
//  HiEventManager.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#include "HiKernel/HiEventManager.h"
#include "HiKernel/HiTelegram.h"
#include "HiKernel/HiModule.h"
#include "HiKernel/HiModuleManager.h"
#include "HiKernel/HiRouteManager.h"
#include "HiKernel/HiCrudeTimer.h"

//#include<iostream>
//#include <string>
//
//using std::cout;
//
//using std::set;
//
//#ifdef TEXTOUTPUT
//#include <fstream>
//extern std::ofstream os;
//#define cout os
//#endif

using namespace HiKernel;

HiEventManager::HiEventManager() : HiObject("EventManager")
{
}

//------------------------------ Instance -------------------------------------

HiEventManager* HiEventManager::Instance()
{
	static HiEventManager instance;

	return &instance;
}



void HiEventManager::Discharge(HiModule* pReceiver,
								  const HiTelegram& msg)
{
	if (!pReceiver->HandleMessage(msg))
	{
		//HiTelegram could not be handled
	//	cout << "Message not handled";
	}
}

void HiEventManager::GiveMessage(double  delay,
										std::string sender,
										std::string receiver,
										int    msg,
										void*  ExtraInfo)
{
    
#ifdef HI_IOS4
	HiModule* pSender = NULL;
	HiModule* pReceiver = NULL;
    
	ModuleManager->FindModule(sender, &pSender);
	ModuleManager->FindModule(receiver, &pReceiver);
    
#else	
	HiObject *pSender = NULL;
	HiObject *pReceiver = NULL;

	pSender = ModuleManager->FindModule(sender);
	pReceiver = ModuleManager->FindModule(receiver);
	if ( receiver ==  ModuleManager->GetName() )
	{
		pReceiver = ModuleManager;
	}
	else if  ( receiver ==  RouteManager->GetName() ) 
	{
		pReceiver = RouteManager; 
	}
	else 
	{
		pReceiver = ModuleManager->FindModule(receiver);
	}
#endif

	if (pReceiver == NULL)
	{
		return;
	}

	HiTelegram telegram(0, pSender->GetID(), pReceiver->GetID(), msg, ExtraInfo);

	if (delay <= 0.0f)                                                        
	{
		Discharge(static_cast<HiModule*>(pReceiver), telegram);
	}
#ifdef WIN32
	//else calculate the time when the HiTelegram should be dispatched
	else
	{
		double CurrentTime = Clock->GetCurrentTime(); 

		telegram.DispatchTime = CurrentTime + delay;

		//and put it in the queue
		PriorityQ.insert(telegram);   

		/*cout << "\nDelayed HiTelegram from " << GetNameOfEntity(pSender->ID()) << " recorded at time " 
			<< Clock->GetCurrentTime() << " for " << GetNameOfEntity(pReceiver->ID())
			<< ". Msg is "<< MsgToStr(msg);*/
	}
#endif
}


//---------------------- DispatchDelayedMessages -------------------------
//
//  This function dispatches any telegrams with a timestamp that has
//  expired. Any dispatched telegrams are removed from the queue
//------------------------------------------------------------------------
#ifdef WIN32
void HiEventManager::DispatchDelayedMessages()
{
//	SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	//get current time
	double CurrentTime = Clock->GetCurrentTime();

	//now peek at the queue to see if any telegrams need dispatching.
	//remove all telegrams from the front of the queue that have gone
	//past their sell by date
	while( !PriorityQ.empty() &&
		(PriorityQ.begin()->DispatchTime < CurrentTime) && 
		(PriorityQ.begin()->DispatchTime > 0) )
	{
		//read the HiTelegram from the front of the queue
		const HiTelegram& HiTelegram = *PriorityQ.begin();

		//find the recipient
		HiModule* pReceiver = ModuleManager->FindModule(HiTelegram.Receiver);//EntityMgr->GetEntityFromID(HiTelegram.Receiver);

		/*cout << "\nQueued HiTelegram ready for dispatch: Sent to " 
			<< GetNameOfEntity(pReceiver->ID()) << ". Msg is " << MsgToStr(HiTelegram.Msg);*/

		//send the HiTelegram to the recipient
		Discharge(pReceiver, HiTelegram);

		//remove it from the queue
		PriorityQ.erase(PriorityQ.begin());
	}
}
#endif