
//
//  HiContainer.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#include "HiKernel/HiContainer.h"
#include "HiKernel/HiField.h"

using namespace HiKernel;

HiContainer::HiContainer(HiContainer *parent)
{
	if(parent != NULL)
		parent->m_listContainer.push_back(this);
}

HiContainer::~HiContainer()
{
}

//void HiContainer::AddToParent(HiContainer *parent)
//{
//	if(parent != NULL)
//		parent->m_listContainer.push_back(this);
//}
//

void 
HiContainer::Realize()
{
	for(std::vector<HiContainer*>::iterator it = m_listContainer.begin(); it!=m_listContainer.end(); it++)
		(*it)->Realize();
}


HiField* HiContainer::AddField(HiField* pField)
{
	m_listField.push_back(pField);
	return pField;
}


HiField* HiContainer::LookupField(int id)
{
	return (HiField*)HiObject::Find( *((std::vector<HiObject*>*)&m_listField), id);
}

HiField* HiContainer::LookupField(const std::string strName)
{
	return (HiField*)HiObject::Find( *((std::vector<HiObject*>*)&m_listField), strName);
}

HiField* HiContainer::GetField(int id)
{
	HiField *pField = NULL;
	if((pField  = LookupField(id)) != NULL)
		return pField;
	
	for(int i=0; i<m_listContainer.size(); i++)
	{
		pField = m_listContainer[i]->GetField(id);
		if(pField != NULL)
			return pField;
	}
    
    return NULL;
}

HiField* HiContainer::GetField(const std::string strName)
{
	HiField *pField = NULL;
	if((pField = LookupField(strName)) != NULL)
		return pField;
	
	for(int i=0; i<m_listContainer.size(); i++)
	{
		pField = m_listContainer[i]->GetField(strName);
		if(pField != NULL)
			return pField;
	}
	
	return NULL;
}

//HiField* HiContainer::GetNthField(int i)
//{
//	return m_listField[i];
//}
//
//void HiContainer::SendEvent(short id)
//{	
//	register HiField* srcField = LookupField(id);
//	if ( !srcField  || srcField->GetDirty() == 0) 
//		return;
//
//	std::vector<HiField*>  outputs = srcField->GetOutputs();
//
//	if (!outputs.empty()) 
//	{
//		HiField *	dstField = NULL;
//		for (int i = 0, num = outputs.size(); i < num; i++) 
//		{
//			if ((dstField = outputs[i]) != NULL) 
//			{
//				dstField->SetInput(srcField);
//				dstField->Validate();
//				SendEvent(id);
//			}
//		}
//		srcField->SetDirty(0);
//	}	
//}
//void HiContainer::SendEvent(const std::string strName)
//{
//	register HiField* srcField = LookupField(strName);
//	if ( !srcField  || srcField->GetDirty() == 0) 
//		return; 
//
//	std::vector<HiField*>  outputs = srcField->GetOutputs();
//
//	if (!outputs.empty()) 
//	{
//		HiField *	dstField = NULL;
//		for (int i = 0, num = outputs.size(); i < num; i++) 
//		{
//			if ((dstField = outputs[i]) != NULL) 
//			{
//				dstField->SetInput(srcField);
//				dstField->Validate();
//				SendEvent(strName);
//			}
//		}
//	}	
//}
//
void HiContainer::SendEvent(HiField *pField)
{
	if ( !pField  || pField->GetDirty() == 0) 
		return;
	
	std::vector<HiField*> 	fieldList = pField->GetOutputs();
	
	if (!fieldList.empty()) 
	{
		HiField *	dstField = NULL;
		for (int i = 0, num = fieldList.size(); i < num; i++) 
		{
			if ((dstField = fieldList[i]) != NULL ) 
			{
				dstField->SetInput(pField);
				dstField->Validate();
				SendEvent(dstField);
			}
		}
	}
	pField->SetDirty(0);
}
//
//void HiContainer::SendAllEvent()
//{
//	for(int i=0;i<m_listField.size();i++)
//		SendEvent( GetNthField(i));
//}