/*
 *  HiOsgCal.h
 *  iOSG
 *
 *  Created by whitefir0 on 09. 06. 16.
 *  Copyright 2009 IMRLAB. All rights reserved.
 *
 */

#ifndef __PROJECTI_HIMODULES_MHIOSGSOUND_HIOSGSOUND_H__
#define __PROJECTI_HIMODULES_MHIOSGSOUND_HIOSGSOUND_H__

#include "HiKernel/HiModule.h"
#include "HiKernel/HiTelegram.h"

#import <AudioToolbox/AudioToolbox.h>
#import <CoreAudio/CoreAudioTypes.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>

class mHiOsgSound : public HiKernel::HiModule
{	
public :
	mHiOsgSound(HiKernel::HiXmlNode* __pXml = NULL);
	~mHiOsgSound();
	
    virtual bool            HandleMessage(const HiKernel::HiTelegram& msg);
	virtual void            PreConfig();
	virtual void            PostConfig();
    virtual void            Render();
    virtual void            Terminate();
	virtual void            Update();
	int						ParseXml(HiKernel::HiXmlNode *xml);
	
	bool SoundOnOff;
	ALCcontext* mContext;
	ALCdevice* mDevice;
	
	NSString* fileName;
	AudioFileID fileID;
	NSUInteger bufferID;
	
	NSUInteger sourceID;
	
	void initOpenAL();
	AudioFileID openAudioFile(NSString* filePath);
	UInt32 audioFileSize(AudioFileID fileDescriptor);
	

    
private :
	
};

#endif