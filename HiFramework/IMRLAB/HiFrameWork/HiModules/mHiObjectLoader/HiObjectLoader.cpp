/*
 *  HiObjectLoader.cpp
 *  ISA
 *
 *  Created by rehomik on 10. 07. 07
 *  Copyright 2010 IMRLAB. All rights reserved.
 *
 */
#include "HiModules/mHiObjectLoader/HiObjectLoader.h"

#include "HiKernel/HiOSGRenderer.h"
#include "HiModules/enumerations.h"

mHiObjectLoader::mHiObjectLoader(HiXmlNode* __pXml)
{
	transform_node_ = 0;
}

mHiObjectLoader::~mHiObjectLoader()
{
}

bool mHiObjectLoader::HandleMessage(const Telegram& msg)
{
	if (MSG_SIZE <= msg.Msg)
	{
		return false;
	}
	
	if (HIOBJECT_LOADER_PITCH_MINUS == msg.Msg)
	{
		osg::Quat minus_rotation(PER_ANGLE * M_PI / 180, osg::Vec3(1.0f, 0.0f, 0.0));
		osg::Matrixf rotation_matrix(minus_rotation);
		transform_node_->postMult(rotation_matrix);
		
	}else if (HIOBJECT_LOADER_PITCH_PLUS == msg.Msg)
	{
		osg::Quat minus_rotation(-PER_ANGLE * M_PI / 180, osg::Vec3(1.0f, 0.0f, 0.0));
		osg::Matrixf rotation_matrix(minus_rotation);
		transform_node_->postMult(rotation_matrix);
		
	}else if (HIOBJECT_LOADER_ROLL_MINUS == msg.Msg)
	{
		osg::Quat minus_rotation(PER_ANGLE * M_PI / 180, osg::Vec3(0.0f, 1.0f, 0.0));
		osg::Matrixf rotation_matrix(minus_rotation);
		transform_node_->postMult(rotation_matrix);
		
	}else if (HIOBJECT_LOADER_ROLL_PLUS == msg.Msg)
	{
		osg::Quat minus_rotation(-PER_ANGLE * M_PI / 180, osg::Vec3(0.0f, 1.0f, 0.0));
		osg::Matrixf rotation_matrix(minus_rotation);
		transform_node_->postMult(rotation_matrix);
		
	}else if (HIOBJECT_LOADER_SCALE_MINUS == msg.Msg)
	{
		osg::Matrixf scale_matrix;
		scale_matrix.makeScale(osg::Vec3(SCALE_MINUS_VALUE,
										 SCALE_MINUS_VALUE,
										 SCALE_MINUS_VALUE));
		transform_node_->postMult(scale_matrix);
		
	}else if (HIOBJECT_LOADER_SCALE_PLUS == msg.Msg)
	{
		osg::Matrixf scale_matrix;
		scale_matrix.makeScale(osg::Vec3(SCALE_PLUS_VALUE,
										 SCALE_PLUS_VALUE,
										 SCALE_PLUS_VALUE));
		transform_node_->postMult(scale_matrix);
		
	}else if (HIOBJECT_LOADER_TRANSLATION_UP == msg.Msg)
	{
		osg::Matrixf translation_matrix;
		translation_matrix.makeTranslate(osg::Vec3(0,
												   0,
												   TRANSLATION_VALUE));
		transform_node_->postMult(translation_matrix);
		
	}else if (HIOBJECT_LOADER_TRANSLATION_LEFT == msg.Msg)
	{
		osg::Matrixf translation_matrix;
		translation_matrix.makeTranslate(osg::Vec3(-TRANSLATION_VALUE,
												   0,
												   0));
		transform_node_->postMult(translation_matrix);
		
	}else if (HIOBJECT_LOADER_TRANSLATION_RIGHT == msg.Msg)
	{
		osg::Matrixf translation_matrix;
		translation_matrix.makeTranslate(osg::Vec3(TRANSLATION_VALUE,
												   0,
												   0));
		transform_node_->postMult(translation_matrix);
		
	}else if (HIOBJECT_LOADER_TRANSLATION_DOWN == msg.Msg)
	{
		osg::Matrixf translation_matrix;
		translation_matrix.makeTranslate(osg::Vec3(0,
												   0,
												   -TRANSLATION_VALUE));
		transform_node_->postMult(translation_matrix);
		
	}else if (HIOBJECT_LOADER_YAW_MINUS == msg.Msg)
	{
		osg::Quat minus_rotation(-PER_ANGLE * M_PI / 180, osg::Vec3(0.0f, 0.0f, 1.0));
		osg::Matrixf rotation_matrix(minus_rotation);
		transform_node_->postMult(rotation_matrix);
		
	}else if (HIOBJECT_LOADER_YAW_PLUS == msg.Msg)
	{
		osg::Quat minus_rotation(PER_ANGLE * M_PI / 180, osg::Vec3(0.0f, 0.0f, 1.0));
		osg::Matrixf rotation_matrix(minus_rotation);
		transform_node_->postMult(rotation_matrix);
		
	}
	
	return true;
}

void mHiObjectLoader::PreConfig()
{
}

void mHiObjectLoader::PostConfig()
{
    osg::Node* test_node = osgDB::readNodeFile("building01.ive");
	transform_node_ = new osg::MatrixTransform();
	transform_node_->addChild(test_node);
    
	osgViewer::Viewer* osg_viewer = OSGRenderer->getViewer();
	
    osg_viewer->setSceneData(transform_node_);
    osg_viewer->setCameraManipulator(new osgGA::TrackballManipulator());
	osg_viewer->getCamera()->setClearColor(osg::Vec4f(0.0f, 0.0f, 0.0f, 0.0f));
}

void mHiObjectLoader::Render()
{
    OSGRenderer->getViewer()->frame();
}

void mHiObjectLoader::Terminate()
{
}

void mHiObjectLoader::Update()
{
}