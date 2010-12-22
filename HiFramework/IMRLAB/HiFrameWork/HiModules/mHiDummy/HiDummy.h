#pragma once

#include "HiKernel/HiModule.h"
//#include "HiKernel/HiTelegram.h"


namespace HiModules {
	
	class HiKernel::HiTelegram;
	
	class HiKernel::HiXmlNode;
	
	class HI_DLLEXPORT mHiDummy : public HiKernel::HiModule
	{
	protected:
	public:
		mHiDummy(HiKernel::HiXmlNode*xml=NULL);                                   
		virtual ~mHiDummy();     
		
		void PostConfig();
		void Update();
		void Terminate();   
		//	void Realize();
		//
		bool HandleMessage(const HiKernel::HiTelegram& msg);
		//
		//	virtual int ParseXml(HiXmlNode*xml) {return 1;};
		//	virtual int SetXmlAttrs(HiXmlNode*xml)                  { return 1; }
		
	};
}
