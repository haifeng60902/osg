/*
 *  HiOsgCal.h
 *  iOSG
 *
 *  Created by whitefir0 on 09. 06. 16.
 *  Copyright 2009 IMRLAB. All rights reserved.
 *
 */

#pragma once


#include "HiKernel/HiModule.h"
#include "HiKernel/HiFramework.h"
/*
namespace osg
{
	class Group;
	class Node;
	class PositionAttitudeTransform;
	class Light;
	class LightSource;
}*/

#include <osg/Group>
#include <osg/Node>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/PositionAttitudeTransform>


namespace HiModules
{

	class HI_DLLEXPORT mHiOsgLight : public HiKernel::HiModule
	{	
		public :
		mHiOsgLight(HiKernel::HiXmlNode* __pXml = NULL);
		~mHiOsgLight();
		
		osg::Group* m_RootNode;
		
		osg::Node* m_World;
		osg::Node* m_Model;
		osg::PositionAttitudeTransform* m_Pos;
		
		osg::Light* myLight2;
		osg::LightSource* lightS2;
		float m_LightPosY;
		bool m_LightCheck;
		
		
		virtual bool            HandleMessage(const HiKernel::HiTelegram& msg);
		virtual void            PreConfig();
		virtual void            PostConfig();
		virtual void            Render();
		virtual void            Terminate();
		virtual void            Update();
		int						ParseXml(HiKernel::HiXmlNode *xml);
		
		
		//osg::ref_ptr<osg::LightSource> ls;
		private :
		
	};
}