/*
 *  HiObjectLoader.h
 *  ISA
 *
 *  Created by rehomik on 10. 07. 07
 *  Copyright 2010 IMRLAB. All rights reserved.
 *
 */

#ifndef MART_HIFRAMEWORK_INCLUDE_HIMODULES_MHIOBJECTLOADER_MHIOBJECTLOADER_H
#define MART_HIFRAMEWORK_INCLUDE_HIMODULES_MHIOBJECTLOADER_MHIOBJECTLOADER_H

#include <osgViewer/Viewer>

#include "HiKernel/HiModule.h"
#include "HiKernel/HiTelegram.h"

class mHiObjectLoader : public HiModule
{	
public :
	mHiObjectLoader(HiXmlNode* __pXml = NULL);
	~mHiObjectLoader();
	
    virtual bool            HandleMessage(const Telegram& msg);
	virtual void            PreConfig();
	virtual void            PostConfig();
    virtual void            Render();
    virtual void            Terminate();
	virtual void            Update();
    
private :
	osg::MatrixTransform*	transform_node_;
};

#endif