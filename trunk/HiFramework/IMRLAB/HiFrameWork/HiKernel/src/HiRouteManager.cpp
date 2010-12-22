//
//  HiRouteManager.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#include "HiKernel/HiRouteManager.h"
#include "HiKernel/HiField.h"
#include "HiKernel/HiXmlNode.h"
#include "HiKernel/HiModuleManager.h"
#include "HiKernel/HiModule.h"
#include "HiKernel/HiTelegram.h"


using namespace HiKernel;

HiRouteManager::HiRouteManager() : HiObject("RouteManager")
{
	// ?�◊¿Ã?��??� ±‚»≠
}

HiRouteManager::~HiRouteManager()
{
	// by rehomik
	for(std::vector<HiRouteTable*>::iterator _pTable = m_vRouteTable.begin();
		_pTable != m_vRouteTable.end();
		++_pTable)
	{
		delete (*_pTable);
	}
}


//bool HiRouteManager::HandleMessage(const HiTelegram& msg)
//{
//	HiModule *pSender = ModuleManager->FindModule(msg.Sender);
//
//	std::cout << pSender->GetName().c_str() << " ModuleManager " <<  msg.Msg << std::endl;
//
//
//	switch ( msg.Msg )
//	{
//	case 10: 
////		SuspendRoute("xyz->pos");
//		break;
//	case 11:
////		ResumeRoute("xyz->pos");
//		break;
//	}
//	return true;
//}
//
bool HiRouteManager::CheckRouteTable(std::string strID, HiField *pSrc, HiField *pDst)
{
	for(int i=0; i<m_vRouteTable.size(); i++)
	{
		if(strID == m_vRouteTable[i]->m_strID)
			return false;	
		if(m_vRouteTable[i]->m_pSrc == pSrc && m_vRouteTable[i]->m_pDst == pDst)
			return false;
	}
	return true;
}

bool HiRouteManager::AddRoute(std::string strId, HiField *pSrc, HiField *pDst)
{
	if(CheckRouteTable(strId, pSrc, pDst))
	{
		HiRouteTable *pRouteInfo = new HiRouteTable(strId, pSrc, pDst);
		m_vRouteTable.push_back(pRouteInfo);
	}

	pSrc->Connect(pDst);
	
	return false;
}

//bool HiRouteManager::RemoveRoute(std::string strId )
//{
//	for(std::vector<HiRouteTable*> ::iterator it = m_vRouteTable.begin(); it!= m_vRouteTable.end(); ++it)
//	{
//		if(strId == (*it)->m_strID)
//		{
//			(*it)->m_pSrc->DisConnect((*it)->m_pDst);
//			m_vRouteTable.erase(it);
//			return true;
//		}
//	}
//	// ?�◊¿Ã?�ÌµÓ∑œ ?�ÀªÁ
//	// ¡¶?�≈
//	// disconnect
//	return false;
//}
//
////bool HiRouteManager::SuspendRoute(std::string strId)
////{
////	for(std::vector<HiRouteTable*> ::iterator it = m_vRouteTable.begin(); it!= m_vRouteTable.end(); ++it)
////	{
////		if(strId == (*it)->m_strID)
////		{
////			(*it)->m_bSuspend = true;
////			return true;
////		}
////	}
////	return false;
////}
////
////bool HiRouteManager::ResumeRoute(std::string strId)
////{
////	for(std::vector<HiRouteTable*> ::iterator it = m_vRouteTable.begin(); it!= m_vRouteTable.end(); ++it)
////	{
////		if(strId == (*it)->m_strID)
////		{
////			(*it)->m_bSuspend = false;
////			return true;
////		}
////	}
////	return false;
////}
//
int HiRouteManager::Create(HiXmlNode *xml)
{
	std::string sID	  = xml->GetAttrs("id");
	std::string sSorc = xml->GetAttrs("sorc");
	std::string sDest = xml->GetAttrs("dest");


	int d1st, d2st;
	std::string sModule, sElement, sField;


	d1st = sSorc.find(":");
	d2st = sSorc.find(".");

	sModule = sSorc.substr(0,d1st);
	HiModule* moduleSorc = NULL;
    ModuleManager->FindModule(sModule, &moduleSorc);
	
	if ( moduleSorc == NULL ) 
	{
		printf("moduleSorc is NULL\n");
		return 0;
	}

	sField = sSorc.substr(d1st+1,sSorc.size()-d1st-1);
	HiField*  fieldSorc  = moduleSorc->GetField(sField);

	if ( fieldSorc == NULL ) 
	{
		printf("fieldSorc is NULL\n");
		return 0;
	}
	

	d1st = sDest.find(":");
	d2st = sDest.find(".");

	sModule = sDest.substr(0,d1st);
	HiModule* moduleDest = NULL;
    ModuleManager->FindModule(sModule, &moduleDest);
	
	if ( moduleDest == NULL )
	{
		printf("moduleDest is NULL\n");
		return 0;
	}
	
	sField = sDest.substr(d1st+1,sDest.size()-d1st-1);
	HiField*  fieldDest  = moduleDest->GetField(sField);

	if ( fieldDest == NULL ) 
	{
		printf("fieldDest is NULL\n");
		return 0;
	}

	//fieldSorc->Connect(fieldDest);

	// if ( CheckRoute(sID, fieldSorc, fieldDest) )
	// tagRouteInfo *pRouteInfo;
	// pRouteInfo = new ROUTE_INFO(strId,pSrc,pDst,false); --> ª?º?�¿⁄ »£?�‚Ω√ ±?�¡∏ ?�ÆΩ∫?�Æø�?¿÷¥¬¡? ?�ÀªÁ»? Null ?�«¥�?return.
	// RouteManager->Connect(pRouteInfo);



	AddRoute(sID, fieldSorc, fieldDest);

	float	offset[3], scale[3];
	int		order[3];

	if ( xml->GetAttrs("offset",3, offset ) ) 
		fieldDest->SetOffset(offset);

	if ( xml->GetAttrs("scale",3, scale ) )
		fieldDest->SetScale(scale);

	if ( xml->GetAttrs("order",3, order) )
		fieldDest->SetOrder(order);

	return 0;
}

HiRouteManager *HiRouteManager::Instance()
{
	static HiRouteManager instance;

	return &instance;
}