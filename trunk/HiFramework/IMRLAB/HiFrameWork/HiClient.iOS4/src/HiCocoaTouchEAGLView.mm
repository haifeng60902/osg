//
//  EAGLView.m
//  OSG_2.6EX
//
//  Created by 김홍준 on 08. 09. 22.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "HiCocoaTouchEAGLView.h"
#include "HiKernel/HiViewer.h"
#include "HiKernel/HiEventManager.h"
#include "HiKernel/HiOpenGLES10Renderer.h"
#include "HiKernel/HiOSGRenderer.h"

#include "HiModules/enumerations.h"

@implementation EAGLView

@synthesize animationTimer;
@synthesize animationInterval;

using namespace HiKernel;


// You must implement this
+ (Class)layerClass {
	return [CAEAGLLayer class];
}

//- (id)initWithCoder:(NSCoder*)coder
//{
//	[self setMultipleTouchEnabled:YES];
//	
//	if ((self = [super initWithCoder:coder])) 
//	{
//        self.backgroundColor = [UIColor clearColor];
//        
//		HiViewer::setCAEAGLLayer((CAEAGLLayer *)self.layer);
//		
//		NSString* _initFilePath = [[NSBundle mainBundle] pathForResource: @"HiFramework" ofType: @"xml"];
//		m_pHiViewer = HiViewer::GetInstance([_initFilePath cStringUsingEncoding:1]);
//		m_pHiViewer->Config();
//	}
//	
//	return self;
//}

- (id)initWithFrame:(CGRect)frame
{
    [self setMultipleTouchEnabled:YES];
	
	if ((self = [super initWithFrame:frame])) 
	{
        self.backgroundColor = [UIColor clearColor];
		
        
#if defined ( HI_OSG_RENDERER)
		osgViewer::GraphicsWindowiPhone::setCAEAGLLayer((CAEAGLLayer *)self.layer);
		static_cast<HiOSGRenderer*>(HiViewerInstance->GetRenderer())->resizeFromLayer((CAEAGLLayer *)self.layer);
#endif
		
#if defined ( HI_GLES1_RENDERER)
		static_cast<HiOpenGLES10Renderer*>(HiViewer::GetInstance()->GetRenderer())->resizeFromLayer((CAEAGLLayer *)self.layer);
#endif
	}
	
	animationInterval = 1.0/60.0;
	
	return self;
}

// 매 프레임 호출됨.
- (void)drawView
{
	HiViewerInstance->Run();
}

- (CGFloat)distanceBetweenTwoPoints:(CGPoint)fromPoint toPoint:(CGPoint)toPoint {
	
	float x = toPoint.x - fromPoint.x;
	float y = toPoint.y - fromPoint.y;
	
	return sqrt(x * x + y * y);
}

- (void)layoutSubviews {

	[self drawView];
}


- (void)startAnimation {
	self.animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval target:self selector:@selector(drawView) userInfo:nil repeats:YES];
}


- (void)stopAnimation {
	self.animationTimer = nil;
}


- (void)setAnimationTimer:(NSTimer *)newTimer {
	[animationTimer invalidate];
	animationTimer = newTimer;
}


- (void)setAnimationInterval:(NSTimeInterval)interval {
	
	animationInterval = interval;
	if (animationTimer) {
		[self stopAnimation];
		[self startAnimation];
	}
}

- (void)dealloc
{
	[super dealloc];
}

@end
