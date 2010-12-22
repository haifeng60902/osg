//
//  HiKernel.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//



#include "HiKernel/HiModule.h"
#include "HiKernel/HiXmlNode.h"
#include "HiKernel/HiTelegram.h"

using namespace HiKernel;


HiModule::HiModule() : HiContainer(this), m_bActive(false) { }

HiModule::~HiModule() {}          

void HiModule::PreConfig() {} ;
void HiModule::PostConfig() {} ;
void HiModule::Sync() {} ;                        
void HiModule::Update() {};
void HiModule::Render() {};
void HiModule::Terminate() {};	

int HiModule::SetXmlAttrs(HiXmlNode*xml){ return 1; }


int 
HiModule::ParseXml(HiXmlNode* _xmlRoot)
{
    if(_xmlRoot==NULL)
		return 0;
    
    SetXmlAttrs(_xmlRoot);
	
	int i, n = _xmlRoot->GetNumChildren();
    for(i=0;i<n;i++)  {
		HiXmlNode* xml = _xmlRoot->GetChild(i);
		SetXmlAttrs(xml);
    }
	
	
    return 1;
}



bool HiModule::HandleMessage(const HiTelegram &msg)
{
	return true;
}

void 
HiModule::SetActive(bool bActive)
{
	m_bActive = bActive;
}

bool 
HiModule::GetActive()
{
	return m_bActive;
}

