/*
 * kalman_filter.h
 *
 * Author: Tommy Wong
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

namespace libutil
{

class KalmanFilter
{
public:
	KalmanFilter(const float q, const float r, const float x, const float p);

	float Filter(const float data);

private:
	void PredictState();
	void PredictCovariance();
	void UpdateState(const float z);
	void UpdateCovariance();
	void Gain();

	float m_q, m_r, m_k, m_x, m_lx, m_p, m_lp;
};

}
