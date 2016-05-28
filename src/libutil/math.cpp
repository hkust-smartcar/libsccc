/*
 * math.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cmath>

#include "libutil/math.h"

#define SQRT_MAGIC_F 0x5F3759DF

namespace libutil
{

/* Formula From Cecil Hastings */
float Math::ArcTan(float x)
{
	float y = (fabs(x) - 1) / (fabs(x) + 1);
	float z = y * y;
    return (.785398f + (.995354f + (-.288679f + .079331f * z) * z) * y)
    		* ((x < 0) ? -1 : 1);
}


// ArcSin : http://cpp.sh/7po
//			http://cpp.sh/5fqt
float Math::ArcSin(float x)
{
	return 1.570796f - Sqrt2(1 - x) * (1.570723f - x * (0.212114f + x
			* (.074261f + x * (-.018729f))));
}

float Math::Sqrt2(const float x)
{
	const float xhalf = 0.5f * x;
	union // get bits for floating value
	{
		float x;
		int i;
	} u;
	u.x = x;
	u.i = SQRT_MAGIC_F - (u.i >> 1);  // gives initial guess y0
	// Newton step, repeating increases accuracy
	return x * u.x * (1.5f - xhalf * u.x * u.x);
}

}
