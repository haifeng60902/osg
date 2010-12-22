//
//  HiDLL.h
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#pragma once

 
#if defined ( HI_IOS4 )

	#define HI_DLLEXPORT 

#else

	#if defined ( HI_DLL_EXPORT )
	#	define HI_DLLEXPORT __declspec(dllexport)
	#else
	#	define HI_DLLEXPORT __declspec(dllimport)
	#endif 

#endif