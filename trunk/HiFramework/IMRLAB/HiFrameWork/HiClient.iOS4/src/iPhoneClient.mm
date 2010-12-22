
#import "iPhoneClient.h"
#import "HiCocoaTouchEAGLView.h"
#include "HiKernel/HiEventManager.h"

@implementation iPhoneClient

@synthesize window;
// @synthesize glView;

// 실질적인 main
- (void)applicationDidFinishLaunching:(UIApplication *)application
{
	NSString* _initFilePath = [[NSBundle mainBundle] pathForResource: @"HiFramework" ofType: @"xml"];
	m_pHiViewer = HiKernel::HiViewer::getInstance([_initFilePath cStringUsingEncoding:1]);
	m_pHiViewer->Config();
	
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
	glView = [[EAGLView alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
	
	HiKernel::Dispatch->GiveMessage(0, "iPhoneInput", "iPhoneInput", 0 , glView);
    
	[window addSubview:glView];
	[glView startAnimation];
	
	[glView setBackgroundColor:[UIColor redColor]];
    
	[window makeKeyAndVisible];
}

- (void)dealloc {
	m_pHiViewer->Terminate();
	delete m_pHiViewer;
	[window release];
	[super dealloc];
}

-(void) applicationWillTerminate:(UIApplication*)application
{
}


/*
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [glView startAnimation];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)dealloc
{
    [window release];
    [glView release];
	
    [super dealloc];
}

@end
 */

@end
