//
//  HiContainer.h
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#pragma once

#include "HiObject.h"


namespace HiKernel {
	
	
	class HiField;

	class HI_DLLEXPORT HiContainer : public HiObject
	{
		friend class HiField;
		
	private:
		static unsigned int			m_uiUpdateMask[32];
		unsigned int				m_uiUpdateFlags;
		std::vector<HiContainer*>	m_listContainer;
		std::vector<HiField*>		m_listField;
		
	protected:
		HiField*				AddField(HiField *pField);
		
		
	public:
		HiContainer(HiContainer *parent = NULL);
		virtual ~HiContainer();
		
		
		//	void	AddToParent(HiContainer *parent);
		//
		virtual		void		Realize();
		//
		
		HiField*				LookupField(int id);
		HiField*				LookupField(const std::string strName);
		
		HiField*				GetField(int id);
		HiField*				GetField(const std::string strName);
		//	HiField*				GetNthField(int i);
		//
		//	void					SendEvent(short id);
		//	void					SendEvent(const std::string strName);
		void					SendEvent(HiField *pField);
		//	void					SendAllEvent();
		
		
	};
}
