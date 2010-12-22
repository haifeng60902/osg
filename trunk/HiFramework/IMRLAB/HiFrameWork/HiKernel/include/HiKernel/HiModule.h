//
//  HiKernel.h
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#pragma once

#include "HiContainer.h"


// 모듈마다 서로 다른 갱신률을 가질 수 있도록 수정 - Time Slice or Thread 도입...
// 수행중에 모듈을 루프에서 대기,  삭제 (추가?) 할수 있도록 수정


// - Plan 
// 1. cal3d 모듈
// 2. lua 모듈
// 3. FSM 모듈	
// 4. Steering Behavior 모듈
// 5. A* 모듈


namespace HiKernel {

	class HiXmlNode;
	
	class HiTelegram;
	
	class HI_DLLEXPORT HiModule : public HiContainer
	{
	private:
		
	protected:
		HiXmlNode *m_pXml;
		bool m_bActive;
		
	public:
		HiModule();
		virtual ~HiModule();          
		
		virtual void PreConfig();
		virtual void PostConfig();
		virtual void Sync();                        
		virtual void Update();
		virtual void Render();
		virtual void Terminate();		
		virtual bool HandleMessage(const HiTelegram& msg);
		
		
		// void SetModuleID(int iID);   
		//virtual void SetDev(const LPDIRECT3DDEVICE9 &pDev) {};           
		//inline int GetModuleID() { return m_iModuleID; }           
		
		
		virtual int ParseXml(HiXmlNode*xml);
		virtual int SetXmlAttrs(HiXmlNode*xml);
		//void construct();
		
		void SetActive(bool bActive);
		bool GetActive();
	};
}