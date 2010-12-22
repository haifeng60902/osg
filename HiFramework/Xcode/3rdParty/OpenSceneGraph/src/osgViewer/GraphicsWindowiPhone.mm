/*
 *  GraphicsWindowiPhone.cpp
 *  OSG_2.6EX
 *
 *  Created by 김홍준 on 08. 12. 24.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#include <osg/observer_ptr>

#include <osgViewer/api/iPhone/GraphicsWindowiPhone>

#include <osg/DeleteHandler>

#define USE_DEPTH_BUFFER 1


#if defined (OSG_GLES1_AVAILABLE)
#include <OpenGLES/EAGL.h>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/EAGLDrawable.h>
#endif


#if defined(OSG_GLES2_AVAILABLE)
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif


#include <iostream>

using namespace osgViewer;


// Carbon-Eventhandler to handle the click in the close-widget and the resize of windows

namespace osgViewer
{
	
	// small helper class which maps the raw key codes to osgGA::GUIEventAdapter::Keys
	
	class OSXKeyboardMap {
		
    public:
        OSXKeyboardMap()
        {
            _keymap[53                ] =  osgGA::GUIEventAdapter::KEY_Escape;
            _keymap[115                ] =  osgGA::GUIEventAdapter::KEY_Home;
            _keymap[76                ] =  osgGA::GUIEventAdapter::KEY_KP_Enter;
            _keymap[119                ] =  osgGA::GUIEventAdapter::KEY_End;
            _keymap[36                ] =  osgGA::GUIEventAdapter::KEY_Return;
            _keymap[116                ] =  osgGA::GUIEventAdapter::KEY_Page_Up;
            _keymap[121                ] = osgGA::GUIEventAdapter::KEY_Page_Down;
            _keymap[123                ] = osgGA::GUIEventAdapter::KEY_Left;
            _keymap[124                ] = osgGA::GUIEventAdapter::KEY_Right;
            _keymap[126                ] = osgGA::GUIEventAdapter::KEY_Up;
            _keymap[125                ] = osgGA::GUIEventAdapter::KEY_Down;
            _keymap[51                ] = osgGA::GUIEventAdapter::KEY_BackSpace;
            _keymap[48                ] = osgGA::GUIEventAdapter::KEY_Tab;
            _keymap[49                ] = osgGA::GUIEventAdapter::KEY_Space;
            _keymap[117                ] = osgGA::GUIEventAdapter::KEY_Delete;
            
            _keymap[122                    ] = osgGA::GUIEventAdapter::KEY_F1;
            _keymap[120                    ] = osgGA::GUIEventAdapter::KEY_F2;
            _keymap[99                    ] = osgGA::GUIEventAdapter::KEY_F3;
            _keymap[118                    ] = osgGA::GUIEventAdapter::KEY_F4;
            _keymap[96                    ] = osgGA::GUIEventAdapter::KEY_F5;
            _keymap[97                    ] = osgGA::GUIEventAdapter::KEY_F6;
            _keymap[98                    ] = osgGA::GUIEventAdapter::KEY_F7;
            _keymap[100                    ] = osgGA::GUIEventAdapter::KEY_F8;
            _keymap[101                    ] = osgGA::GUIEventAdapter::KEY_F9;
            _keymap[109                    ] = osgGA::GUIEventAdapter::KEY_F10;
            _keymap[103                    ] = osgGA::GUIEventAdapter::KEY_F11;
            _keymap[111                    ] = osgGA::GUIEventAdapter::KEY_F12;
            
            _keymap[75                    ] = osgGA::GUIEventAdapter::KEY_KP_Divide;
            _keymap[67                    ] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
            _keymap[78                    ] = osgGA::GUIEventAdapter::KEY_KP_Subtract;
            _keymap[69                    ] = osgGA::GUIEventAdapter::KEY_KP_Add;
            _keymap[89                    ] = osgGA::GUIEventAdapter::KEY_KP_Home;
            _keymap[91                    ] = osgGA::GUIEventAdapter::KEY_KP_Up;
            _keymap[92                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Up;
            _keymap[86                    ] = osgGA::GUIEventAdapter::KEY_KP_Left;
            _keymap[87                    ] = osgGA::GUIEventAdapter::KEY_KP_Begin;
            _keymap[88                    ] = osgGA::GUIEventAdapter::KEY_KP_Right;
            _keymap[83                    ] = osgGA::GUIEventAdapter::KEY_KP_End;
            _keymap[84                    ] = osgGA::GUIEventAdapter::KEY_KP_Down;
            _keymap[85                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Down;
            _keymap[82                    ] = osgGA::GUIEventAdapter::KEY_KP_Insert;
            _keymap[65                    ] = osgGA::GUIEventAdapter::KEY_KP_Delete;
			
        }
        
        ~OSXKeyboardMap() {
        }
        
        unsigned int remapKey(unsigned int key, unsigned int rawkey)
        {
            KeyMap::iterator itr = _keymap.find(rawkey);
            if (itr == _keymap.end()) return key;
            else return itr->second;
        }
    private:
        typedef std::map<unsigned int, osgGA::GUIEventAdapter::KeySymbol> KeyMap;
        KeyMap _keymap;
	};
	
	/** remaps a native os x keycode to a GUIEventAdapter-keycode */
	static unsigned int remapOSXKey(unsigned int key, unsigned int rawkey)
	{
		static OSXKeyboardMap s_OSXKeyboardMap;
		return s_OSXKeyboardMap.remapKey(key,rawkey);
	}
	
#pragma mark * * * OSXWindowingSystemInterface * * * 
	
	struct OSXiPhoneWindowingSystemInterface : public osg::GraphicsContext::WindowingSystemInterface
	{
		
		/** ctor, get a list of all attached displays **/
		OSXiPhoneWindowingSystemInterface() 
		{
		}
		
		/** dtor **/
		~OSXiPhoneWindowingSystemInterface()
		{
			if (osg::Referenced::getDeleteHandler())
			{
				osg::Referenced::getDeleteHandler()->setNumFramesToRetainObjects(0);
				osg::Referenced::getDeleteHandler()->flushAll();
			}
		}
		/** @return count of attached screens **/
		virtual unsigned int getNumScreens(const osg::GraphicsContext::ScreenIdentifier& si) 
		{
			return 1;
		}
		
		
		/** returns the resolution of a specific display **/
		virtual void getScreenResolution(const osg::GraphicsContext::ScreenIdentifier& si, unsigned int& width, unsigned int& height)
		{
#if defined (HI_IPAD)
			width = 768;
			height = 1024;
#elif defined (HI_IPHONE)
			width = 320;
			height = 480;			
#endif
		}
		
		/** return the top left coord of a specific screen in global screen space **/
		void getScreenTopLeft(const osg::GraphicsContext::ScreenIdentifier& si, int& x, int& y) 
		{
			x = 0;
			y = 0;
			// osg::notify(osg::DEBUG_INFO) << "topleft of screen " << si.screenNum <<" " << bounds.origin.x << "/" << bounds.origin.y << std::endl;
		}
		
		/** helper method to get a value out of a CFDictionary **/
		static double getDictDouble (CFDictionaryRef refDict, CFStringRef key)
		{
			double double_value;
			CFNumberRef number_value = (CFNumberRef) CFDictionaryGetValue(refDict, key);
			if (!number_value) // if can't get a number for the dictionary
				return -1;  // fail
			if (!CFNumberGetValue(number_value, kCFNumberDoubleType, &double_value)) // or if cant convert it
				return -1; // fail
			return double_value; // otherwise return the long value
		}
		
		
		/** implementation of setScreenResolution **/
		
		virtual bool setScreenResolution(const osg::GraphicsContext::ScreenIdentifier& screenIdentifier, unsigned int width, unsigned int height) 
		{ 
#if defined (HI_IPAD)
			width = 768;
			height = 1024;
#elif defined (HI_IPHONE)
			width = 320;
			height = 480;			
#endif
			return true; 
		}
		
		
		/** implementation of setScreenRefreshRate **/
		
		virtual bool setScreenRefreshRate(const osg::GraphicsContext::ScreenIdentifier& screenIdentifier, double refreshRate) { 
			
			return 1;
		}
		
		virtual osg::GraphicsContext* createGraphicsContext(osg::GraphicsContext::Traits* traits)
		{
			{
				osg::ref_ptr<osgViewer::GraphicsWindowiPhone> window = new GraphicsWindowiPhone(traits);
				if (window->valid()) return window.release();
				else return 0;
			}
		}
		
		
		
		void enumerateScreenSettings(const osg::GraphicsContext::ScreenIdentifier& screenIdentifier, osg::GraphicsContext::ScreenSettingsList & resolutionList) {
			
			/*
			// Warning! This method has not been tested.
			resolutionList.clear();
			
			CGDirectDisplayID displayid = getDisplayID(screenIdentifier);
			CFArrayRef availableModes = CGDisplayAvailableModes(displayid);
			unsigned int numberOfAvailableModes = CFArrayGetCount(availableModes);
			for (unsigned int i=0; i<numberOfAvailableModes; ++i) {
				// look at each mode in the available list
				CFDictionaryRef mode = (CFDictionaryRef)CFArrayGetValueAtIndex(availableModes, i);
				osg::GraphicsContext::ScreenSettings tmpSR;
				
				long width = getDictLong(mode, kCGDisplayWidth);
				tmpSR.width = width<=0 ? 0 : width;
				long height = getDictLong(mode, kCGDisplayHeight);
				tmpSR.height = height<=0 ? 0 : height;
				long rate = getDictLong(mode, kCGDisplayRefreshRate);
				tmpSR.refreshRate = rate<=0 ? 0 : rate;
				long depth = getDictLong(mode, kCGDisplayBitsPerPixel);
				tmpSR.colorDepth = depth<=0 ? 0 : depth;
				
				resolutionList.push_back(tmpSR);
			}
			 */
		}
		

		void getScreenSettings(const osg::GraphicsContext::ScreenIdentifier& si, osg::GraphicsContext::ScreenSettings & resolution)
		{
			/*
			CGDirectDisplayID id = getDisplayID(si);
			resolution.width = CGDisplayPixelsWide(id);
			resolution.height = CGDisplayPixelsHigh(id);
			resolution.colorDepth = CGDisplayBitsPerPixel(id);
			resolution.refreshRate = getDictDouble (CGDisplayCurrentMode(id), kCGDisplayRefreshRate);        // Not tested
			if (resolution.refreshRate<0) resolution.refreshRate = 0;
			 */
		}
		
		
	
		
    private:
        
	};
	
}


#pragma mark * * * GraphicsWindowiPhone * * *
//deu+
CAEAGLLayer* GraphicsWindowiPhone::eaglLayer = NULL;

//deu+
void GraphicsWindowiPhone::setCAEAGLLayer(CAEAGLLayer *_eaglLayer)
{
	eaglLayer = _eaglLayer;
}

void GraphicsWindowiPhone::init()
{
    if (_initialized) return;
	
    // getEventQueue()->setCurrentEventState(osgGA::GUIEventAdapter::getAccumulatedEventState().get());
    
    _lastModifierKeys = 0;
    _windowTitleHeight = 0;
    _closeRequested = false;
    _ownsWindow = false;
    _context = NULL;
	
	_valid = true;
    _initialized = true;
}

bool GraphicsWindowiPhone::realizeImplementation()
{
	if (_realized) return true;
    if (!_initialized)
    {
        init();
        if (!_initialized) return false;
    }
    if (!_traits) return false;
    
    osg::notify(osg::INFO) << "GraphicsWindowiPhone:: realizeIMplementation" << std::endl;
    
    setWindowDecoration(_traits->windowDecoration);
    useCursor(_traits->useCursor);
	
	
#if defined(OSG_GLES1_AVAILABLE)
	
	{
		_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1/* sharegroup:globalsharegroup*/];
		[EAGLContext setCurrentContext:_context];
		glGenFramebuffersOES(1, &viewFramebuffer);
		glGenRenderbuffersOES(1, &viewRenderbuffer);
		
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
		[_context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
		
		glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
		glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
		
		if (USE_DEPTH_BUFFER) 
		{
			glGenRenderbuffersOES(1, &depthRenderbuffer);
			glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
			glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
			glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
		}
		
		if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) 
		{
			NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
			return NO;
		}
	}
#endif
	
#if defined(OSG_GLES2_AVAILABLE)
	
	
	//deu+
	{
		_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2]; // sharegroup:globalsharegroup
		[EAGLContext setCurrentContext:_context];
		glGenFramebuffers(1, &viewFramebuffer);
		glGenRenderbuffers(1, &viewRenderbuffer);
		
		glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
		[_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, viewRenderbuffer);
		
		
		
		
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
		
		if (USE_DEPTH_BUFFER) 
		{
			glGenRenderbuffers(1, &depthRenderbuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
		}
		
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
		{
			NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
			return NO;
		}
	}
#endif
	
	
    if (!_context) {
		osg::notify(osg::WARN) << "GraphicsWindowiPhone::realizeImplementation failed creating a context: "  << std::endl;
        return false;
    }
		
    makeCurrent();
	_realized = true;
    return _realized;
}



bool GraphicsWindowiPhone::makeCurrentImplementation()
{
#if defined(OSG_GLES1_AVAILABLE)
	bool _bool = ([EAGLContext setCurrentContext:_context] == GL_TRUE);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
#endif
	
#if defined(OSG_GLES2_AVAILABLE)
	
	bool _bool = ([EAGLContext setCurrentContext:_context] == GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer);
#endif
	return _bool;
}

bool GraphicsWindowiPhone::releaseContextImplementation()
{
    if (!_realized)
    {
        osg::notify(osg::NOTICE)<<"Warning: GraphicsWindow not realized, cannot do makeCurrent."<<std::endl;
        return false;
    }
	
    // osg::notify(osg::NOTICE)<<"makeCurrentImplementation "<<this<<" "<<OpenThreads::Thread::CurrentThread()<<std::endl;
    // osg::notify(osg::NOTICE)<<"   glXMakeCurrent ("<<_display<<","<<_window<<","<<_glxContext<<std::endl;
	
	return ([EAGLContext setCurrentContext:nil] == GL_TRUE);
	
}



void GraphicsWindowiPhone::closeImplementation()
{
    if (_context) 
    {
		if ([EAGLContext currentContext] == _context) {
			[EAGLContext setCurrentContext:nil];
		}
		
		[_context release]; 
		
        _context = NULL;
    }
}



void GraphicsWindowiPhone::swapBuffersImplementation()
{
	
#if defined(OSG_GLES1_AVAILABLE)
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[_context presentRenderbuffer:GL_RENDERBUFFER_OES];
#endif
	
#if defined(OSG_GLES2_AVAILABLE)
	glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
	[_context presentRenderbuffer:GL_RENDERBUFFER];
#endif
}



void GraphicsWindowiPhone::resizedImplementation(int x, int y, int width, int height)
{
    GraphicsContext::resizedImplementation(x, y, width, height);
}

void GraphicsWindowiPhone::grabFocusIfPointerInWindow()
{
	// TODO: implement
	osg::notify(osg::ALWAYS) << "GraphicsWindowiPhone::grabFocusIfPointerInWindow" << std::endl;
}
void GraphicsWindowiPhone::transformMouseXY(float& x, float& y)
{
    if (getEventQueue()->getUseFixedMouseInputRange())
    {
        osgGA::GUIEventAdapter* eventState = getEventQueue()->getCurrentEventState();
        x = eventState->getXmin() + (eventState->getXmax()-eventState->getXmin())*x/float(_traits->width);
        y = eventState->getYmin() + (eventState->getYmax()-eventState->getYmin())*y/float(_traits->height);
    }
}

struct RegisterWindowingSystemInterfaceProxy
{
    RegisterWindowingSystemInterfaceProxy()
    {
        osg::GraphicsContext::setWindowingSystemInterface(new osgViewer::OSXiPhoneWindowingSystemInterface());
    }
	
    ~RegisterWindowingSystemInterfaceProxy()
    {
        if (osg::Referenced::getDeleteHandler())
        {
            osg::Referenced::getDeleteHandler()->setNumFramesToRetainObjects(0);
            osg::Referenced::getDeleteHandler()->flushAll();
        }
		
        osg::GraphicsContext::setWindowingSystemInterface(0);
    }
};

RegisterWindowingSystemInterfaceProxy createWindowingSystemInterfaceProxy;

// declare C entry point for static compilation.
extern "C" void graphicswindow_Carbon(void)
{
    osg::GraphicsContext::setWindowingSystemInterface(new osgViewer::OSXiPhoneWindowingSystemInterface());
}




