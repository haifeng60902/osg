//
//  HiObject.h
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#pragma once

#include <vector>
#include <string>
#include "HiDLL.h"

namespace HiKernel {
	
	class HI_DLLEXPORT HiObject
	{
	public:
		HiObject(const std::string _name="NULL_NAME");
		virtual ~HiObject();
		
		static HiObject* Find(std::vector<HiObject*>& list, const std::string& _name);
		static HiObject* Find(std::vector<HiObject*>& list, int id);
		
		const std::string GetName();
		void SetName(const std::string& _name);
		
		int GetID();
		
		// virtual bool HandleMessage(const HiTelegram& msg); // 모듈 사이에서만 교환 가능하도록 ...
	protected:
		// void SetIndex(int i){ m_iIndex = i; }  // chpark : 중복되는 값이 나올수 있음 
		
	private:
		static int m_siIndex;
		std::string m_sName;
		int m_iIndex;

	};
	
}