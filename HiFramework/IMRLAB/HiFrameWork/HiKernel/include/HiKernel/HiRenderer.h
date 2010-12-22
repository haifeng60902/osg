//
//  HiRenderer.h
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//


#pragma once

#include "HiKernel/HiObject.h"
#ifdef WIN32
#include <windows.h>
#endif
//#include <string>
//
//#ifndef __IPHONE_TOUCH__
//#include <windows.h>
//#include <d3d9.h>
//#include <d3dx9.h>
//#include <mmsystem.h>
//#endif

namespace HiKernel {
	
	class HI_DLLEXPORT HiRenderer : public HiObject
	{
	protected:
#ifdef WIN32
		HWND				m_hWnd;
#endif
		bool				m_bWindowed;
		int					m_iWidth;
		int					m_iHeight;
		bool				m_bFullscreen;
		
	public:
		enum RenderType
		{
			DirectX,
			Gamebryo,
			OpenSceneGraph,
			OpenGLES10
		};
		
		RenderType m_RenderType;
		
		HiRenderer(RenderType type) : m_RenderType(type), m_bWindowed(false), m_iWidth(640), m_iHeight(480) {};
		virtual ~HiRenderer() {};
		
#ifdef WIN32
		virtual bool Initialize(HWND hwnd, HINSTANCE hInst = NULL) = 0;
#else
		virtual bool Initialize() = 0;
#endif
		virtual void Terminate() = 0;
		virtual void BeginScene() = 0;
		virtual void EndScene() = 0;

		virtual bool IsDone()=0;
		
		inline RenderType GetRenderType() { return m_RenderType; }
		inline void SetScreen(bool bWindowed, int iWidth, int iHeight) { m_bWindowed=bWindowed; m_iWidth=iWidth; m_iHeight=iHeight; }
		inline bool GetWindowed() { return m_bWindowed; }
		inline int	GetWidth() { return m_iWidth; }
		inline int	GetHeight() { return m_iHeight; }
	};
}