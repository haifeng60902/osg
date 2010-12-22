/*
 *  mHiObjectControlPanel.h
 *  ISA
 *
 *  Created by rehomik on 10. 07. 11
 *  Copyright 2010 IMRLAB. All rights reserved.
 *
 */

#ifndef MART_HIFRAMEWORK_INCLUDE_HIMODULES_MHIOBJECTCONTROLPANEL_HIOBJECTCONTROLPANEL_H
#define MART_HIFRAMEWORK_INCLUDE_HIMODULES_MHIOBJECTCONTROLPANEL_HIOBJECTCONTROLPANEL_H

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#include "HiKernel/HiModule.h"
#include "HiKernel/HiTelegram.h"

@interface ObjectControlPanelViewController : UIViewController
<UIActionSheetDelegate>
{
	UIView* camera_mode_view_;
	UIView* control_mode_view_;
}

- (void)controlMode;
- (void)changeModeToControl;
- (void)changeModeToCamera;

- (void)pitchMinus;
- (void)pitchPlus;

- (void)rollMinus;
- (void)rollPlus;

- (void)scaleMinus;
- (void)scalePlus;

- (void)translationUp;
- (void)translationLeft;
- (void)translationRight;
- (void)translationDown;

- (void)yawMinus;
- (void)yawPlus;

@end


class mHiObjectControlPanel : public HiModule
{	
public :
	mHiObjectControlPanel(HiXmlNode* __pXml = NULL);
	~mHiObjectControlPanel();
	
    virtual bool            HandleMessage(const Telegram& msg);
	virtual void            PreConfig();
	virtual void            PostConfig();
    virtual void            Render();
    virtual void            Terminate();
	virtual void            Update();
    
private :
	ObjectControlPanelViewController* panel_view_contoller_;
};

#endif