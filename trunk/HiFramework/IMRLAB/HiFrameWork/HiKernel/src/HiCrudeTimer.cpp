#ifdef WIN32
#include"HiKernel/HiCrudeTimer.h"


CrudeTimer* CrudeTimer::Instance()
{
	static CrudeTimer instance;

	return &instance;
}
#endif