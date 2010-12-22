//
//  EAGLView.h
//  OSG_2.6EX
//
//  Created by 김홍준 on 08. 09. 22.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface EAGLView: UIView
{	
	@public
	NSTimer *animationTimer;
	NSTimeInterval animationInterval;
}

@property NSTimeInterval animationInterval;
@property (nonatomic, assign) NSTimer *animationTimer;

+ (Class)layerClass;
- (void)startAnimation;
- (void)stopAnimation;
- (void)drawView;
- (void)layoutSubviews;
- (void)dealloc;

@end