//
//  HiViewer.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

//#include <string>
//#include <stack>
//#include <sstream>

#include "HiKernel/HiViewer.h"
#include "HiKernel/HiXmlNode.h"
#include "HiKernel/HiModuleManager.h"
#include "HiKernel/HiRouteManager.h"
#include "HiKernel/HiEventManager.h"
#include "HiKernel/HiRenderer.h"
#include "HiKernel/HiTelegram.h"

#include <list>


#if defined ( HI_OSG_RENDERER )
#include "HiKernel/HiOSGRenderer.h"
#endif

#if defined ( HI_GLES1_RENDERER )
#include "HiKernel/HiOpenGLES10Renderer.h"
#endif



using namespace HiKernel;

HiViewer* HiViewer::ms_pViewer = NULL;

HiViewer* HiViewer::getInstance(const std::string& filename)
{ 
	if ( ms_pViewer == NULL) 
		ms_pViewer = new HiViewer(filename); 
	return ms_pViewer;
}

HiViewer::HiViewer(const std::string filename) : m_pRenderer(NULL)
{
//#ifdef __IPHONE_TOUCH__
//    route_manager_ = new HiRouteManager();
//#endif
    
	HiXmlNode* xml = HiXmlNode::ParseFile(filename.c_str());

	m_strViewer = xml->GetAttrs("renderType");
	
#ifdef HI_IOS4
	
	setenv("OSG_SCREEN", xml->GetAttrs("osgScreen").c_str(),1);
	setenv("OSG_WINDOW", xml->GetAttrs("osgWindow").c_str(),1);
	setenv("OSG_THREADING", xml->GetAttrs("osgThreading").c_str(),1);

	xml->GetAttrs("osgWindow",4,m_iScreen);
	 
#endif
	
	
	
	if (xml)
		ParseXml(xml);
}

HiViewer::~HiViewer()
{
}

#ifdef WIN32
void HiViewer::Config(HWND hWnd, HINSTANCE hInst)
#else
void HiViewer::Config()
#endif
{
	ModuleManager->PreConfigAll();
	
#if defined ( HI_GLES1_RENDERER )
	m_pRenderer = new HiOpenGLES10Renderer();
#endif	

#if defined ( HI_OSG_RENDERER )
	m_pRenderer = HiOSGRenderer::getInstance();
#endif

#if defined ( HI_GB_RENDERER )
	m_pRenderer = new HiGBRenderer();
#endif

#if defined ( HI_DX_RENDERER )
	m_pRenderer = new HiDXRenderer();
#endif


#ifdef WIN32
	m_pRenderer->Initialize(hWnd, hInst);
#else
	m_pRenderer->Initialize();
#endif

	ModuleManager->PostConfigAll();

}

bool HiViewer::Run()
{
#ifdef WIN32
	Dispatch->DispatchDelayedMessages();
#endif

//	ms_pModuleManager->ProcessAll();

	ModuleManager->UpdateAll();

	m_pRenderer->BeginScene();
	ModuleManager->RenderAll();
	m_pRenderer->EndScene();

	// updateall , renderall ?ë≈?¬∫?¬°¬ß
	/*
	std::vector<HiModule*> modulelist = ModuleManager->GetModuleList();	
	for(std::vector<HiModule*> ::iterator it = modulelist.begin(); it != modulelist.end(); it++)
		if((*it)->GetActive() == true)
			(*it)->Update();

	m_pRenderer->BeginScene();

	for(std::vector<HiModule*> ::iterator it = modulelist.begin(); it != modulelist.end(); it++)
		if((*it)->GetActive() == true)
			(*it)->Render();

	m_pRenderer->EndScene();*/
	return m_pRenderer->IsDone();
}


void HiViewer::Terminate()
{
	ModuleManager->TerminateAll();
//	m_pRenderer->Terminate();
#if defined ( HI_GLES1_RENDERER )
	m_pRenderer->Terminate();
#endif
	
//	delete m_pRenderer;
}

void HiViewer::Sync()
{
}

int HiViewer::ParseXml(HiXmlNode* xml)
{
	if(xml == NULL)
		return 0;
	std::list<HiXmlNode*> list;
	std::vector<HiXmlNode*> route;
	std::vector<HiXmlNode*> module;
	list.push_back(xml);

	while(!list.empty())
	{
		HiXmlNode* node = list.front();
		list.pop_front();
		
		if ( node->GetName() == "HiModule" ) 
		{
			module.push_back(node);
		} 
		else if ( node->GetName() == "HiRoute" ) 
		{
			route.push_back(node);
		}
		else 
		{
			for(int i=0;i<node->GetNumChildren();i++)
				list.push_back( node->GetChild(i) );
		}
	}

	for(int i=0; i<module.size(); i++)
		ModuleManager->Create( module[i] );

	for(int i=0; i<route.size(); i++)
		RouteManager->Create( route[i] );
//#else
//    for(int i=0; i<route.size(); i++)
//		route_manager_->Create( route[i] );
//#endif
	
	return 1;
}
