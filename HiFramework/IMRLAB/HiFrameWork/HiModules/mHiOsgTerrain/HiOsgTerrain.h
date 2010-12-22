/*
 *  HiOsgCal.h
 *  iOSG
 *
 *  Created by whitefir0 on 09. 06. 16.
 *  Copyright 2009 IMRLAB. All rights reserved.
 *
 */

#pragma once

#include "HiKernel/HiFramework.h"
#include "HiKernel/HiModule.h"

#include <osg/Vec3>
#include <osg/Vec3d>


namespace osg {
	
	class Group;
	class Node;
	class PositionAttitudeTransform;
}

namespace osgUtil {
	class LineSegmentIntersector;
	class IntersectionVisitor;
}

namespace HiModules {
	
	class HI_DLLEXPORT mHiOsgTerrain : public HiKernel::HiModule
	{	
		public :
		mHiOsgTerrain(HiKernel::HiXmlNode* __pXml = NULL);
		~mHiOsgTerrain();
		
		osg::Group* m_RootNode;
		
		osg::Node* m_CharacterNode;
		osg::Node* m_TerrainNode;
		
		osg::PositionAttitudeTransform* m_ChaXform;
		
		osg::Vec3d m_TerrainHeight;
		
		osg::Vec3 m_ChaUpPos;
		osg::Vec3 m_ChaDownPos;
		
		float m_ChaXPos;
		bool m_ChaCheck;
		
		
		osgUtil::LineSegmentIntersector* m_TrackLocationSegment;
		osgUtil::IntersectionVisitor* m_findTrackElevationVisitor;
		
		virtual bool            HandleMessage(const HiKernel::HiTelegram& msg);
		virtual void            PreConfig();
		virtual void            PostConfig();
		virtual void            Render();
		virtual void            Terminate();
		virtual void            Update();
		int						ParseXml(HiKernel::HiXmlNode *xml);
		
		private :
	};
}
