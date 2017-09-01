/*
 * touchscreen_lcd.h
 *
 *  Created on: Aug 5, 2017
 *      Author: LeeChunHei
 */

#ifndef INC_TOUCHSCREEN_LCD_H_
#define INC_TOUCHSCREEN_LCD_H_

#include "libbase/k60/hardware.h"

#include <string.h>
#include <cassert>
#include <cstddef>
#include <cstdint>

#include <bitset>
#include <functional>

#include <libbase/k60/gpio.h>
#include <libbase/k60/cmsis/mk60f15.h>
#include "libbase/k60/misc_utils.h"
#include <libsc/system.h>
#include "libbase/k60/pin.h"
#include "libbase/k60/pin_isr_manager.h"
#include "libbase/k60/pin_utils.h"
#include "libbase/k60/pinout.h"
#include <libbase/k60/i2c_master.h>
#include <libbase/k60/sim.h>

#include "libutil/misc.h"

using namespace libutil;

namespace libsc {
namespace k60 {

class TouchScreenLcd {
private:
	libbase::k60::Gpo* LCD_RST;
	libbase::k60::Gpo* backlight;
	libbase::k60::I2cMaster* touchscreen_i2c;
	libbase::k60::Gpo* Touch_RST;
	libbase::k60::Gpi* Touch_Interrupt;

	uint8_t dir;			//0:vertical; 1:horizontal
	uint16_t ramcmd;		//Write gram command
	uint16_t setxcmd;		//set x coor cmd
	uint16_t setycmd;		//set y coor cmd

	uint16_t LCD_RD_DATA(void);
	void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
	uint16_t LCD_ReadReg(uint16_t LCD_Reg);
	void LCD_WriteRAM(uint16_t RGB_Code);
	void LCD_Scan_Dir(uint8_t dir);
	uint32_t LCD_Pow(uint8_t m, uint8_t n);

	const uint8_t GT9147_CFG_TBL[184] = { 0X60, 0XE0, 0X01, 0X20, 0X03, 0X05, 0X35, 0X00, 0X02, 0X08, 0X1E, 0X08, 0X50, 0X3C, 0X0F, 0X05, 0X00, 0X00, 0XFF, 0X67, 0X50, 0X00, 0X00, 0X18, 0X1A, 0X1E, 0X14, 0X89, 0X28, 0X0A, 0X30, 0X2E, 0XBB, 0X0A, 0X03, 0X00, 0X00, 0X02, 0X33, 0X1D, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X32, 0X00, 0X00, 0X2A, 0X1C, 0X5A, 0X94, 0XC5, 0X02, 0X07, 0X00, 0X00, 0X00, 0XB5, 0X1F, 0X00, 0X90, 0X28, 0X00, 0X77, 0X32, 0X00, 0X62, 0X3F, 0X00, 0X52, 0X50, 0X00, 0X52, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X0F, 0X0F, 0X03, 0X06, 0X10, 0X42, 0XF8, 0X0F, 0X14, 0X00, 0X00, 0X00, 0X00, 0X1A, 0X18, 0X16, 0X14, 0X12, 0X10, 0X0E, 0X0C, 0X0A, 0X08, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X29, 0X28, 0X24, 0X22, 0X20, 0X1F, 0X1E, 0X1D, 0X0E, 0X0C, 0X0A, 0X08, 0X06, 0X05, 0X04, 0X02, 0X00, 0XFF, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF };
	const uint16_t GT9147_TPX_TBL[5] = { 0X8150, 0X8158, 0X8160, 0X8168, 0X8170 };

	uint8_t GT9147_Send_Cfg(uint8_t mode);
	bool GT9147_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len);
	void GT9147_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len);

public:
	//color
	static constexpr uint16_t WHITE = 0xFFFF;
	static constexpr uint16_t BLACK = 0x0000;
	static constexpr uint16_t BLUE = 0x001F;
	static constexpr uint16_t BRED = 0XF81F;
	static constexpr uint16_t GRED = 0XFFE0;
	static constexpr uint16_t GBLUE = 0X07FF;
	static constexpr uint16_t RED = 0xF800;
	static constexpr uint16_t MAGENTA = 0xF81F;
	static constexpr uint16_t GREEN = 0x07E0;
	static constexpr uint16_t CYAN = 0x7FFF;
	static constexpr uint16_t YELLOW = 0xFFE0;
	static constexpr uint16_t BROWN = 0XBC40;
	static constexpr uint16_t BRRED = 0XFC07;
	static constexpr uint16_t GRAY = 0X8430;
	static constexpr uint16_t DARKBLUE = 0X01CF;
	static constexpr uint16_t LIGHTBLUE = 0X7D7C;
	static constexpr uint16_t GRAYBLUE = 0X5458;
	static constexpr uint16_t LIGHTGREEN = 0X841F;
	static constexpr uint16_t LGRAY = 0XC618;
	static constexpr uint16_t LGRAYBLUE = 0XA651;
	static constexpr uint16_t LBBLUE = 0X2B12;
	uint16_t POINT_COLOR = WHITE;
	uint16_t BACK_COLOR = BLACK;

	uint16_t width;			//LCD width
	uint16_t height;		//LCD height
	uint16_t upper_bound;	//LCD upper bound
	uint16_t left_bound;	//LCD left bound

	TouchScreenLcd();

	void DisplayOn(void);
	void DisplayOff(void);
	void SetCursor(uint16_t Xpos, uint16_t Ypos);
	void DrawPoint(uint16_t x, uint16_t y);
	void DrawPoint(uint16_t x, uint16_t y, uint16_t color);
	void Display_Dir(uint8_t dir);
	void Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);
	void Clear(uint16_t color);
	void Fill(int16_t sx, int16_t sy, int16_t ex, int16_t ey, uint16_t color);
	void FillBuffer(int16_t x, int16_t y, uint16_t width, uint16_t height, const uint16_t zero_color, const uint16_t one_color, uint8_t* buf);
	void Fill_Color_Buffer(int16_t sx, int16_t sy, int16_t ex, int16_t ey, const uint16_t *color);
	void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void DrawRectangle(int16_t x1, int16_t y1, uint16_t x2, uint16_t y2);
	void DrawCircle(int16_t x0, int16_t y0, uint8_t r);
	void ShowChar(int16_t x, int16_t y, uint8_t num, uint8_t size, uint8_t mode);
	void ShowNum(int16_t x, int16_t y, uint32_t num, uint8_t len, uint8_t size);
	void ShowxNum(int16_t x, int16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode);
	void ShowString(int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint8_t mode);

	uint16_t touch_x[5];
	uint16_t touch_y[5];
	uint8_t touch_status;

	bool Scan(uint8_t mode);

};

}
}




#endif /* INC_TOUCHSCREEN_LCD_H_ */
