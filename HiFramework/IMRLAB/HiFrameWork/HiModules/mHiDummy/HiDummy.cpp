#include "HiModules/mHiDummy/HiDummy.h"
#include "HiKernel/HiXmlNode.h"
#include "HiKernel/HiTelegram.h"
#include "HiKernel/HiModuleManager.h"
#include "HiModules/enumerations.h"


namespace HiDummy 
{
	enum HiDummy 
	{
		FIELD_KEY
	};
}

using namespace HiModules;


mHiDummy::mHiDummy(HiKernel::HiXmlNode*xml)
{
}

mHiDummy::~mHiDummy()
{
}


void mHiDummy::Terminate()
{
//	m_pGBRender->m_spScene->DetachChild(m_pNode);
//	m_pNode = 0;
}
//
bool mHiDummy::HandleMessage(const HiKernel::HiTelegram& msg)
{
	HiModule *pSender = HiKernel::ModuleManager->FindModule(msg.Sender);

	std::cout << pSender->GetName().c_str() << " --- " <<  msg.Msg << std::endl;

	switch ( msg.Msg )
	{
	}

	return HiKernel::ModuleManager->DefHandleMessage(msg,this);
}



void
mHiDummy::PostConfig()
{
	//m_pGBRender = (HiGBRenderer*)HiViewer::GetInstance()->GetRenderer();
//
//	if ( m_pGBRender ->GetRenderType() != HiRenderer::Gamebryo )
//		exit(-1);
//
//	m_pNode = m_pGBRender->Load("WORLD.nif");
//	m_pGBRender->m_spScene->AttachChild(m_pNode);

	// chpark 090501: attach »? m_spScene ø°º??«?�≤®?¯ø° «ÿ¡÷¥¬ ??�µÂ∏¶ ?ø?�Œø�??�ﬂ?��?}
}
void mHiDummy::Update()
{
//	NiInputGamePad* pkGamePad = NULL; 
//	NiInputKeyboard* pkKeyboard = NULL;
//
//	if(m_pGBRender->m_spInputSystem )
//	{
//		if (m_pGBRender->m_spInputSystem->GetMode() == NiInputSystem::MAPPED)
//		{
//			if (m_pGBRender->m_spInputSystem->UpdateActionMap() == NIIERR_DEVICELOST)
//				m_pGBRender->m_spInputSystem->HandleDeviceChanges();
//		}
//		else
//		{
//			if (m_pGBRender->m_spInputSystem->UpdateAllDevices() == NIIERR_DEVICELOST)
//				m_pGBRender->m_spInputSystem->HandleDeviceChanges();
//		}
//	}    
//
//	D3DXVECTOR3 &m_vVector = m_vVec3f->get();
//	if (m_pGBRender->GetInputSystem())
//	{
//		pkKeyboard = m_pGBRender->GetInputSystem()->GetKeyboard();
//	}
//<<<<<<< .mine
//=======
//
//>>>>>>> .r35
//	if (pkKeyboard != NULL)
//	{
//		if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_DOWN))
//		{
//			m_vVector.x+=1.0f;
//		}
//		if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_LEFT))
//		{
//			m_vVector.z+=1.0f;
//		}
//		if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_RIGHT))
//		{
//			m_vVector.z-=1.0f;
//		}
//		if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_UP))
//		{
//			m_vVector.x-=1.0f;
//		}
//	}
//	m_vVec3f->SetDirty(1);
//	Realize();
}
//
//void mHiDummy::Realize()
//{
//	this->SendEvent(m_vVec3f);
//}
//

#ifndef HI_IOS4

extern "C" HI_DLLEXPORT
mHiDummy* HiLoadModule_mHiDummy(HiKernel::HiXmlNode* xml)
{
	mHiDummy *m= new mHiDummy(xml);
	return m;
}

#endif