#if defined ( HI_GLES1_RENDERER )

/*
 *  mHiOpenGLES10Demo.cpp
 *  KIST_iPhone_Mart
 *
 *  Created by karan99 on 10. 7. 22..
 *  Copyright 2010 IMRLAB. All rights reserved.
 *
 */

#include "mHiOpenGLES10Demo.h"
#include "HiKernel/HiXmlNode.h"
#include "HiKernel/HiTelegram.h"
#include "HiModules/enumerations.h"

//#include "HiKernel/HiModuleManager.h"

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

using namespace HiModules;

mHiOpenGLES10Demo::mHiOpenGLES10Demo(HiKernel::HiXmlNode* __pXml)
{
	vx_ = 1;
	prev_vx_ = 1;
	vxfield_ = new HiKernel::HiVec3f(this, "pt");
}

mHiOpenGLES10Demo::mHiOpenGLES10Demo()
{
	
}

void mHiOpenGLES10Demo::PreConfig()
{	
}

void mHiOpenGLES10Demo::PostConfig()
{
	is_stop_ = false;
	is_left_ = false;
	is_right_ = false;
}

void mHiOpenGLES10Demo::Update()
{
	vx_ = vxfield_->m_pStorage[0];
	if(prev_vx_ < vx_)
	{
		is_right_ = true;
		is_left_ = false;
	}
	else if(prev_vx_ > vx_)
	{
		is_right_ = false;
		is_left_ = true;
	}
	
	prev_vx_ = vx_;
}

void mHiOpenGLES10Demo::Render()
{
	const GLfloat squareVertices[] = {
        -0.5f, -0.5f,
        0.5f,  -0.5f,
        -0.5f,  0.5f,
        0.5f,   0.5f,
    };
    const GLubyte squareColors[] = {
        255, 255, 255, 255,
        255, 255, 255, 255,
        255, 255, 255, 255,
        255, 255, 255, 255,
    };
	const GLubyte squareColors2[] = {
        0,     0,   0,   0,
        0,     0,   0,   0,
        0,     0,   0,   0,
        0,     0,   0,   0,
    };
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	if(is_right_)
		glRotatef(3.0f, 0.0f, 0.0f, 1.0f);
	else if(is_left_)
		glRotatef(-3.0f, 0.0f, 0.0f, 1.0f);		
	
	glVertexPointer(2, GL_FLOAT, 0, squareVertices);
    glEnableClientState(GL_VERTEX_ARRAY);
	if(!is_stop_)
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
	else
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors2);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void mHiOpenGLES10Demo::Terminate()
{
}

bool mHiOpenGLES10Demo::HandleMessage(const HiKernel::HiTelegram& msg)
{
	if(msg.Msg == IPHONE_TOUCHED)
	{
		is_stop_ = !is_stop_;
	}
	if(msg.Msg == IPHONE_TOUCH_END)
	{
		is_right_ = false;
		is_left_ = false;
	}
	
	return true;
}



#endif