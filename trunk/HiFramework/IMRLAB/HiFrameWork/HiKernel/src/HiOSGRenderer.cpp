#if defined ( HI_OSG_RENDERER )

//
//  HiOSGRenderer.mm
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//


#include "HiKernel/HiOSGRenderer.h"

#include <osg/ArgumentParser>

using namespace HiKernel;

HiOSGRenderer* HiOSGRenderer::getInstance()
{
	static HiOSGRenderer _instance;
	
	return &_instance;
}

HiOSGRenderer::HiOSGRenderer(): HiRenderer(OpenSceneGraph), m_pOSGViewer(NULL), m_pRoot(NULL)
{
	m_pRoot = new osg::Group();
	
//	int argc = 0;
//	osg::ArgumentParser arguments(&argc,0);;
	
//	arguments.getApplicationUsage()->addCommandLineOption("--SingleThreaded","");
	
	m_pOSGViewer = new osgViewer::Viewer();
}

HiOSGRenderer::~HiOSGRenderer()
{
	// root는 destructor가 protected임.
	//delete m_pOSGViewer;
}


#ifdef HI_WIN32
bool HiOSGRenderer::Initialize(HWND hWnd, HINSTANCE hInst)
#else
bool HiOSGRenderer::Initialize()
#endif
{
	m_pOSGViewer->setSceneData(m_pRoot);
	m_pOSGViewer->realize();	
	
	return true;
}

void HiOSGRenderer::BeginScene()
{
	m_pOSGViewer->frame();
	
	// cgoarj deb yg test
	
}

void HiOSGRenderer::EndScene()
{
}


void HiOSGRenderer::Terminate()
{
}

bool HiOSGRenderer::HandleMessage(const HiTelegram& msg)
{
	return true;
}

osg::Group* HiOSGRenderer::getRoot() const
{
	return m_pRoot;
}
osgViewer::Viewer* HiOSGRenderer::getViewer() const
{
	return m_pOSGViewer;
}


bool HiOSGRenderer::IsDone() 
{
	return m_pOSGViewer->done();
}


#ifdef HI_IOS4

void HiOSGRenderer::resizeFromLayer(CAEAGLLayer* __eaglLayer)
{
	/*
	// Allocate color buffer backing based on the current layer size
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:__eaglLayer];
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
		NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
    }
	 */
}
/*
void HiOSGRenderer::setCAEAGLLayer(CAEAGLLayer* __eaglLayer)
{
	osgViewer::GraphicsWindowiPhone::setCAEAGLLayer(__eaglLayer);
}
*/
#endif
#endif