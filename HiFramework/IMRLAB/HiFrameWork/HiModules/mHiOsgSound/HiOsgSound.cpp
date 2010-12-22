 /*
 *  HiOsgCal.cpp
 *  iOSG
 *
 *  Created by whitefir0 on 09. 06. 16.
 *  Copyright 2009 IMRLAB. All rights reserved.
 *
 */

#include "HiModules/mHiOsgSound/HiOsgSound.h"
#include "enumerations.h"
#include "HiKernel/HiOSGRenderer.h"
#include "HiKernel/HiXmlNode.h"

mHiOsgSound::mHiOsgSound(HiKernel::HiXmlNode* __pXml)
{
	if(__pXml != NULL)
		ParseXml(__pXml);
	
	SoundOnOff =true;
}

mHiOsgSound::~mHiOsgSound()
{
}

int mHiOsgSound::ParseXml(HiKernel::HiXmlNode *xml)
{

	return 1;
}

bool mHiOsgSound::HandleMessage(const HiKernel::HiTelegram& msg)
{
//	if(HiModules::MSG_SIZE <= msg.Msg)
//		return false;
	
//	if(msg.Msg == HiModules::GOOD_EFFECT_SOUND)
//	{
//		alSourcePlay( good_effect_id_ );
//	}
//	else if(msg.Msg == HiModules::BAD_EFFECT_SOUND)
//	{
//		alSourcePlay( bad_effect_id_ );
//	}
    
	
	return true;
}

void mHiOsgSound::PreConfig()
{	
}

void mHiOsgSound::PostConfig()
{
	
	initOpenAL();
	
	// get the full path of the file
	fileName = [[NSBundle mainBundle] pathForResource:@"music_stage1" ofType:@"wav"];
	// first, open the file
	
	fileID = openAudioFile(fileName);//[self openAudioFile:fileName];
	
	UInt32 fileSize = audioFileSize(fileID);//[self audioFileSize:fileID];
	////////////////////////////////////////
	
	// this is where the audio data will live for the moment
	unsigned char * outData = (unsigned char*)malloc(fileSize);
	
	// this where we actually get the bytes from the file and put them
	// into the data buffer
	OSStatus result = noErr;
	result = AudioFileReadBytes(fileID, false, 0, &fileSize, outData);
	AudioFileClose(fileID); //close the file
	
	if (result != 0) NSLog(@"cannot load effect: %@",fileName);
	
	// grab a buffer ID from openAL
	alGenBuffers(1, &bufferID);
	
	// jam the audio data into the new buffer
	alBufferData(bufferID,AL_FORMAT_STEREO16,outData,fileSize,44100); 
	
	// save the buffer so I can release it later
	//[bufferStorageArray addObject:[NSNumber numberWithUnsignedInteger:bufferID]];
	
	// grab a source ID from openAL
	alGenSources(1, &sourceID); 
	
	// attach the buffer to the source
	alSourcei(sourceID, AL_BUFFER, bufferID);
	// set some basic source prefs
	alSourcef(sourceID, AL_PITCH, 1.0f);
	alSourcef(sourceID, AL_GAIN, 1.0f);
	//	if (loops)
	alSourcei(sourceID, AL_LOOPING, AL_TRUE);
	
	// store this for future use
	//	[soundDictionary setObject:[NSNumber numberWithUnsignedInt:sourceID] forKey:@"neatoSound"];	
	
	// clean up the buffer
	if (outData)
	{
		free(outData);
		outData = NULL;
	}
	
	alSourcePlay(sourceID);
	
	

}

void mHiOsgSound::Update()
{
	NSLog(@"mHiOsgSound");
	
}
void mHiOsgSound::Render()
{
}

void mHiOsgSound::Terminate()
{
	// delete the sources
	//	for (NSNumber * sourceNumber in [soundDictionary allValues]) {
	//		NSUInteger sourceID = [sourceNumber unsignedIntegerValue];
	alDeleteSources(1, &sourceID);
	//	}
	//	[soundDictionary removeAllObjects];
	
	// delete the buffers
	//	for (NSNumber * bufferNumber in bufferStorageArray) {
	//		NSUInteger bufferID = [bufferNumber unsignedIntegerValue];
	alDeleteBuffers(1, &bufferID);
	//	}
	//	[bufferStorageArray removeAllObjects];
	
	// destroy the context
	alcDestroyContext(mContext);
	// close the device
	alcCloseDevice(mDevice);
}
void mHiOsgSound::initOpenAL()
{
	// Initialization
	mDevice = alcOpenDevice(NULL); // select the "preferred device"
	if (mDevice) {
		// use the device to make a context
		mContext=alcCreateContext(mDevice,NULL);
		// set my context to the currently active one
		alcMakeContextCurrent(mContext);
	}
}
AudioFileID mHiOsgSound::openAudioFile(NSString* filePath)
{
	AudioFileID outAFID;
	// use the NSURl instead of a cfurlref cuz it is easier
	NSURL * afUrl = [NSURL fileURLWithPath:filePath];
	
	// do some platform specific stuff..
	OSStatus result = AudioFileOpenURL((CFURLRef)afUrl, kAudioFileReadPermission, 0, &outAFID);

	if (result != 0) NSLog(@"cannot openf file: %@",filePath);
	return outAFID;	
}
UInt32 mHiOsgSound::audioFileSize(AudioFileID fileDescriptor)
{
	UInt64 outDataSize = 0;
	UInt32 thePropSize = sizeof(UInt64);
	OSStatus result = AudioFileGetProperty(fileDescriptor, kAudioFilePropertyAudioDataByteCount, &thePropSize, &outDataSize);
	if(result != 0) NSLog(@"cannot find file size");
	return (UInt32)outDataSize;	
}
//
//
//bool mHiOsgSound::Create()
//{
//    ALFWInit();
//    if( AL_FALSE == ALFWInitOpenAL() )
//    {
//		ALFWShutdown();
//		return false;
//    }
//	
//    return true;
//}
//void mHiOsgSound::Destroy()
//{
//    UnloadWave();  // 로드된 Wave데이터가 있다면 해제 시켜주는 함수다.
//	
//    ALFWShutdownOpenAL();
//    ALFWShutdown();
//}
//
//// 로드된 Wave데이터가 있다면 해제 시켜주는 함수다.
//void mHiOsgSound::UnloadWave()
//{
//    // Stream Data 해제
//    if( m_pStreamData )
//    {
//		delete m_pStreamData;
//		m_pStreamData = 0;
//    }
//	
//    // Stream Play에 사용된 Wave Loader 해제
//    if( m_pWaveLoader )
//    {
//		m_pWaveLoader->DeleteWaveFile(m_waveID);
//		delete m_pWaveLoader;
//		m_pWaveLoader = 0;
//    }
//	
//    // OpenAL은 각 사운드를 ID로 관리한다.
//	// 하나의 Sound Data가 생성되면 그 Sound Data를 대표하는 하나의 ID가 보여되고,
//	// 그 ID를 통해 접근 할 수 있다.
//	// 현재 로드된 Sound Data가 있다면 해제한다.
//    if( m_uiSourceID )
//    {
//		alDeleteSources(m_uiSourceSize, &m_uiSourceID);
//		m_uiSourceID = 0;
//		m_uiSourceSize = 1;
//    }
//	
//    // Sound Data가 채워질 버퍼를 대표하는 ID들의 리스트를 해제한다.
//    if( m_pBuffersID )
//    {
//		alDeleteBuffers(m_uiBufferSize, m_pBuffersID);
//		delete[] m_pBuffersID;
//		m_pBuffersID = 0;
//		m_uiBufferSize = 1;
//    }
//}
//
//// Wave 파일을 로드한다.
//bool mHiOsgSound::LoadWaveStatic(std::string strFilename)
//{
//    m_pBuffersID = new unsigned int[m_uiBufferSize];
//	
//    alGenBuffers(m_uiBufferSize, m_pBuffersID);
//    alGenSources(m_uiSourceSize, &m_uiSourceID);
//	
//    if( AL_FALSE == ALFWLoadWaveToBuffer(strFilename.c_str(), m_pBuffersID[0]) )
//		return false;
//	
//    // Source ID의 Sound Data가 채워질 BufferID를 설정한다.
//    alSourcei(m_uiSourceID, AL_BUFFER, m_pBuffersID[0]);
//	
//    return true;
//}
//
//bool mHiOsgSound::UpdateStatic()
//{
//    if( m_iState != AL_PLAYING )
//		return false;
//	
//    Sleep(100);
//	
//    // 현재 Source의 상태를 플레이 중인지 아닌지를 얻는다.
//    alGetSourcei( m_uiSourceID, AL_SOURCE_STATE, &m_iState);
//	
//    return true;
//}
//
//// 플레이 시키는 함수
//void mHiOsgSound::PlayStatic()
//{
//    // Play Source
//    alSourcePlay( m_uiSourceID );
//    m_iState = AL_PLAYING;
//}
//
//// 스톱 시키는 함수
//void mHiOsgSound::StopStatic()
//{
//    alSourceStop(m_uiSourceID);
//    m_iState = AL_INITIAL;
//}
