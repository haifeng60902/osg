/*
 *  HiPreviewCamera.h
 *  ISA
 *
 *  Created by rehomik on 10. 07. 09
 *  Copyright 2010 IMRLAB. All rights reserved.
 *
 */

#ifndef MART_HIFRAMEWORK_INCLUDE_HIMODULES_MHIINFOMATIONABOUTPICTURE_MHIINFOMATIONABOUTPICTURE_H
#define MART_HIFRAMEWORK_INCLUDE_HIMODULES_MHIINFOMATIONABOUTPICTURE_MHIINFOMATIONABOUTPICTURE_H

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#include "HiKernel/HiModule.h"
#include "HiKernel/HiTelegram.h"

@class HttpManager;

@interface AVCaptureDelegate : NSObject
<AVCaptureVideoDataOutputSampleBufferDelegate>
{
	BOOL	capturing_;
	
	CGColorSpaceRef gray_color_space_;
	CGContextRef	gray_context_;
	
	HttpManager* http_manager_;
	
	double		previous_time_;
	UIButton*	popup_;
	
	UIImageView*	sub_view_;
}

- (void)beAddedTo: (UIView*)root_view;

- (void)getOutputDelegate: (AVCaptureSession*)avcapture_session;
- (void)httpRecvFinished:(NSData *)data;

- (void)stopCapturing;
- (void)startCapturing;

- (void)queryData:(NSData *)data;

@end


class mHiInfomationAboutPicture : public HiModule
{	
public :
	mHiInfomationAboutPicture(HiXmlNode* __pXml = NULL);
	~mHiInfomationAboutPicture();
	
    virtual bool            HandleMessage(const Telegram& msg);
	virtual void            PreConfig();
	virtual void            PostConfig();
    virtual void            Render();
    virtual void            Terminate();
	virtual void            Update();
    
private :
	AVCaptureDelegate* capture_delegate_;
};

#endif