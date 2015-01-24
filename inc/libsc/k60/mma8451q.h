/*
 * mma8451q.h
 *
 *  Created on: Jan 16, 2015
 *      Author: Peter
 */

#ifndef INC_LIBSC_K60_MMA8451Q_H_
#define INC_LIBSC_K60_MMA8451Q_H_

#include <assert.h>
#include <libbase/k60/soft_i2c_master.h>
#include <libbase/k60/pin.h>
#include <libbase/k60/dwt.h>

#include <vector>
#include <cstring>
#include <cfloat>
#include <cmath>
#include <cstdlib>

using namespace std;
using namespace libbase::k60;

namespace libsc
{

namespace k60
{

class Mma8451q
{
public:

	struct Config : SoftI2cMaster::Config
	{
		enum struct Sensitivity
		{
			High = 0,
			Mid,
			Low
		};

		enum struct DataLength
		{
			k8	= 1,
			k14	= 2
		};

		enum struct ODR
		{
			k800Hz = 0,
			k400Hz,
			k200Hz,
			k100Hz,
			k50Hz,
			k12_5Hz,
			k6_25Hz,
			k1_56Hz
		};

		enum struct PowerMode
		{
			Normal = 0,
			LowNoiseLowPower,
			HighResolution,
			LowPower
		};

		uint8_t id;
		Sensitivity sens = Sensitivity::Mid;
		DataLength len = DataLength::k14;
		PowerMode power_mode = PowerMode::Normal;
		ODR output_data_rate = ODR::k400Hz;
	};

	explicit Mma8451q();
	explicit Mma8451q(Mma8451q::Config config);
	~Mma8451q();

	bool IsConnected();

	// 13ms
	bool Update();

	float GetAccelX();
	float GetAccelY();
	float GetAccelZ();

	float GetAngleX();
	float GetAngleY();
	float GetAngleZ();

	array<float, 3> GetAccel();
	array<float, 3> GetAngle();

private:

	SoftI2cMaster m_I2cMaster;
	Config::Sensitivity m_Sens;
	Config::DataLength m_Len;

	array<float, 3> m_lastAccel;
	array<float, 3> m_lastAngle;

	// 2ms
	void GetAllAccel();
	// 0ms
	void GetAllAngle();

	float ArcTan(float x);
	float ArcSin(float x);
	float Sqrt2(const float x);

	Byte ReadRegByte(const Byte RegAddr);
	bool WriteRegByte(const Byte RegAddr, const Byte data);
	Byte *ReadRegBytes(const Byte RegAddr, const Byte Length);
	bool WriteRegBytes(const Byte RegAddr, const Byte *data);

};

}

}

#endif /* INC_LIBSC_K60_MMA8451Q_H_ */
