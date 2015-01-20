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

#include <vector>
#include <cstring>
#include <cfloat>

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
			k800Hz = 0<<3,
			k400Hz = 1<<3,
			k200Hz = 2<<3,
			k100Hz = 3<<3,
			k50Hz = 4<<3,
			k12_5Hz = 5<<3,
			k6_25Hz = 6<<3,
			k1_56Hz = 7<<3
		};

		enum struct PowerMode
		{
			Normal = 0,
			LowNoiseLowPower,
			HighResolution,
			LowPower
		};

		uint8_t id = 0;
		Sensitivity sens = Sensitivity::Low;
		DataLength len = DataLength::k8;
		PowerMode power_mode = PowerMode::LowNoiseLowPower;
		ODR output_data_rate = ODR::k50Hz;
	};

	explicit Mma8451q(Mma8451q::Config config);
	explicit Mma8451q();
	~Mma8451q();

	bool IsConnected();

	float GetAccelX();
	float GetAccelY();
	float GetAccelZ();

	array<float, 3> GetAccel();

private:

	SoftI2cMaster m_I2cMaster;
	Config::Sensitivity m_Sens;
	Config::DataLength m_Len;

	float m_lastAccelX = 0.0f;
	float m_lastAccelY = 0.0f;
	float m_lastAccelZ = 0.0f;

	float GetAccelScaleFactor();
	float GetAccelGeneral(const Byte MsbRegAddr);

	Byte ReadRegByte(const Byte RegAddr);
	bool WriteRegByte(const Byte RegAddr, const Byte data);
	Byte *ReadRegBytes(const Byte RegAddr, const Byte Length);
	bool WriteRegBytes(const Byte RegAddr, const Byte *data);

};

}

}

#endif /* INC_LIBSC_K60_MMA8451Q_H_ */
