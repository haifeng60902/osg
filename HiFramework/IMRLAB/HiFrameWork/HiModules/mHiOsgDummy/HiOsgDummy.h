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

#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Billboard>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>

#include <osgViewer/Viewer>

#include <osg/PositionAttitudeTransform>

//#include <vector>

class HI_DLLEXPORT mHiOsgDummy : public HiKernel::HiModule
{	
public :
	mHiOsgDummy(HiKernel::HiXmlNode* __pXml = NULL);
	~mHiOsgDummy();
	
    virtual bool            HandleMessage(const HiKernel::HiTelegram& msg);
	virtual void            PreConfig();
	virtual void            PostConfig();
    virtual void            Render();
    virtual void            Terminate();
	virtual void            Update();
	int						ParseXml(HiKernel::HiXmlNode *xml);
    
private :
	
	struct sModel
	{
		osg::Node* ModelNode;
		osg::PositionAttitudeTransform* ModelPos;
		osg::MatrixTransform* ModelRot;
		std::string ModelFileName;
		int ModelTrans[3];
		float ModelRotation[4];
		float ModelScale[3];
	};
	
	std::vector<sModel*> m_vModel;
	
	osg::Group* rootNode;
	osg::Node* terrainNode;
	
	//osg::Group* aa;
};

