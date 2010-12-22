/*
 *  HiPreviewCamera.cpp
 *  ISA
 *
 *  Created by rehomik on 10. 07. 09
 *  Copyright 2010 IMRLAB. All rights reserved.
 *
 */
#include "HiModules/mHiInfomationAboutPicture/HiInfomationAboutPicture.h"

#import <QuartzCore/QuartzCore.h>
#import "HttpManager.h"
#include "HiKernel/HiXmlNode.h"
#include "HiModules/enumerations.h"

/*
 Implement AVCaptureDelegate
 */
@implementation AVCaptureDelegate

- (void)beAddedTo: (UIView*)root_view
{
	if (nil != root_view)
	{
		[root_view addSubview: sub_view_];
		[sub_view_ release];
		
		// infomation button.		
		popup_ = [UIButton buttonWithType: UIButtonTypeRoundedRect];
		popup_.frame = CGRectMake(60, 
								  160, 
								  200, 
								  100);
		
		NSString* relic_title = [NSString stringWithFormat: @"정보를 찾는 중입니다."];
		[popup_ setTitle: relic_title forState: UIControlStateNormal];
		[root_view addSubview: popup_];
		popup_.hidden = YES;
	}
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput 
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer 
	   fromConnection:(AVCaptureConnection *)connection
{
	if (capturing_)
	{
		double current_time = CACurrentMediaTime();
		double interval = current_time - previous_time_;
		
		if (interval > 2)
		{
			////////////////////////////////////////////////////////////////////
			// make a original image.
			
			CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
			
			// Lock the image buffer
			CVPixelBufferLockBaseAddress(imageBuffer, 0);
			
			// Get information of the image
			uint8_t *baseAddress = (uint8_t *)CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 0); 
			size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer); 
			size_t width = CVPixelBufferGetWidth(imageBuffer); 
			size_t height = CVPixelBufferGetHeight(imageBuffer); 
			
			// Create Colorspace 
			CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB(); 
			CGContextRef newContext = CGBitmapContextCreate(baseAddress,
															width,
															height,
															8,
															bytesPerRow,
															colorSpace,
															kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst); 
			CGImageRef newImage = CGBitmapContextCreateImage(newContext);
			////////////////////////////////////////////////////////////////////
			
			UIImage *image = [UIImage imageWithCGImage:newImage];
			sub_view_.image = image;
			
			// resize
			UIGraphicsBeginImageContext(CGSizeMake(320, 240));
			[image drawInRect:CGRectMake(0, 0, 320, 240)];
			UIImage* compImage = UIGraphicsGetImageFromCurrentImageContext();
			UIGraphicsEndImageContext();
			
			// gray
			CGContextDrawImage(gray_context_, CGRectMake(0, 0, 320, 240), compImage.CGImage);
			UIImage *grayImage = [UIImage imageWithCGImage:CGBitmapContextCreateImage(gray_context_)];
			
	        NSData* taken_picture = UIImageJPEGRepresentation(grayImage, 0.7f);
			[self queryData: taken_picture];
			
			// Release it 
			CGContextRelease(newContext); 
			CGColorSpaceRelease(colorSpace);
			CGImageRelease(newImage);
			
			// Unlock the image buffer 
			CVPixelBufferUnlockBaseAddress(imageBuffer,0);
			
			previous_time_ = current_time;
		}
	}
}

- (void)getOutputDelegate: (AVCaptureSession*)avcapture_session
{	
    AVCaptureVideoDataOutput* capture_output_ = [[AVCaptureVideoDataOutput alloc] init];
    
    NSString *key = (NSString *)kCVPixelBufferPixelFormatTypeKey;
	NSNumber *val = [NSNumber numberWithUnsignedInt:kCVPixelFormatType_32BGRA];
	NSDictionary* video_settings = [NSDictionary dictionaryWithObject:val forKey:key];
    
	[capture_output_ setVideoSettings:video_settings];
    [capture_output_ setSampleBufferDelegate: self queue: dispatch_get_current_queue()];
    [avcapture_session addOutput: capture_output_];
	
	sub_view_ = [[UIImageView alloc] initWithFrame: CGRectMake(220, 
															   0, 
															   100, 
															   100)];
	sub_view_.transform = CGAffineTransformMakeRotation(90 * M_PI / 180);
}

- (void)httpRecvFinished:(NSData *)data
{
//	NSString* result_string;
//    
//    result_string = [[NSString alloc] initWithData: data
//                                          encoding: NSUTF8StringEncoding];
//    
//    const char* result_c_string = [result_string cStringUsingEncoding: 1];
//    [result_string release];
//    
//    HiXmlNode* result_xml = HiXmlNode::ParseString(const_cast<char*>(result_c_string));
//	
//	HiXmlNode* xml_relic_name = result_xml->FindChild("RelicName");
//	if (0 == xml_relic_name)
//	{
//		return;
//	}
//    HiXmlNode* xml_relic_value = xml_relic_name->GetChild(0);
//    
//    NSString* relic_title = [NSString stringWithUTF8String: xml_relic_value->GetValue().c_str()];
	NSString* relic_title = [NSString stringWithFormat: @"정보를 찾을 수 없습니다."];
	[popup_ setTitle: relic_title forState: UIControlStateNormal];
}

- (id)init
{
    if (self = [super init])
    {
		capturing_ = YES;
		
		previous_time_ = 0.0f;
		
		sub_view_ = nil;
		
		gray_color_space_ = CGColorSpaceCreateDeviceGray();
		gray_context_ = CGBitmapContextCreate (nil,
											 320,
											 480,
											 8,
											 0,
											 gray_color_space_,
											 kCGImageAlphaNone);
    }
    
    return self;
}

- (void)stopCapturing
{
	capturing_ = NO;
	
	sub_view_.hidden = YES;
	popup_.hidden = YES;
}

- (void)startCapturing
{
	capturing_ = YES;
	
	sub_view_.hidden = NO;
	popup_.hidden = NO;
}

- (void) queryData:(NSData *)data
{    
	if (nil == http_manager_)
	{
		http_manager_ = [[HttpManager alloc] init];
	}
	
    http_manager_.delegate = self;
    http_manager_.selector = @selector(httpRecvFinished:);
    http_manager_.method = @"POST";
    http_manager_.uri = @"http://161.122.38.253:50000/photo_ar/search";
    http_manager_.body = data;
	
	[http_manager_ openHttpConnection];
}

@end

/*
 Implement mHiInfomationAboutPicture
 */
mHiInfomationAboutPicture::mHiInfomationAboutPicture(HiXmlNode* __pXml)
{
}

mHiInfomationAboutPicture::~mHiInfomationAboutPicture()
{
}

bool mHiInfomationAboutPicture::HandleMessage(const Telegram& msg)
{
	if (MSG_SIZE <= msg.Msg)
	{
		return false;
	}
	
	if (GIVE_ME_THE_OUTPUT_DELEGATE == msg.Msg)
	{
		[capture_delegate_ getOutputDelegate: (AVCaptureSession*)msg.ExtraInfo];
		
	}else if (INFOMATION_ABOUT_PICTURE_STOP_CAPTUREING == msg.Msg)
	{
		[capture_delegate_ stopCapturing];
		
	}else if (INFOMATION_ABOUT_PICTURE_START_CAPTUREING == msg.Msg)
	{
		[capture_delegate_ startCapturing];
		
	}else if (THIS_IS_ROOT_VIEW == msg.Msg)
	{
		[capture_delegate_ beAddedTo: (UIView*)msg.ExtraInfo];
	}
	
	return true;
}

void mHiInfomationAboutPicture::PreConfig()
{
}

void mHiInfomationAboutPicture::PostConfig()
{
	capture_delegate_ = [[AVCaptureDelegate alloc] init];
}

void mHiInfomationAboutPicture::Render()
{
}

void mHiInfomationAboutPicture::Terminate()
{
	[capture_delegate_ release];
}

void mHiInfomationAboutPicture::Update()
{
}