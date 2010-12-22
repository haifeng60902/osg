

#import <UIKit/UIKit.h>
#include "HiKernel/HiViewer.h"

//@class EAGLView;
//
//@interface iPhoneClient : NSObject <UIApplicationDelegate>
//{
//	UIWindow *window;
//	EAGLView *glView;
//}
//@property (nonatomic, retain) UIWindow *window;
//@property (nonatomic, retain) EAGLView *glView;
//
//@end



@class EAGLView;

@interface iPhoneClient : NSObject <UIApplicationDelegate>
{
	UIWindow *window;
	EAGLView *glView;
@private
	HiKernel::HiViewer* m_pHiViewer;
}
@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) EAGLView *glView;

@end
