/*
 * cache.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

namespace libbase
{
namespace k60
{

class Cache
{
public:
	struct Config
	{
		bool is_enable;
	};

	static Cache& Get()
	{
		static Cache inst;
		return inst;
	}

	void Init(const Config &config);

private:
	Cache();

	void InitPc();
	void InitPs();

	bool m_is_init;
};

}
}
