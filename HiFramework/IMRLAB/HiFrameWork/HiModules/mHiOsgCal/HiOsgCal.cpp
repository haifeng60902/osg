 /*
 *  HiOsgCal.cpp
 *  iOSG
 *
 *  Created by whitefir0 on 09. 06. 16.
 *  Copyright 2009 IMRLAB. All rights reserved.
 *
 */

#include "HiModules/mHiOsgCal/HiOsgCal.h"
//#include "HiModules/TileNode.h"
#include "HiKernel/HiXmlNode.h"
#include "HiKernel/HiTelegram.h"
#include "HiModules/enumerations.h"
#include "HiKernel/HiOSGRenderer.h"


#include <osgViewer/Viewer>
#include <osg/ArgumentParser>
#include <osg/CullFace>
#include <osg/Light>
#include <osg/Notify>
#include <osg/ShadeModel>
#include <osg/LightModel>
#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osg/Texture2D>
#include <osgUtil/GLObjectsVisitor>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>




using namespace HiModules;

mHiOsgCal::mHiOsgCal(HiKernel::HiXmlNode* __pXml) :
m_pRoot(new osg::Group),
m_pCalGroup(new osg::Group),
m_ModelCount(0)
{
	if(__pXml != NULL)
		ParseXml(__pXml);
	
	m_Ani=0;
}

mHiOsgCal::~mHiOsgCal()
{
}
int mHiOsgCal::ParseXml(HiKernel::HiXmlNode *xml)
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
	float scalexyz[3];
	for(int i=0;i<node->GetNumChildren();i++)
	{
		m_calChildModel.push_back(new s_CalInstanceModel);
		HiKernel::HiXmlNode* child = node->GetChild(i);
		tempCoreId = child->GetAttrs("coreid");
		child->GetAttrs("trans", 3, tempxyz);
		child->GetAttrs("scale", 3, scalexyz);
		for(int j=0;j<m_calModel.size();j++)
		{
			//addModel(0, tempxyz[0], tempxyz[1], tempxyz[2]);
			if(m_calModel[j]->m_CordId == tempCoreId)
			{
				m_calChildModel[i]->m_index = j;
				m_calChildModel[i]->m_Pos.set(tempxyz[0], tempxyz[1], tempxyz[2]);
				m_calChildModel[i]->m_Scale.set(scalexyz[0], scalexyz[1], scalexyz[2]);
			}
		}
	}
	
	
	return 0;
}

bool mHiOsgCal::HandleMessage(const HiKernel::HiTelegram& msg)
{
	if(MSG_SIZE <= msg.Msg)
		return false;
	
	//if(IPHONEINPUT_ONETOUCH_DOWN == msg.Msg)
	//{
	//	//NSLog(@"? ë‹ˆë©”ì´??ë°”êº¼ì£¼ì‹­?œìš”???‹ã…‹??);
	//	ChangeAnimation();		
	//}
 //   

	return true;
}

void mHiOsgCal::PreConfig()
{	

}

void mHiOsgCal::PostConfig()
{
	m_pRoot = HiKernel::OSGRenderer->getRoot();
	
	for(int i=0;i<m_calChildModel.size();i++)
	{
		//xml?‹á…¦?‰á…¥ ?‡á…¡??„‹?¡á„‹?©á†« ?†á…©?ƒá…¦??????Œá…ª?‘á…­?€?¡á†¹?‹á…³?…á…© ?á…¢?…á…µ?¨á„?¥á„…?³á†¯ ?‰á…¢?¼á„‰?¥á†¼?’á…¢?Œá…®?«á„ƒ??
		addModel(m_calChildModel[i]->m_index, m_calChildModel[i]->m_Pos.x(), 
				 m_calChildModel[i]->m_Pos.y(),
				 m_calChildModel[i]->m_Pos.z(),
				 m_calChildModel[i]->m_Scale.x(),
				 m_calChildModel[i]->m_Scale.y(),
				 m_calChildModel[i]->m_Scale.z());
	}
	
	m_pCalGroup->setName("CalGroup");
	//active?ƒá…¬??„„???‹á…¥?á…¢?Žá…µ?’á…¡?«á„ƒ??
	m_pRoot->addChild(m_pCalGroup);
}

void mHiOsgCal::Render()
{
}

void mHiOsgCal::Terminate()
{
}

void mHiOsgCal::Update()
{
	//NSLog(@"OsgCal");
 
}

void mHiOsgCal::ChangeAnimation()
{
	m_Ani++;
	for( int i=0; i<m_calChildModel.size();i++)
	{
		int count =	m_calChildModel[i]->m_Model->getCoreModel()->getAnimationNames().size();
		
		if(m_Ani >= count)
		{
			m_Ani = 0;
		}
		
		for(int j=0; j<count; j++)
		{
			if( j != m_Ani)
			{
				m_calChildModel[i]->m_Model->clearCycle(j, 0.5f);
			}
		}
		m_calChildModel[i]->m_Model->blendCycle( m_Ani, 0.6f, 0.4f);
		
		
	}
}

osgCal::Model* mHiOsgCal::makeModel(osgCal::CoreModel *cm, osgCal::BasicMeshAdder *ma, int animNum)
{
	osgCal::Model* model = new osgCal::Model();
	
	model->load( cm, ma );
	
//	if ( animNum != -1 )
//	{
		model->blendCycle( 0, 1.0f, 0 );
//	}
	return model;	
}
bool mHiOsgCal::addModel(int _id, float _x, float _y, float _z, float _ScaleX, float _ScaleY, float _ScaleZ)
{
	static int Count=0;
	osg::Vec3 _pos;
	_pos.set(_x, _y, _z);		
	osg::Vec3 _Scale;
	_Scale.set(_ScaleX, _ScaleY, _ScaleZ);
	
	m_calChildModel[Count]->m_Model = makeModel(m_calModel[_id]->m_CoreModel.get(),
												m_calChildModel[Count]->m_meshAdder.get(),
												-1 );
	
	m_calChildModel[Count]->m_modelPos->setPosition(_pos);
	m_calChildModel[Count]->m_modelPos->setScale(_Scale);
	if( !m_calModel[_id])
	{
		return false;
	}
	
	m_pCalGroup->addChild( m_calChildModel[Count]->m_modelPos );
	
	m_calChildModel[Count]->m_modelPos->addChild( m_calChildModel[Count]->m_rotMat );
	m_calChildModel[Count]->m_rotMat->addChild( m_calChildModel[Count]->m_Model );
	
	Count++;
	return true;
}

#ifndef HI_IOS4

extern "C" 
HI_DLLEXPORT mHiOsgCal* HiLoadModule_mHiOsgCal(HiKernel::HiXmlNode* xml)
{
	mHiOsgCal *m = new mHiOsgCal(xml);
	return m;
}

#endif