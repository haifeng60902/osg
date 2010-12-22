#if defined ( HI_GLES1_RENDERER )

/*
 *  mHiOpenGLES10Demo.h
 *  KIST_iPhone_Mart
 *
 *  Created by karan99 on 10. 7. 22..
 *  Copyright 2010 IMRLAB. All rights reserved.
 *
 */


#include "HiKernel/HiModule.h"
#include "HiKernel/HiField.h"

namespace HiModules {
	
	class HiXmlNode;
	class HiTelegram;

	
class HI_DLLEXPORT mHiOpenGLES10Demo : public HiKernel::HiModule
{
private:
	bool				is_stop_;
	bool				is_left_, is_right_;
	
	HiKernel::HiVec3f*	vxfield_;
	float				vx_, prev_vx_;
	
public :
	mHiOpenGLES10Demo(HiKernel::HiXmlNode* __pXml = NULL);
	mHiOpenGLES10Demo();
	
	virtual void PreConfig();
	virtual void PostConfig();
	virtual void Update();
	virtual void Render();
	virtual void Terminate();
	virtual bool HandleMessage(const HiKernel::HiTelegram& msg);
};
	
}

#endif