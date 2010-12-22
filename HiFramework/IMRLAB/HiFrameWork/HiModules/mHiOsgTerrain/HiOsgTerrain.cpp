 /*
 *  HiOsgCal.cpp
 *  iOSG
 *
 *  Created by whitefir0 on 09. 06. 16.
 *  Copyright 2009 IMRLAB. All rights reserved.
 *
 */

#include "HiModules/mHiOsgTerrain/HiOsgTerrain.h"
#include "HiModules/enumerations.h"
#include "HiKernel/HiOSGRenderer.h"


#include <osgViewer/Viewer>
#include <osg/ArgumentParser>
#include <osg/MatrixTransform>
#include <osg/CullFace>
#include <osg/Light>
#include <osg/Vec3>
#include <osg/Vec3d>
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

#include <osg/PositionAttitudeTransform>


//#include <osgCal/CoreModel>
//#include <osgCal/Model>


using namespace HiModules;


mHiOsgTerrain::mHiOsgTerrain(HiKernel::HiXmlNode* __pXml)
{
	if(__pXml != NULL)
		ParseXml(__pXml);
	
	m_ChaXPos = 0.0f;
	
	m_ChaCheck=true;
}

mHiOsgTerrain::~mHiOsgTerrain()
{
}
int mHiOsgTerrain::ParseXml(HiKernel::HiXmlNode *xml)
{
	return 1;
}

bool mHiOsgTerrain::HandleMessage(const HiKernel::HiTelegram& msg)
{
	//if(HiKernel::MSG_SIZE <= msg.Msg)
	//	return false;
    

	return true;
}

void mHiOsgTerrain::PreConfig()
{	
}

void mHiOsgTerrain::PostConfig()
{
	m_RootNode = HiKernel::OSGRenderer->getRoot();
	m_CharacterNode = NULL;
	m_TerrainNode = NULL;
	
	m_ChaXform = new osg::PositionAttitudeTransform();
	
	m_CharacterNode = osgDB::readNodeFile("Fire/Fire.ive");
	m_TerrainNode = osgDB::readNodeFile("JoeDirt/JoeDirt.ive");
	
	m_RootNode->addChild(m_TerrainNode);
	m_RootNode->addChild(m_ChaXform);
	m_ChaXform->addChild(m_CharacterNode);
	
	//m_ChaXform->setScale(osg::Vec3(2.0f, 2.0f, 2.0f));
	
	
//	double tankXPosition = -100.0;
//	double tankYPosition = -10.0;
	
	m_TrackLocationSegment = new osgUtil::LineSegmentIntersector(osg::Vec3(0.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 0.0f) );
	m_findTrackElevationVisitor = new osgUtil::IntersectionVisitor(m_TrackLocationSegment);
	
	
//	m_TerrainNode->accept(findTankElevationVisitor);
//	
//	if ( tankLocationSegment->containsIntersections() )
//	{
//		
//		const osgUtil::LineSegmentIntersector::Intersection& intersection 
//		= tankLocationSegment->getFirstIntersection();
//		
//		terrainHeight = intersection.getWorldIntersectPoint();
//	}
//	
//	
//	tankXform->setPosition( terrainHeight );
	
}

void mHiOsgTerrain::Update()
{
	
	if(m_ChaCheck)
	{
		m_ChaXPos++;
	}
	else 
	{
		m_ChaXPos--;
	}

	
	m_ChaUpPos.set( 0.0f,m_ChaXPos, 999.0f);
	m_ChaDownPos.set( 0.0f,m_ChaXPos, -999.0f);
	
	m_TrackLocationSegment->setStart(m_ChaUpPos);
	m_TrackLocationSegment->setEnd(m_ChaDownPos);
	m_TerrainNode->accept(*m_findTrackElevationVisitor);
	
	
	//?Žá…®?¼á„ƒ?©á†¯?ƒá…¬?‚á…³?«á„Œ?µá„‹?§á†¨?€?¥á†·?‰á…¡( ?Žá…¡?½á„‹?³á†«?€???‹á…µ?»á„‹?³á„†?§á†« ?‰á…µ??„’?¢á†¼?’á…¢?Œá…®?«á„ƒ?? )
	if ( m_TrackLocationSegment->containsIntersections() )
	{
		/*
		const osgUtil::LineSegmentIntersector::Intersection& intersection 
		= m_TrackLocationSegment->getFirstIntersection();
		*/
		static osgUtil::LineSegmentIntersector::Intersection intersection = m_TrackLocationSegment->getFirstIntersection();


		m_TerrainHeight = intersection.getWorldIntersectPoint();
		
		//?Œá…¥?«á„‹?¦á„‘?µá†¨?á…µ?¼á„ƒ?¬á†« ?€?¡á†¹?‹á…³????¨á„‰?©á†¨ ?€?¡á„Œ?µá??©á„‹?µá†»?‹á…¥?‰á…¥ ?…á…µ?‰á…¦?ºá„‹?³á†¯ ?’á…¢?Œá…®?«á„ƒ??
		m_TrackLocationSegment->reset();
	}
	else 
	{
		if(m_ChaCheck)
		{
			m_ChaCheck=false;
		}
		else 
		{
			m_ChaCheck=true;
		}
	}

	
	m_ChaXform->setPosition(m_TerrainHeight);
}

void mHiOsgTerrain::Render()
{
}

void mHiOsgTerrain::Terminate()
{
}

#ifndef HI_IOS4

extern "C" 
HI_DLLEXPORT mHiOsgTerrain* HiLoadModule_mHiOsgTerrain(HiKernel::HiXmlNode* xml)
{
	mHiOsgTerrain *m = new mHiOsgTerrain(xml);
	return m;
}

#endif

