//
//  HiViewer.h
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#pragma once

#include "HiKernel/HiObject.h"

#ifdef WIN32
#include <Windows.h>
#endif
//
//#ifndef __IPHONE_TOUCH__
//#include <Windows.h>
//#include "HiKernel/HiObject.h"
//#include "HiKernel/HiModuleManager.h"
//#include "HiKernel/HiRouteManager.h"
//#include "HiKernel/HiEventManager.h"
//#include "HiKernel/HiField.h"
//#else
//#include "HiKernel/HiObject.h"
//#include "HiKernel/HiFramework.h"
//#include "HiKernel/HiRenderer.h"
//


namespace HiKernel {
	
	class HiXmlNode;
	class HiRenderer;
	class HiXmlNode;
	
	#define HiViewerInstance HiViewer::getInstance()
	
	
	class HI_DLLEXPORT HiViewer : public HiObject
	{		
	private:
		static HiViewer*    ms_pViewer;
		std::string         m_strViewer;
		HiRenderer*         m_pRenderer;
		int					m_iScreen[4];
		//  
		//#ifdef __IPHONE_TOUCH__
		//    HiRouteManager*     route_manager_;
		//#endif
		
	public:
		~HiViewer();
		
#ifdef WIN32
		void Config(HWND hwnd, HINSTANCE hInst);			
#else
		void Config();
#endif
		bool Run();
		void Terminate();
		
		static HiViewer* getInstance(const std::string& filename="");
		
		HiRenderer* GetRenderer(){ return m_pRenderer; }
	public:	
		HiViewer(const std::string filename);
		
		int		ParseXml(HiXmlNode* xml);
		void	Sync();	
		std::string GetStrViewerName(){		return m_strViewer;		}
		
		int*	getScreen() { return m_iScreen; }
		

	};
	
}