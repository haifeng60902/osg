//
//  HiTelegram.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#include "HiKernel/HiTelegram.h"


using namespace HiKernel;

HiTelegram::HiTelegram():DispatchTime(-1), Sender(-1), Receiver(-1), Msg(-1)
{
}


HiTelegram::HiTelegram(double time,
		   int    sender,
		   int    receiver,
		   int    msg,
		   void*  info): DispatchTime(time),
Sender(sender),
Receiver(receiver),
Msg(msg),
ExtraInfo(info)
{
}
