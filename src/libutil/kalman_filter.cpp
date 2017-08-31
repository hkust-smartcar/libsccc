/*
 * kalman_filter.h
 *
 * Author: Tommy Wong
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libutil/kalman_filter.h"

namespace libutil
{

KalmanFilter::KalmanFilter(const float q, const float r, const float x,
		const float p)
		: m_q(q), m_r(r), m_k(0), m_x(x), m_lx(0), m_p(p), m_lp(0)
{}

float KalmanFilter::Filter(const float data)
{
	PredictState();
	PredictCovariance();
	Gain();
	UpdateState(data);
	UpdateCovariance();
	return m_x;
}

void KalmanFilter::SetQ(const float lpQ)
{
	m_q = lpQ;
}

void KalmanFilter::SetR(const float lpR)
{
	m_r = lpR;
}

void KalmanFilter::SetX(const float lpX)
{
	m_x = lpX;
}

void KalmanFilter::SetP(const float lpP)
{
	m_p = lpP;
}

void KalmanFilter::PredictState()
{
	m_lx = m_x;
}

void KalmanFilter::PredictCovariance()
{
	m_lp = m_p + m_q;
}

void KalmanFilter::UpdateState(const float z)
{
	m_x = m_lx + (m_k * (z - m_lx));
}

void KalmanFilter::UpdateCovariance()
{
	m_p = (1 - m_k) * m_lp;
}

void KalmanFilter::Gain()
{
	m_k = m_lp / (m_lp + m_r);
}

}
