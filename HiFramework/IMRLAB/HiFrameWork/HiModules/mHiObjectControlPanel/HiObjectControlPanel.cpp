/*
 *  mHiObjectControlPanel.cpp
 *  ISA
 *
 *  Created by rehomik on 10. 07. 10
 *  Copyright 2010 IMRLAB. All rights reserved.
 *
 */
#include "HiModules/mHiObjectControlPanel/HiObjectControlPanel.h"

#include "HiKernel/HiMessageDispatcher.h"
#include "HiModules/enumerations.h"
#include "EAGLView.h"

/*
 Implement ObjectControlPanelViewController
 */
@implementation ObjectControlPanelViewController

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{	
	if (CAMERA_MODE == buttonIndex)
	{
		[self changeModeToCamera];
		
	}else if (CONTROL_MODE == buttonIndex)
	{
		[self changeModeToControl];
	}
}

- (void)controlMode
{
	UIButton* control_button = (UIButton*)[self.view viewWithTag: TAG_CONTROL_BUTTON];
	
	if (nil != control_button)
	{
		UIActionSheet* popup_switch_panel = [[UIActionSheet alloc] initWithTitle: @"전환할 모드를 선택해주세요."
																		delegate: self
															   cancelButtonTitle: nil
														  destructiveButtonTitle: nil
															   otherButtonTitles: @"Camera Mode", @"Control Mode", nil];
		
		popup_switch_panel.actionSheetStyle = UIActionSheetStyleDefault;
		[popup_switch_panel showInView: self.view];
		[popup_switch_panel release];
	}
}

- (void)changeModeToControl
{
	camera_mode_view_.hidden = YES;
	control_mode_view_.hidden = NO;
	
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "InfomationAboutPicture",
							  INFOMATION_ABOUT_PICTURE_STOP_CAPTUREING,
							  0);
	
	UIView* root_view = self.view.superview;
	if (nil != root_view)
	{
		EAGLView* gl_view = [root_view viewWithTag: TAG_GLVIEW];
		gl_view.hidden = NO;
		[gl_view startAnimation];
	}
}

- (void)changeModeToCamera
{
	camera_mode_view_.hidden = NO;
	control_mode_view_.hidden = YES;
	
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "InfomationAboutPicture",
							  INFOMATION_ABOUT_PICTURE_START_CAPTUREING,
							  0);
	
	UIView* root_view = self.view.superview;
	if (nil != root_view)
	{
		EAGLView* gl_view = [root_view viewWithTag: TAG_GLVIEW];
		gl_view.hidden = YES;
		[gl_view stopAnimation];
	}
}

- (void)dealloc
{
	[camera_mode_view_ release];
	[control_mode_view_ release];
	
	[super dealloc];
}

- (id)init
{
    if (self = [super init])
    {
    }
    
    return self;
}

- (void)loadView
{
	self.view = [[UIView alloc] initWithFrame: CGRectMake(0, 0, 320, 480)];
	self.view.tag = TAG_CONTROL_PANEL;
	
	camera_mode_view_ = [[UIView alloc] initWithFrame: CGRectMake(0, 0, 320, 380)];
	camera_mode_view_.backgroundColor = [UIColor clearColor];
	camera_mode_view_.hidden = YES;
	
	control_mode_view_ = [[UIView alloc] initWithFrame: CGRectMake(0, 0, 320, 480)];
	control_mode_view_.backgroundColor = [UIColor clearColor];
	
	[self changeModeToControl];
	
	[self.view addSubview: camera_mode_view_];	
	[self.view addSubview: control_mode_view_];
	
	// Control mode button.
	UIButton* control_mode = [UIButton buttonWithType: UIButtonTypeInfoLight];
	control_mode.frame = CGRectMake(MODE_SELECTION_BUTTON_X,
									MODE_SELECTION_BUTTON_Y,
									MODE_SELECTION_BUTTON_WIDTH,
									MODE_SELECTION_BUTTON_HEIGHT);
	control_mode.alpha = BUTTON_ALPHA;
	control_mode.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	control_mode.contentHorizontalAlignment = UIControlContentHorizontalAlignmentLeft;
	control_mode.tag = TAG_CONTROL_BUTTON;
	
	[control_mode setTitleColor: [UIColor blackColor]
					  forState: UIControlStateNormal];
	[control_mode addTarget: self
					action: @selector(controlMode)
		  forControlEvents: UIControlEventTouchUpInside];
	
	[self.view addSubview: control_mode];
	
	// scale minus button.
	UIButton* scale_minus = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	scale_minus.frame = CGRectMake(SCALE_MINUS_BUTTON_X,
								   SCALE_MINUS_BUTTON_Y,
								   BUTTON_WIDTH,
								   BUTTON_HEIGHT);
	scale_minus.alpha = BUTTON_ALPHA;
	scale_minus.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	scale_minus.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[scale_minus setTitle: @"Scale -"
				 forState: UIControlStateNormal];
	[scale_minus setTitleColor: [UIColor blackColor]
					  forState: UIControlStateNormal];
	[scale_minus addTarget: self
					action: @selector(scaleMinus)
		  forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: scale_minus];
	
	// scale plus button.
	UIButton* scale_plus = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	scale_plus.frame = CGRectMake(SCALE_PLUS_BUTTON_X,
								  SCALE_PLUS_BUTTON_Y,
								  BUTTON_WIDTH,
								  BUTTON_HEIGHT);
	scale_plus.alpha = BUTTON_ALPHA;
	scale_plus.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	scale_plus.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[scale_plus setTitle: @"Scale +"
				forState: UIControlStateNormal];
	[scale_plus setTitleColor: [UIColor blackColor]
					 forState: UIControlStateNormal];
	[scale_plus addTarget: self
				   action: @selector(scalePlus)
		 forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: scale_plus];
	
	// yaw minus.
	UIButton* yaw_minus = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	yaw_minus.frame = CGRectMake(YAW_MINUS_BUTTON_X,
								 YAW_MINUS_BUTTON_Y,
								 BUTTON_WIDTH,
								 BUTTON_HEIGHT);
	yaw_minus.alpha = BUTTON_ALPHA;
	yaw_minus.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	yaw_minus.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[yaw_minus setTitle: @"Yaw -"
			   forState: UIControlStateNormal];
	[yaw_minus setTitleColor: [UIColor blackColor]
					forState: UIControlStateNormal];
	[yaw_minus addTarget: self
				  action: @selector(yawMinus)
		forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: yaw_minus];
	
	// yaw plus.
	UIButton* yaw_plus = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	yaw_plus.frame = CGRectMake(YAW_PLUS_BUTTON_X,
								YAW_PLUS_BUTTON_Y,
								BUTTON_WIDTH,
								BUTTON_HEIGHT);
	yaw_plus.alpha = BUTTON_ALPHA;
	yaw_plus.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	yaw_plus.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[yaw_plus setTitle: @"Yaw +"
			  forState: UIControlStateNormal];
	[yaw_plus setTitleColor: [UIColor blackColor]
				   forState: UIControlStateNormal];
	[yaw_plus addTarget: self
				 action: @selector(yawPlus)
	   forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: yaw_plus];
	
	// pitch minus.
	UIButton* pitch_minus = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	pitch_minus.frame = CGRectMake(PITCH_MINUS_BUTTON_X,
								   PITCH_MINUS_BUTTON_Y,
								   BUTTON_WIDTH,
								   BUTTON_HEIGHT);
	pitch_minus.alpha = BUTTON_ALPHA;
	pitch_minus.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	pitch_minus.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[pitch_minus setTitle: @"Pitch -"
				 forState: UIControlStateNormal];
	[pitch_minus setTitleColor: [UIColor blackColor]
					  forState: UIControlStateNormal];
	[pitch_minus addTarget: self
					action: @selector(pitchMinus)
		  forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: pitch_minus];
	
	// pitch plus.
	UIButton* pitch_plus = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	pitch_plus.frame = CGRectMake(PITCH_PLUS_BUTTON_X,
								  PITCH_PLUS_BUTTON_Y,
								  BUTTON_WIDTH,
								  BUTTON_HEIGHT);
	pitch_plus.alpha = BUTTON_ALPHA;
	pitch_plus.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	pitch_plus.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[pitch_plus setTitle: @"Pitch +"
				forState: UIControlStateNormal];
	[pitch_plus setTitleColor: [UIColor blackColor]
					 forState: UIControlStateNormal];
	[pitch_plus addTarget: self
				   action: @selector(pitchPlus)
		 forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: pitch_plus];
	
	// roll minus.
	UIButton* roll_minus = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	roll_minus.frame = CGRectMake(ROLL_MINUS_BUTTON_X,
								  ROLL_MINUS_BUTTON_Y,
								  BUTTON_WIDTH,
								  BUTTON_HEIGHT);
	roll_minus.alpha = BUTTON_ALPHA;
	roll_minus.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	roll_minus.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[roll_minus setTitle: @"Roll -"
				forState: UIControlStateNormal];
	[roll_minus setTitleColor: [UIColor blackColor]
					 forState: UIControlStateNormal];
	[roll_minus addTarget: self
				   action: @selector(rollMinus)
		 forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: roll_minus];
	
	// roll plus.
	UIButton* roll_plus = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	roll_plus.frame = CGRectMake(ROLL_PLUS_BUTTON_X,
								 ROLL_PLUS_BUTTON_Y,
								 BUTTON_WIDTH,
								 BUTTON_HEIGHT);
	roll_plus.alpha = BUTTON_ALPHA;
	roll_plus.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	roll_plus.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[roll_plus setTitle: @"Roll +"
			   forState: UIControlStateNormal];
	[roll_plus setTitleColor: [UIColor blackColor]
					forState: UIControlStateNormal];
	[roll_plus addTarget: self
				  action: @selector(rollPlus)
		forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: roll_plus];
	
	// translation up.
	UIButton* translation_up = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	translation_up.frame = CGRectMake(TRANSLATION_UP_X,
									  TRANSLATION_UP_Y,
									  BUTTON_WIDTH,
									  BUTTON_HEIGHT);
	translation_up.alpha = BUTTON_ALPHA;
	translation_up.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	translation_up.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[translation_up setTitle: @"Up"
					forState: UIControlStateNormal];
	[translation_up setTitleColor: [UIColor blackColor]
						 forState: UIControlStateNormal];
	[translation_up addTarget: self
					   action: @selector(translationUp)
			 forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: translation_up];
	
	// translation left.
	UIButton* translation_left = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	translation_left.frame = CGRectMake(TRANSLATION_LEFT_X,
										TRANSLATION_LEFT_Y,
										BUTTON_WIDTH,
										BUTTON_HEIGHT);
	translation_left.alpha = BUTTON_ALPHA;
	translation_left.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	translation_left.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[translation_left setTitle: @"Left"
					  forState: UIControlStateNormal];
	[translation_left setTitleColor: [UIColor blackColor]
						   forState: UIControlStateNormal];
	[translation_left addTarget: self
						 action: @selector(translationLeft)
			   forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: translation_left];
	
	// translation right.
	UIButton* translation_right = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	translation_right.frame = CGRectMake(TRANSLATION_RIGHT_X,
										 TRANSLATION_RIGHT_Y,
										 BUTTON_WIDTH,
										 BUTTON_HEIGHT);
	translation_right.alpha = BUTTON_ALPHA;
	translation_right.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	translation_right.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[translation_right setTitle: @"Right"
					   forState: UIControlStateNormal];
	[translation_right setTitleColor: [UIColor blackColor]
							forState: UIControlStateNormal];
	[translation_right addTarget: self
						  action: @selector(translationRight)
				forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: translation_right];
	
	// translation down.
	UIButton* translation_down = [UIButton buttonWithType: UIButtonTypeRoundedRect];
	translation_down.frame = CGRectMake(TRANSLATION_DOWN_X,
										TRANSLATION_DOWN_Y,
										BUTTON_WIDTH,
										BUTTON_HEIGHT);
	translation_down.alpha = BUTTON_ALPHA;
	translation_down.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	translation_down.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	[translation_down setTitle: @"Down"
					  forState: UIControlStateNormal];
	[translation_down setTitleColor: [UIColor blackColor]
						   forState: UIControlStateNormal];
	[translation_down addTarget: self
						 action: @selector(translationDown)
			   forControlEvents: UIControlEventTouchUpInside];
	
	[control_mode_view_ addSubview: translation_down];
}

- (void)pitchMinus
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_PITCH_MINUS,
							  0);
}

- (void)pitchPlus
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_PITCH_PLUS,
							  0);
}

- (void)rollMinus
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_ROLL_MINUS,
							  0);
}

- (void)rollPlus
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_ROLL_PLUS,
							  0);
}

- (void)scaleMinus
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_SCALE_MINUS,
							  0);
}

- (void)scalePlus
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_SCALE_PLUS,
							  0);
}

- (void)translationUp
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_TRANSLATION_UP,
							  0);
}

- (void)translationLeft
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_TRANSLATION_LEFT,
							  0);
}

- (void)translationRight
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_TRANSLATION_RIGHT,
							  0);
}

- (void)translationDown
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_TRANSLATION_DOWN,
							  0);
}

- (void)yawMinus
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_YAW_MINUS,
							  0);
}

- (void)yawPlus
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
							  "dummy",
							  "ObjectLoader",
							  HIOBJECT_LOADER_YAW_PLUS,
							  0);
}

@end

/*
 Implement mHiObjectControlPanel
 */
mHiObjectControlPanel::mHiObjectControlPanel(HiXmlNode* __pXml)
{
	panel_view_contoller_ = 0;
}

mHiObjectControlPanel::~mHiObjectControlPanel()
{
}

bool mHiObjectControlPanel::HandleMessage(const Telegram& msg)
{
	if (MSG_SIZE <= msg.Msg)
	{
		return false;
	}
	
	if (THIS_IS_ROOT_VIEW == msg.Msg)
	{
		UIWindow* root_view = (UIWindow*)msg.ExtraInfo;
		[root_view addSubview: panel_view_contoller_.view];
		
	}
	
	return true;
}

void mHiObjectControlPanel::PreConfig()
{
}

void mHiObjectControlPanel::PostConfig()
{
	panel_view_contoller_ = [[ObjectControlPanelViewController alloc] init];
}

void mHiObjectControlPanel::Render()
{
}

void mHiObjectControlPanel::Terminate()
{
}

void mHiObjectControlPanel::Update()
{
}