#ifndef LIBSC_CCD_SMART_CAR_H_
#define LIBSC_CCD_SMART_CAR_H_

#include "libsc/com/linear_ccd.h"
#include "libsc/smart_car.h"

namespace libsc
{

class CcdSmartCar : public SmartCar
{
public:
	const bool* SampleCcd()
	{
		return m_ccd.SampleData();
	}

private:
	LinearCcd m_ccd;
};

}

#endif /* LIBSC_CCD_SMART_CAR_H_ */
