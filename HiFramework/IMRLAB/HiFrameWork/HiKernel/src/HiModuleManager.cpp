//
//  HiModuleManager.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#include <string>

#include "HiKernel/HiModuleManager.h"
#include "HiKernel/HiXmlNode.h"
#include "HiKernel/HiModule.h"
#include "HiKernel/HiTelegram.h"

#include "HiModules/UsedModules.h"
#include "HiModules/enumerations.h"

#ifndef HI_IOS4
	#include <windows.h>
	#include <Winbase.h>
#endif

using namespace HiKernel;
using namespace HiModules;


HiModuleManager *HiModuleManager::Instance()
{
	static HiModuleManager Instance;

	return &Instance;
}

HiModuleManager::HiModuleManager() : HiObject("ModuleManager")
{
	m_vModules.clear();
}

HiModuleManager::~HiModuleManager()
{
	if(m_vModules.empty())
		return;

	for(int i=0; i<m_vModules.size(); i++)
	{
		if(m_vModules[i] != NULL)
		{
			delete m_vModules[i];
			m_vModules[i] = NULL;
		}
	}

	m_vModules.clear();

#ifndef HI_IOS4
	for(int i=0; i<m_vInstance.size(); i++)
	{
		FreeLibrary(m_vInstance[i]);
	}
	m_vInstance.clear();
#endif
}

void HiModuleManager::PreConfigAll()
{
	touched = false;
	for(int i=0; i<m_vModules.size(); i++)
		m_vModules[i]->PreConfig();
}

void HiModuleManager::PostConfigAll()
{
	for(int i=0; i<m_vModules.size(); i++)
		m_vModules[i]->PostConfig();
}

void HiModuleManager::UpdateAll()
{
	for(int i=0; i<m_vModules.size(); i++)
		if(m_vModules[i]->GetActive())
			m_vModules[i]->Update();
}
void HiModuleManager::RenderAll()
{
	for(int i=0; i<m_vModules.size(); i++)
		if(m_vModules[i]->GetActive())
			m_vModules[i]->Render();
}

void HiModuleManager::TerminateAll()
{
	for(int i=0; i<m_vModules.size(); i++)
		if(m_vModules[i]->GetActive())
			m_vModules[i]->Terminate();
}


bool HiModuleManager::DefHandleMessage(const HiTelegram& msg, HiModule* module)
{
	switch ( msg.Msg )
	{
	case MSG_ACTIVE:
		module->SetActive(true);
		break;
	case MSG_NONACTIVE:
		module->SetActive(false);
		break;
	default:
		return false;
	}

	return true;
}


bool HiModuleManager::HandleMessage(const HiTelegram& msg)
{

	HiModule *pSender = FindModule(msg.Sender);

	std::cout << pSender->GetName().c_str() << " ModuleManager " <<  msg.Msg << std::endl;

	Suspend(pSender->GetName());

	
	return true;
}

void HiModuleManager::Suspend(std::string strID)
{
	for(std::vector<HiModule*> ::iterator it = m_vModules.begin(); it<m_vModules.end(); it++)
	{
		if((*it)->GetName() == strID)
		{
			(*it)->SetActive(false);
		}
	}
}

void HiModuleManager::Resume(std::string strID)
{
	for(std::vector<HiModule*> ::iterator it = m_vModules.begin(); it<m_vModules.end(); it++)
	{
		if((*it)->GetName() == strID)
		{
			(*it)->SetActive(true);
		}
	}
}

   
HiModule* HiModuleManager::Load( const std::string& _className, const std::string& _dsoName, HiXmlNode* _xml  )
{
#ifdef HI_WIN32
	std::string strname;
	if ( _dsoName =="") 
	{
#if defined(WIN32)
    #ifdef _DEBUG
		strname = _className+"_d.dll";
    #else
		strname = _className +".dll";
    #endif
#endif
	}else{
		strname = _dsoName;
	}

	HINSTANCE instance = LoadLibrary(( LPCTSTR )strname.c_str());

    if ( instance == NULL ) 
    {
		printf("module %s is NULL\n", _className.c_str());
        return 0;
	}

    std::string strProcess = "HiLoadModule_" + _className;

	HiModule* (*pCreateModuleFunc)(HiXmlNode*) = (HiModule*(*)(HiXmlNode*))GetProcAddress(instance, strProcess.c_str() );

	m_vInstance.push_back(instance);

    if ( pCreateModuleFunc== NULL ) 
		return 0;
	else
	{
		return pCreateModuleFunc(_xml);
	}
#else
	HiModule* _pTempModule = NULL;
	
#ifdef HI_GLES1_RENDERER
	if(_className.compare("mHiOpenGLES10Demo") == 0)
	{
		_pTempModule = static_cast<HiModule*> ( new mHiOpenGLES10Demo(_xml));
		
	}
#endif
	 
	
#ifdef HI_IOS4
	if(_className.compare("mHiIPhoneInput") == 0)
	{
		_pTempModule = new mHiIPhoneInput(_xml);
		
	}
	else if(_className.compare("mHiOsgCamera") == 0)
	{
		_pTempModule = new mHiOsgCamera(_xml);
	}
	else if(_className.compare("mHiOsgLight") == 0)
	{
	//	_pTempModule = new mHiOsgLight(_xml);
	}
	else if(_className.compare("mHiOsgTerrain") == 0)
	{
	//	_pTempModule = new mHiOsgTerrain(_xml);
	}
	else if(_className.compare("mHiOsgCal") == 0)
	{
		_pTempModule = new mHiOsgCal(_xml);
	}
	else if(_className.compare("mHiOsgDummy") == 0)
	{
		_pTempModule = new mHiOsgDummy(_xml);
	}
	else if(_className.compare("mHiOsgSound") == 0)
	{
	//	_pTempModule = new mHiOsgSound(_xml);
	}
	else if(_className.compare("mHiImageObject") == 0)
	{
	//	_pTempModule = new mHiImageObject(_xml);
	}
	else if(_className.compare("mHiGameWorld") == 0)
	{
	//	_pTempModule = new mHiGameWorld(_xml);
	}

#endif
	
	return _pTempModule;
#endif
	
}


int HiModuleManager::AddModule(HiModule *pModule)
{
	if(pModule != NULL)
		m_vModules.push_back(pModule);
	else
		return 0;
	return 1;
}

HiModule *HiModuleManager::GetModule(int i)
{
	int num = 0;
	std::vector<HiModule *>::iterator it;
	for(it = m_vModules.begin(); it!=m_vModules.end(); it++)
	{
		num+=1;
		if(num == i)
			return *it;
	}
	return NULL;
}

HiModule *HiModuleManager::FindModule(HiModule *pModule)
{
	std::vector<HiModule *>::iterator it;
	for(it = m_vModules.begin(); it!=m_vModules.end(); it++)
	{
		if(*it == pModule)
			return *it;
	}
	return NULL;
}

HiModule *HiModuleManager::FindModule(const std::string &id)
{
	return (HiModule*)HiObject::Find( *((std::vector<HiObject*>*)&m_vModules), id);
}


HiModule* HiModuleManager::FindModule(int id)
{
	return (HiModule*)HiObject::Find( *((std::vector<HiObject*>*)&m_vModules), id);
}


void HiModuleManager::FindModule(const std::string &id, HiModule** __pModule)
{
	for (std::vector<HiModule*>::iterator _iter = m_vModules.begin();
		 _iter != m_vModules.end();
		 ++_iter)
	{
		if( (*_iter)->GetName().compare(id) == 0)
		{
			(*__pModule) = (*_iter);
			return;
		}
	}
}


// ?èÔ¨Å¬??ú¬µ??¬ø?��?è?�ü�?��?Create
int HiModuleManager::Create(HiXmlNode *xml)
{
	// AddModule
	std::string className = xml->GetAttrs("class");
	std::string dsoName = xml->GetAttrs("dso");

	HiModule* module = Load(className, dsoName, xml);

//	//	module->SetName(className);
//	//printf("setXmlModule |%s| |%s|\n", className.c_str(), dsoName.c_str());
//
	if ( module != NULL )
	{
		std::string name = xml->GetAttrs("id");
		std::string active = xml->GetAttrs("active");
		if(active == "true")
			module->SetActive(true);
		else
			module->SetActive(false);
		module->SetName(name);

		AddModule(module);
		return 1;
	}
	return 0;
}

