#ifndef MK60_SCCB_H_
#define MK60_SCCB_H_

#include <cstdint>

void SCCB_GPIO_init(void);
int SCCB_WriteByte(uint16_t write_addr, uint8_t send_byte);
int SCCB_ReadByte(uint8_t *buffer, uint16_t length, uint8_t read_addr);

#endif /* MK60_SCCB_H_ */
