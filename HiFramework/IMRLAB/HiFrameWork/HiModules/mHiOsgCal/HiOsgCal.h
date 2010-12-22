//
//  HiOsgCal.h 
//  opengles_01 [ Tutorial.02 for iPhone ] 
//
//  Created by [ ê¹€?€ê·?] 7/11/10.
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserved.
//

#pragma once

#include "HiKernel/HiModule.h"
#include "HiKernel/HiFramework.h"
// #include "HiKernel/HiTelegram.h"
#include <osgCal/CoreModel>
#include <osgCal/Model>


#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>


namespace HiModules 
{
	
	struct s_CalCoreModel
	{
		//osgCal::Model* m_Model;
		std::string	m_CordId;
		int			m_CoreIndex;
		osg::ref_ptr<osgCal::CoreModel> m_CoreModel; 
		std::vector< std::string > animationNames;
		osg::ref_ptr<osgCal::MeshParameters> m_p;
		
		s_CalCoreModel() : 
		m_CoreModel(new osgCal::CoreModel),
		m_p(new osgCal::MeshParameters)
		{}
	};
	
	struct s_CalInstanceModel
	{
		int m_index;
		osgCal::Model* m_Model;
		osg::ref_ptr<osgCal::BasicMeshAdder> m_meshAdder;
		osg::PositionAttitudeTransform* m_modelPos;
		osg::Vec3	m_Pos;
		osg::Vec3   m_Scale;
		osg::Vec4	m_Forword;
		
		osg::Vec4   m_Direction;
		osg::Vec4   m_BackVector;
		osg::Vec4   m_RightVector;
		osg::Vec4   m_LeftVector;
		osg::Vec4   m_UpVector;
		osg::Vec4   m_DownVector;
		
		osg::MatrixTransform* m_rotMat;
		
		s_CalInstanceModel() : m_modelPos(new osg::PositionAttitudeTransform),
		m_Model(new osgCal::Model),
		m_meshAdder(new osgCal::DefaultMeshAdder),
		m_rotMat(new osg::MatrixTransform)
		{}
	};
	
	class HI_DLLEXPORT mHiOsgCal : public HiKernel::HiModule
	{	
		public :
		mHiOsgCal(HiKernel::HiXmlNode* __pXml = NULL);
		~mHiOsgCal();
		
		virtual bool            HandleMessage(const HiKernel::HiTelegram& msg);
		virtual void            PreConfig();
		virtual void            PostConfig();
		virtual void            Render();
		virtual void            Terminate();
		virtual void            Update();
		int						ParseXml(HiKernel::HiXmlNode *xml);
		
		osgCal::Model* makeModel( osgCal::CoreModel* cm,
								 osgCal::BasicMeshAdder* ma,
								 int animNum = -1) ;
		bool	addModel(int _id, float _x, float _y, float _z, float _ScaleX, float _ScaleY, float _ScaleZ);
		
		void ChangeAnimation();
		
		int m_Ani;
		
		private :
		
		osg::Group* m_pCalGroup;
		
		std::string m_cfgFile;
		std::vector<s_CalCoreModel*> m_calModel;
		typedef std::vector<s_CalInstanceModel*> CalInstance_array;
		CalInstance_array m_calChildModel;
		
		osg::ref_ptr<osg::Group> m_pRoot;
		
		// ?†á…©?ƒá…¦??„‹???€?¢á†º?‰á…®
		int	m_ModelCount;
		
		
	};
	
}