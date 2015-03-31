/*
 * kalman_filter.h
 *
 * Author: Tommy Wong
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

namespace libutil
{

class KalmanFilter
{
public:
	KalmanFilter(const float q, const float r, const float x, const float p);

	float Filter(const float data);

	void SetQ(const float lpQ);
	void SetR(const float lpR);
	void SetX(const float lpX);
	void SetP(const float lpP);

private:
	void PredictState();
	void PredictCovariance();
	void UpdateState(const float z);
	void UpdateCovariance();
	void Gain();
protected:
	float m_q, m_r, m_k, m_x, m_lx, m_p, m_lp;
};

}
