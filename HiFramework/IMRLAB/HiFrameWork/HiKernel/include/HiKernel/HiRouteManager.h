//
//  HiRouteManager.h
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//


#pragma once
#include "HiObject.h"


namespace HiKernel {
	
	class HiField;
	class HiXmlNode;
	
	struct HiRouteTable
	{
		std::string		m_strID;
		HiField			*m_pSrc;
		HiField			*m_pDst;
		
		bool			m_bSuspend;
		
		HiRouteTable(std::string strID, HiField	*pSrc, HiField	*pDst)
		{
			m_strID = strID;
			m_pSrc = pSrc;
			m_pDst = pDst;
			m_bSuspend = false;
		}
	};
	
	
	class HI_DLLEXPORT HiRouteManager : public HiObject
	{
	private:
		std::vector<HiRouteTable*>			m_vRouteTable;
	public:
		HiRouteManager();
		~HiRouteManager();
		
		bool							CheckRouteTable(std::string strID, HiField *pSrc, HiField *pDst);
		//
		int								Create(HiXmlNode *xml);
		//
		bool							AddRoute(std::string strId, HiField *pSrc, HiField *pDst);
		//	bool							RemoveRoute(std::string strId);
		//	//bool							SuspendRoute(std::string strId);
		//	//bool							ResumeRoute(std::string strId);
		//
		//	bool							HandleMessage(const HiTelegram& msg);
		
		static	HiRouteManager			*Instance();
	};
	
	#define RouteManager HiRouteManager::Instance()

}