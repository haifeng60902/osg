/*
 *  UsedModules.h
 *  iOSG
 *
 *  Created by whitefir0 on 09. 06. 13.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#if defined ( HI_IOS4 )


#if defined ( HI_OSG_RENDERER)

#include "HiModules/mHiIPhoneInput/mHiIPhoneInput.h"
//#include "HiModules/Game01/Game01.h"
//#include "HiModules/Game02/Game02.h"

#include "HiModules/mHiOsgCamera/mHiOsgCamera.h"
#include "HiModules/mHiOsgDummy/HiOsgDummy.h"
#include "HiModules/mHiOsgTerrain/HiOsgTerrain.h"
#include "HiModules/mHiOsgSound/HiOsgSound.h"
#include "HiModules/mHiOsgLight/HiOsgLight.h"
#include "HiModules/mHiOsgCal/HiOsgCal.h"
#include "HiModules/mHiImageObject/mHiImageObject.h"
#include "HiModules/mhiGameWorld/mhigameworld.h"

#endif

#if defined ( HI_GLES1_RENDERER)
#include "HiModules/mHiOpenGLES10Demo/mHiOpenGLES10Demo.h"
#endif


#endif