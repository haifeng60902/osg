#include "HiKernel/HiFramework.h"
#include "HiModules/mHiCal3dTest/mHiCal3dTest.h"
#include "HiKernel/HiXmlNode.h"
#include "HiKernel/HiTelegram.h"
#include "HiKernel/HiOSGRenderer.h"
//#include "HiModules/MessageState.h"

#include <time.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace HiModules;

mHiCal3dTest::mHiCal3dTest(HiKernel::HiXmlNode *xml) : 
m_pRoot(new osg::Group),
m_pCalGroup(new osg::Group),
m_ModelCount(0)
{
	if(xml != NULL)
		ParseXml(xml);	

}
mHiCal3dTest::~mHiCal3dTest()
{

}
int mHiCal3dTest::ParseXml(HiKernel::HiXmlNode *xml)
{

	HiKernel::HiXmlNode* node = xml->FindChild("core");

	for(int i=0;i<node->GetNumChildren();i++) {
		HiKernel::HiXmlNode* child = node->GetChild(i);
		m_cfgFile = child->GetAttrs("cfg");

		m_calModel.push_back(new s_CalCoreModel);
		m_calModel[i]->m_CordId = child->GetAttrs("id");
		m_calModel[i]->m_CoreIndex = i;
#if defined (HI_IOS4)
		m_calModel[i]->m_p->software = true;
		
		NSString * temp = [[NSString alloc] initWithCString:m_cfgFile.c_str() encoding:NSASCIIStringEncoding]; 
	
		NSString * path = [[NSBundle mainBundle] pathForResource: temp  ofType: @"cfg"];
		FILE *f = fopen([path cStringUsingEncoding:1],"r");
				
		m_calModel[i]->m_CoreModel->load( [path cStringUsingEncoding:1], m_calModel[i]->m_p.get());
#else
		m_calModel[i]->m_CoreModel->load(m_cfgFile, m_calModel[i]->m_p.get());
#endif
		m_calModel[i]->animationNames = m_calModel[i]->m_CoreModel->getAnimationNames();

		m_ModelCount++;
	}

	node = xml->FindChild("inst");
	std::string tempCoreId;
	float tempxyz[3];
	for(int i=0;i<node->GetNumChildren();i++)
	{
		m_calChildModel.push_back(new s_CalInstanceModel);
		HiKernel::HiXmlNode* child = node->GetChild(i);
		tempCoreId = child->GetAttrs("coreid");
		child->GetAttrs("trans", 3, tempxyz);
		for(UINT j=0;j<m_calModel.size();j++)
		{
			//addModel(0, tempxyz[0], tempxyz[1], tempxyz[2]);
			if(m_calModel[j]->m_CordId == tempCoreId)
			{
				m_calChildModel[i]->m_index = j;
				m_calChildModel[i]->m_Pos.set(tempxyz[0], tempxyz[1], tempxyz[2]);
			}
		}
	}


	return 0;

}


bool mHiCal3dTest::HandleMessage(const HiKernel::HiTelegram &msg)
{
	//HiModule *pSender = ModuleManager->FindModule(msg.Sender);

	//switch ( msg.Msg )
	//{

	//}

	//return ModuleManager->DefHandleMessage(msg,this);

	return true;

}

void mHiCal3dTest::PostConfig()
{

	m_pRoot = HiKernel::OSGRenderer->getRoot();

	for(UINT i=0;i<m_calChildModel.size();i++)
	{
		//xml에서 받아온 모델과 좌표값으로 캐릭터를 생성해준다.
		addModel(m_calChildModel[i]->m_index, m_calChildModel[i]->m_Pos.x(), m_calChildModel[i]->m_Pos.y(), m_calChildModel[i]->m_Pos.z());
	}
	m_pCalGroup->setName("CalGroup");

	m_pRoot->addChild(m_pCalGroup);

	HiKernel::OSGRenderer->getViewer()->setCameraManipulator(new osgGA::TrackballManipulator());

	
}
void mHiCal3dTest::Update()
{

 //	float a = m_calChildModel[0]->m_Model->getCalModel()->getMixer()->getAnimationTime();

	//static float Time = 2.6f; 

	//m_calChildModel[0]->m_Model->getCalModel()->getMixer()->setAnimationTime(Time);

	//Time = Time - 0.01;

	//if(Time < 1.0f)
	//{
	//	Time = 2.6f;
	//}

	//std::cout << a << std::endl;


}
osgCal::Model* mHiCal3dTest::makeModel(osgCal::CoreModel *cm, osgCal::BasicMeshAdder *ma, int animNum)
{
	osgCal::Model* model = new osgCal::Model();

	model->load( cm, ma );

	model->blendCycle( 0, 1.0f, 0 );
	//if ( animNum != -1 )
	//{
	//	model->blendCycle( animNum, 1.0f, 0 );
	//}

	return model;	
}
bool mHiCal3dTest::addModel(int _id, float _x, float _y, float _z)
{
	static int Count=0;
	osg::Vec3 _pos;
	_pos.set(_x, _y, _z);		
	m_calChildModel[Count]->m_Model = makeModel(m_calModel[_id]->m_CoreModel.get(),
		m_calChildModel[Count]->m_meshAdder.get(),
		-1 );

	m_calChildModel[Count]->m_modelPos->setPosition(_pos);
	if( !m_calModel[_id])
	{
		std::cout << "현재 없는 캐릭터 입니다. " << std::endl;
		return false;
	}

	//캐릭터의 이동정보를 단다
	m_pCalGroup->addChild( m_calChildModel[Count]->m_modelPos );

	//이동정보에 회전정보달고 회전정보에 모델을 단다.
	m_calChildModel[Count]->m_modelPos->addChild( m_calChildModel[Count]->m_rotMat );
	m_calChildModel[Count]->m_rotMat->addChild( m_calChildModel[Count]->m_Model );

	Count++;
	return true;
}


extern "C" 
HI_DLLEXPORT mHiCal3dTest* HiLoadModule_mHiCal3dTest(HiKernel::HiXmlNode* xml)
{
	mHiCal3dTest *m = new mHiCal3dTest(xml);
	return m;
}