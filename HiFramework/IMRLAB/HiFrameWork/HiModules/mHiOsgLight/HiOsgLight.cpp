 /*
 *  HiOsgCal.cpp
 *  iOSG
 *
 *  Created by whitefir0 on 09. 06. 16.
 *  Copyright 2009 IMRLAB. All rights reserved.
 *
 */

#include "HiModules/mHiOsgLight/HiOsgLight.h"
#include "HiModules/enumerations.h"
#include "HiKernel/HiOSGRenderer.h"

#include "HiKernel/HiTelegram.h"
//
//#include <osgViewer/Viewer>
//#include <osg/StateAttribute>
//#include <osg/Geometry>
//#include <osg/Point>
//#include <osg/MatrixTransform>
//#include <osgDB/Registry>
//#include <osgDB/ReadFile>
//#include <osgUtil/Optimizer>
//#include <osgUtil/SmoothingVisitor>

using namespace HiModules;


mHiOsgLight::mHiOsgLight(HiKernel::HiXmlNode* __pXml)
{
	if(__pXml != NULL)
		ParseXml(__pXml);
	
	m_LightPosY = 0.0f;
	m_LightCheck=true;
}

mHiOsgLight::~mHiOsgLight()
{
}
int mHiOsgLight::ParseXml(HiKernel::HiXmlNode *xml)
{

	return 1;
}

bool mHiOsgLight::HandleMessage(const HiKernel::HiTelegram& msg)
{
	if(MSG_SIZE <= msg.Msg)
		return false;
    

	return true;
}

void mHiOsgLight::PreConfig()
{	
}

void mHiOsgLight::PostConfig()
{
	
	m_RootNode = HiKernel::OSGRenderer->getRoot();

	
//	m_World = NULL;
	m_Model = NULL;
	
//	m_World = osgDB::readNodeFile("JoeDirt.osg");
	m_Model = osgDB::readNodeFile("gilder/glider.ive");
	m_Pos = new osg::PositionAttitudeTransform();
	
	// m_pOSGRender->m_pRoot->addChild(m_World);
	m_RootNode->addChild(m_Pos);
	
	m_Pos->addChild(m_Model);
	m_Pos->setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
	//// create statesets.
	osg::StateSet* rootStateSet = new osg::StateSet;
	m_RootNode->setStateSet(rootStateSet);
	//// create a spot light.
	//osg::Light* myLight1 = new osg::Light;
	//myLight1->setLightNum(0);
	//myLight1->setPosition(osg::Vec4(0.0f,0.0f, 10.0f, 1.0f));
	//myLight1->setAmbient(osg::Vec4(0.0f,1.0f,0.0f,1.0f));
	//myLight1->setDiffuse(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	//myLight1->setSpotCutoff(20.0f);
	//myLight1->setSpotExponent(50.0f);
	//myLight1->setDirection(osg::Vec3(0.0f,0.0f,-1.0f));
	//osg::LightSource* lightS1 = new osg::LightSource;    
	//lightS1->setLight(myLight1);
	//lightS1->setLocalStateSetModes(osg::StateAttribute::ON);
	//lightS1->setStateSetModes(*rootStateSet,osg::StateAttribute::ON);
	//m_pOSGRender->m_pRoot->addChild(lightS1);
	// create a local light.
	myLight2 = new osg::Light;
	myLight2->setLightNum(0);
	myLight2->setPosition(osg::Vec4(0.0,0.0, 0.0,1.0f));
	myLight2->setAmbient(osg::Vec4(0.0f,1.0f,1.0f,1.0f));
	myLight2->setDiffuse(osg::Vec4(0.0f,1.0f,1.0f,1.0f));
	myLight2->setConstantAttenuation(1.0f);
	myLight2->setLinearAttenuation(2.0f);
	myLight2->setQuadraticAttenuation(2.0f);
	lightS2 = new osg::LightSource;    
	lightS2->setLight(myLight2);
	lightS2->setLocalStateSetModes(osg::StateAttribute::ON);
	lightS2->setStateSetModes(*rootStateSet,osg::StateAttribute::ON);
	m_RootNode->addChild(lightS2);
	
}

void mHiOsgLight::Update()
{
	//NSLog(@"mHiOsgLight");
	
	if(m_LightCheck)
	{
		m_LightPosY = m_LightPosY + 0.01f;
		if(m_LightPosY>1.0f)
		{
			m_LightCheck=false;
		}
	}
	else
	{
		m_LightPosY = m_LightPosY - 0.01f;
		if(m_LightPosY < -1.0f)
		{
			m_LightCheck=true;
		}
	}
	
	
	myLight2->setPosition(osg::Vec4(0.0f, m_LightPosY, 0.0f,1.0f));
	
}
void mHiOsgLight::Render()
{
}

void mHiOsgLight::Terminate()
{
}

#ifndef HI_IOS4

extern "C" 
HI_DLLEXPORT mHiOsgLight* HiLoadModule_mHiOsgLight(HiKernel::HiXmlNode* xml)
{
	mHiOsgLight *m = new mHiOsgLight(xml);
	return m;
}

#endif