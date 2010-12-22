/*
 *  HiOsgCal.cpp
 *  iOSG
 *
 *  Created by whitefir0 on 09. 06. 16.
 *  Copyright 2009 IMRLAB. All rights reserved.
 *
 */

#include "HiModules/mHiOsgDummy/HiOsgDummy.h"
#include "HiKernel/HiXmlNode.h"
#include "HiModules/enumerations.h"
#include "HiKernel/HiOSGRenderer.h"



using namespace HiModules;

mHiOsgDummy::mHiOsgDummy(HiKernel::HiXmlNode* __pXml)
{
	if(__pXml != NULL)
		ParseXml(__pXml);
}

mHiOsgDummy::~mHiOsgDummy()
{
}
int mHiOsgDummy::ParseXml(HiKernel::HiXmlNode *xml)
{
	HiKernel::HiXmlNode* node = xml->FindChild("data");
	
	for(int i=0; i<node->GetNumChildren();i++)
	{
		HiKernel::HiXmlNode* child = node->GetChild(i);
		
		sModel* Model = new sModel;
		Model->ModelFileName = child->GetAttrs("file");
		child->GetAttrs("trans", 3 , Model->ModelTrans);
		child->GetAttrs("rotation", 4 , Model->ModelRotation);
		child->GetAttrs("scale", 3 , Model->ModelScale);
		
		m_vModel.push_back(Model);
	}
	return 1;
}

bool mHiOsgDummy::HandleMessage(const HiKernel::HiTelegram& msg)
{
	//	if(MSG_SIZE <= msg.Msg)
	//		return false;
    
	
	return true;
}

void mHiOsgDummy::PreConfig()
{	
}

void mHiOsgDummy::PostConfig()
{
	
	rootNode = HiKernel::OSGRenderer->getRoot();
	
//#if defined(OSG_GLES1_AVAILABLE)
	
	for(int i=0; i<m_vModel.size(); i++)
	{
		m_vModel[i]->ModelNode = NULL;
		m_vModel[i]->ModelNode = osgDB::readNodeFile(m_vModel[i]->ModelFileName.c_str());	
		
		//osgDB::ReaderWriterIVE* iveLoader = new osgDB::ReaderWriterIVE;
		//ReaderWriter::ReadResult rr = iveLoader->readNode(m_vModel[i]->ModelFileName.c_str(), NULL);
		
		m_vModel[i]->ModelPos = new osg::PositionAttitudeTransform();
		m_vModel[i]->ModelRot = new osg::MatrixTransform();
		
		m_vModel[i]->ModelPos->addChild(m_vModel[i]->ModelRot);
		m_vModel[i]->ModelRot->addChild(m_vModel[i]->ModelNode);
		
		rootNode->addChild(m_vModel[i]->ModelPos);
		
		m_vModel[i]->ModelPos->setPosition(osg::Vec3(m_vModel[i]->ModelTrans[0],m_vModel[i]->ModelTrans[1],m_vModel[i]->ModelTrans[2]));
		m_vModel[i]->ModelPos->setScale(osg::Vec3(m_vModel[i]->ModelScale[0],m_vModel[i]->ModelScale[1],m_vModel[i]->ModelScale[2]));
		
		m_vModel[i]->ModelRot->setMatrix(osg::Matrix::rotate(m_vModel[i]->ModelRotation[0],
															 m_vModel[i]->ModelRotation[1],
															 m_vModel[i]->ModelRotation[2],
															 m_vModel[i]->ModelRotation[3]));
	}
//#endif
//	
//#if defined(OSG_GLES2_AVAILABLE)
//	
//	static const char* vertSource = {
//		"void main()\n"
//		"{\n"
//		"    gl_Position = ftransform();\n"
//		"}\n"
//	};
//	
//	static const char* fragSource = {
//		"void main()\n"
//		"{\n"
//		"    gl_FragColor = vec4(1.0,0.0,0.0,1.0);\n"
//		"}\n"
//	};
//	
//	
//	osg::Node* building  = osgDB::readNodeFile("building01.ive"); //-10 10
//	
//	
//    osg::StateSet* state_set = building->getOrCreateStateSet();
//	//  osg::StateSet* state_set = pyramidGeode->getOrCreateStateSet();
//    {
//        osg::Program* test_program = new osg::Program;
//        
//        osg::Shader* test_vs_shader = new osg::Shader( osg::Shader::VERTEX, vertSource);
//        {
//			//   NSString* shader_path = [[NSBundle mainBundle] pathForResource: @"Shader"
//			//                                                         ofType: @"vsh"];
//			//  test_vs_shader->loadShaderSourceFromFile([shader_path cStringUsingEncoding: 1]);
//        }
//        
//        osg::Shader* test_fs_shader = new osg::Shader( osg::Shader::FRAGMENT, fragSource);
//        {
//			//  NSString* shader_path = [[NSBundle mainBundle] pathForResource: @"Shader"
//            //                                                        ofType: @"fsh"];
//			//   test_fs_shader->loadShaderSourceFromFile([shader_path cStringUsingEncoding: 1]);
//        }
//        
//        test_program->addShader( test_vs_shader );
//        test_program->addShader( test_fs_shader );
//        
//        state_set->setAttributeAndModes(test_program);
//    }
//	
//	rootNode->addChild(building);    
//	
//	
//#endif
	
}

void mHiOsgDummy::Render()
{
}

void mHiOsgDummy::Terminate()
{
}

void mHiOsgDummy::Update()
{
	//	NSLog(@"mHiOsgDummy");
	
}

#ifndef HI_IOS4

extern "C" 
HI_DLLEXPORT mHiOsgDummy* HiLoadModule_mHiOsgDummy(HiKernel::HiXmlNode* xml)
{
	mHiOsgDummy *m = new mHiOsgDummy(xml);
	return m;
}

#endif