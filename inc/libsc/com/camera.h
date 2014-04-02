/*
 * camera.h
 * Camera
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_CAMERA_H_
#define LIBSC_CAMERA_H_

#include <mini_common.h>

namespace libsc
{

class Camera
{
public:
	virtual ~Camera()
	{}

	virtual bool Init() = 0;

	virtual void ShootOnce() = 0;
	virtual void ShootContinuously() = 0;
	virtual void StopShoot() = 0;

	virtual bool IsImageReady() const = 0;
	virtual const Byte* GetImage() = 0;
	virtual Uint GetImageW() const = 0;
	virtual Uint GetImageH() const = 0;
};

}

#endif /* LIBSC_CAMERA_H_ */
