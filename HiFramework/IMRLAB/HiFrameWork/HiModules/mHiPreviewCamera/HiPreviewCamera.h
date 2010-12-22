/*
 *  HiPreviewCamera.h
 *  ISA
 *
 *  Created by rehomik on 10. 07. 09
 *  Copyright 2010 IMRLAB. All rights reserved.
 *
 */

#ifndef __MART_HIFRAMEWORK_INCLUDE_HIMODULES_MHIOSGCAL_HIOSGCAL_H__
#define __MART_HIFRAMEWORK_INCLUDE_HIMODULES_MHIOSGCAL_HIOSGCAL_H__

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#include "HiKernel/HiModule.h"
#include "HiKernel/HiTelegram.h"

@class EAGLView;

@interface AVPreviewCameraViewController : UIViewController
{    
    AVCaptureVideoPreviewLayer* preview_video_layer_;
	double						previous_time_;
}

@end


class mHiPreviewCamera : public HiModule
{	
public :
	mHiPreviewCamera(HiXmlNode* __pXml = NULL);
	~mHiPreviewCamera();
	
    virtual bool            HandleMessage(const Telegram& msg);
	virtual void            PreConfig();
	virtual void            PostConfig();
    virtual void            Render();
    virtual void            Terminate();
	virtual void            Update();
    
private :
	AVPreviewCameraViewController* preview_camera_;
};

#endif