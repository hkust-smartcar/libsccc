#ifndef LIBSC_ENCODER_H_
#define LIBSC_ENCODER_H_

namespace libsc
{

class Encoder
{
public:
	Encoder();

	uint16_t GetCount() const
	{
		return m_count;
	}
	uint16_t CountDiff(const uint16_t a) const
	{
		return 0;
	}

private:
	static volatile uint16_t m_count;
};

}

#endif /* LIBSC_ENCODER_H_ */
