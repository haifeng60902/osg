#if defined ( HI_OSG_RENDERER )
//
//  HiOSGRenderer.h
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#pragma once

#include "HiKernel/HiRenderer.h"

// osg
#include <osg/MatrixTransform>
#include <osg/LightModel>
#include <osg/displaysettings>
#include <osgDB/ReadFile>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/group>
#include <osg/PositionAttitudeTransform>
#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#ifdef HI_IOS4
#include <osgViewer/api/iPhone/GraphicsWindowiPhone>
#endif

namespace HiKernel {
	
	#define OSGRenderer HiOSGRenderer::getInstance()
	
	class HiTelegram;
	
	class HI_DLLEXPORT HiOSGRenderer : public HiRenderer 
	{
		private :
		HiOSGRenderer();
		
	private:	
		osgViewer::Viewer*	m_pOSGViewer;
		osg::Group* m_pRoot;
		
	public:
		~HiOSGRenderer();
		
		static HiOSGRenderer* getInstance();
		
		virtual bool HandleMessage(const HiTelegram& msg);
		
#ifdef HI_WIN32
		virtual bool Initialize(HWND hWnd, HINSTANCE hInst = NULL);
#else
		virtual bool Initialize();
#endif
		virtual void Terminate();
		virtual void BeginScene();
		virtual void EndScene();

		virtual bool IsDone();
		
		osgViewer::Viewer* getViewer() const;	
		osg::Group* getRoot() const;
		
#ifdef HI_IOS4		
		void resizeFromLayer(CAEAGLLayer* __eaglLayer);
#endif
	};
	
	
}
#endif