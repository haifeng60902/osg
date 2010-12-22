#if defined ( HI_GLES1_RENDERER )
//
//  HiOpenGLES10Renderer.h
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#pragma once

#include "HiKernel/HiRenderer.h"

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>


namespace HiKernel {
	
	class HiTelegram;

	class HI_DLLEXPORT HiOpenGLES10Renderer : public HiRenderer 
	{
		private :
		
		EAGLContext *context;
		
		// The pixel dimensions of the CAEAGLLayer
		GLint backingWidth;
		GLint backingHeight;
		
		// The OpenGL names for the framebuffer and renderbuffer used to render to this view
		GLuint defaultFramebuffer, colorRenderbuffer;
		
		public :
		HiOpenGLES10Renderer();
		
	public:
		~HiOpenGLES10Renderer();
		
		virtual bool HandleMessage(const HiTelegram& msg);
		

		virtual bool Initialize();
		virtual void Terminate();
		virtual void BeginScene();
		virtual void EndScene();
		
		void resizeFromLayer(CAEAGLLayer* __eaglLayer);
		
	};
	
}

#endif