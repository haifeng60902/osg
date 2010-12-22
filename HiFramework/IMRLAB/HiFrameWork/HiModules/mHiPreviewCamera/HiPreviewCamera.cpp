/*
 *  HiPreviewCamera.cpp
 *  ISA
 *
 *  Created by rehomik on 10. 07. 09
 *  Copyright 2010 IMRLAB. All rights reserved.
 *
 */
#include "HiModules/mHiPreviewCamera/HiPreviewCamera.h"

#include "HiModules/enumerations.h"
#include "HiKernel/HiMessageDispatcher.h"

/*
 Implement AVPreviewCameraViewController
 */
@implementation AVPreviewCameraViewController

//- (void)captureOutput:(AVCaptureOutput *)captureOutput 
//didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer 
//	   fromConnection:(AVCaptureConnection *)connection
//{
//	double current_time = CACurrentMediaTime();
//    double interval = current_time - previous_time_;
//	
//	if (interval > 2)
//	{
//		NSLog(@"video capture!");
//		
//		previous_time_ = current_time;
//	}
//}

- (id)init
{
    if (self = [super init])
    {
        preview_video_layer_ = nil;
		
		previous_time_ = 0;
    }
    
    return self;
}

- (void)loadView
{
    // create view of preview camera.
    self.view = [[UIView alloc] initWithFrame: CGRectMake(0, 0, 320, 480)];
    
    // create capture session.
    AVCaptureSession* av_capture_session = [[AVCaptureSession alloc] init];
    AVCaptureDeviceInput* capture_input =
    [AVCaptureDeviceInput deviceInputWithDevice:[AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo]
                                          error:nil];
    
    [av_capture_session addInput: capture_input];
	
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "InfomationAboutPicture",
							  GIVE_ME_THE_OUTPUT_DELEGATE,
							  (void*)av_capture_session);
	
    // create preview camera
    preview_video_layer_ = [[AVCaptureVideoPreviewLayer alloc] initWithSession: av_capture_session];
    preview_video_layer_.frame = self.view.frame;
    preview_video_layer_.orientation = AVCaptureVideoOrientationPortrait;
    
    [self.view.layer addSublayer: preview_video_layer_];
    [preview_video_layer_.session startRunning];
}

@end

/*
 Implement mHiPreviewCamera
 */
mHiPreviewCamera::mHiPreviewCamera(HiXmlNode* __pXml)
{
	preview_camera_ = 0;
}

mHiPreviewCamera::~mHiPreviewCamera()
{
}

bool mHiPreviewCamera::HandleMessage(const Telegram& msg)
{
	if (MSG_SIZE <= msg.Msg)
	{
		return false;
	}
	
	if (THIS_IS_ROOT_VIEW == msg.Msg);
	{
		UIWindow* root_view = (UIWindow*)msg.ExtraInfo;
		[root_view addSubview: preview_camera_.view];
		
	}
	
	return true;
}

void mHiPreviewCamera::PreConfig()
{
}

void mHiPreviewCamera::PostConfig()
{
	preview_camera_ = [[AVPreviewCameraViewController alloc] init];
}

void mHiPreviewCamera::Render()
{
}

void mHiPreviewCamera::Terminate()
{
	[preview_camera_ release];
}

void mHiPreviewCamera::Update()
{
}