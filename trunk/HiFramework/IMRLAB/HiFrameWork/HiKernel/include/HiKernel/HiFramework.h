#pragma once

#ifdef HI_WIN32
#include <windows.h>
#endif


#ifdef HI_GLES1_RENDERER

#ifdef HI_IOS4
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#endif

#endif

// osg include
#if defined ( HI_OSG_RENDERER ) 
#if defined ( HI_WIN32)
#pragma comment(lib, "OpenThreadsd.lib")
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgDBd.lib")
//#pragma comment(lib, "osgFXd.lib")
#pragma comment(lib, "osgGAd.lib")
//#pragma comment(lib, "osgManipulatord.lib")

//#pragma comment(lib, "osgParticled.lib")
//#pragma comment(lib, "osgShadowd.lib")
#pragma comment(lib, "osgSimd.lib")
//#pragma comment(lib, "osgTerraind.lib")

#pragma comment(lib, "osgTextd.lib")
#pragma comment(lib, "osgUtild.lib")
#pragma comment(lib, "osgViewerd.lib")
//#pragma comment(lib, "osgWidgetd.lib")

#endif
#endif // HI_OSG_RENDERER


#if defined ( HI_GB_RENDERER )

#include "HiKernel/HiGBRenderer.h"

#include <NiMain.h>
#include <NiAnimation.h>
#include <NiParticle.h>
#include <NiApplication.h>
#include <NiCollision.h>
#include <NiDebug.h>

#pragma comment(lib, "NiSystem.lib")
#pragma comment(lib, "NiMain.lib")
#pragma comment(lib, "NiAnimation.lib")
#pragma comment(lib, "NiCollision.lib")
#pragma comment(lib, "NiParticle.lib")
#pragma comment(lib, "NiInput.lib")
#pragma comment(lib, "NiApplication.lib")
#pragma comment(lib, "NiVisualTracker.lib")
#pragma comment(lib, "NiFont.lib")
#pragma comment(lib, "NiEntity.lib")
#pragma comment(lib, "NiWebMetricsOutput.lib")
#pragma comment(lib, "NiMetricsOutput.lib")

#ifndef _LIB
#define _LIB
#endif //#ifndef _LIB
#include <NSBShaderLib.h>
#include <NSFParserLib.h>

#if defined(WIN32)
#pragma comment(lib, "NiBinaryShaderLibDX9.lib")
#pragma comment(lib, "NiD3D10BinaryShaderLibD3D10.lib")
#pragma comment(lib, "NSBShaderLibDX9.lib")
#pragma comment(lib, "NSFParserLibDX9.lib")
#elif defined(_XENON)
#pragma comment(lib, "NiBinaryShaderLib.lib")
#pragma comment(lib, "NSBShaderLib.lib")
#pragma comment(lib, "NSFParserLib.lib")
#elif defined (_PS3)
#else
#error "target platform not defined"
#endif  // #if defined(WIN32)

#include <NiDX9Renderer.h>
#pragma comment(lib, "NiDX9Renderer.lib")
#pragma comment(lib, "dxguid.lib")

#include <NiD3D10Renderer.h>
#pragma comment(lib, "NiD3D10Renderer.lib")

#pragma comment(lib, "NiInput.lib")
#include <NiInput.h>
#include <NiInputSystem.h>

#endif  // HI_GB_RENDERER
//

// directX include
#if defined ( HI_DXRENDERER )

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "HiKernel/HiDXRenderer.h"

#endif // HI_DX_RENDERER
