//
//  HiModuleManager.h
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


namespace HiKernel {
	
	class HiModule;
	
	class HiXmlNode;
	
	class HiTelegram;

	#define ModuleManager HiModuleManager::Instance()
	
	class HI_DLLEXPORT HiModuleManager : public HiObject
	{
		
	private:
		std::vector<HiModule*>		m_vModules;
		
#ifdef WIN32
		std::vector<HINSTANCE>		m_vInstance;
#endif
		
	public:
		
		enum eGBCamera
		{
			MSG_ACTIVE,
			MSG_NONACTIVE
		};
		
		
		HiModuleManager();
		~HiModuleManager();
		
		HiModule* Load( const std::string& className, 
					   const std::string& dsoName="",
					   HiXmlNode*xml=NULL );
		
		void				PreConfigAll();
		void				PostConfigAll();
		
		void				UpdateAll();
		void				RenderAll();
		
		void				TerminateAll();
		
		int					Create(HiXmlNode* xml);
		
		int					AddModule(HiModule *pModule);
		HiModule*			GetModule(int i);
		HiModule*			FindModule(HiModule *pModule);
		void                FindModule(const std::string &id, HiModule** __pModule);

		HiModule*			FindModule(const std::string &id);
		HiModule*			FindModule(int id);
		
		void				Suspend(std::string strID);
		void				Resume(std::string strID);
		
		bool				HandleMessage(const HiTelegram& msg);
		bool				DefHandleMessage(const HiTelegram& msg, HiModule* module);
		
		static				HiModuleManager *Instance();
		
		std::vector<HiModule *>			GetModuleList()		{		return m_vModules;			}
		
	private:
		bool	touched;
	};
	
	
	
}