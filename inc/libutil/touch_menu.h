/*
 * touch_menu.h
 *
 *  Created on: Aug 22, 2017
 *      Author: LeeChunHei
 */

#ifndef INC_UTIL_TOUCH_MENU_H_
#define INC_UTIL_TOUCH_MENU_H_

#include <functional>
#include <string.h>
#include <vector>
#include <cstdio>
#include <memory>
#include <libsc/k60/touchscreen_lcd.h>
#include <libbase/k60/flash.h>

#if MK60F15
namespace libutil {

class Touch_Menu {
private:
	/*
	 * Type of the item's data
	 */
	enum var_type {
		uint8, int8, uint16, int16, uint32, int32, flp, boolean, menu, func
	};

	struct Menu;

	/*
	 * Item structure that stores all the necessary information
	 */
	struct Item {
		char* name = nullptr;
		var_type type = menu;
		uint8_t value_index = 0;
		char* true_text = nullptr;
		char* false_text = nullptr;
		Menu* sub_menu = nullptr;
	};

	/*
	 * Menu structure that stores more items, act like a sub menu
	 */
	struct Menu {
		std::vector<Item> menu_items;
		char* menu_name = nullptr;
	};

public:
	/*
	 * Default contructor
	 * Pass touch screen lcd and flash(keep the data memory even the device shut down) into touch menu class
	 */
	Touch_Menu(libsc::k60::TouchScreenLcd* lcd, libbase::k60::Flash* flash);
	Touch_Menu(libsc::k60::TouchScreenLcd* lcd);

	Menu main_menu;

	/*
	 * Add item to the menu
	 * Use override function to allow different type of data added to the menu
	 *
	 * @param *menu the menu you want the item add in
	 */
	void AddItem(char* name, uint8_t* value, Menu* menu);
	void AddItem(char* name, int8_t* value, Menu* menu);
	void AddItem(char* name, uint16_t* value, Menu* menu);
	void AddItem(char* name, int16_t* value, Menu* menu);
	void AddItem(char* name, uint32_t* value, Menu* menu);
	void AddItem(char* name, int32_t* value, Menu* menu);
	void AddItem(char* name, float* value, Menu* menu);
	void AddItem(char* name, bool* value, char* true_text, char* false_text, Menu* menu);
	void AddItem(char* menu_name, Menu* menu);
	void AddItem(char* name, std::function<void()> f, Menu* menu);

	/*
	 * Entry of menu
	 */
	void EnterMenu(Menu* menu, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t char_size);

private:
	libsc::k60::TouchScreenLcd* lcd = nullptr;
	uint8_t max_string_width = 15; //first char count as one
	uint8_t max_row = 9; //first row count as one
	libbase::k60::Flash* flash = nullptr;
	uint16_t flash_sum = 0;
	std::vector<uint8_t*> uint8_data;
	std::vector<int8_t*> int8_data;
	std::vector<uint16_t*> uint16_data;
	std::vector<int16_t*> int16_data;
	std::vector<uint32_t*> uint32_data;
	std::vector<int32_t*> int32_data;
	std::vector<float*> float_data;
	std::vector<bool*> bool_data;
	std::vector<uint8_t> uint8_backup;
	std::vector<int8_t> int8_backup;
	std::vector<uint16_t> uint16_backup;
	std::vector<int16_t> int16_backup;
	std::vector<uint32_t> uint32_backup;
	std::vector<int32_t> int32_backup;
	std::vector<float> float_backup;
	std::vector<bool> bool_backup;
	std::vector<std::function<void()>> func_vector;
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t char_size;
	uint8_t max_string_length;
	int8_t first_item_num = 0;
	int16_t first_row_y;
	int8_t focus = 0;
	bool item_selected = false;

	void PrintItem(Item item, uint8_t row);

	void MenuAction(Menu* menu);

	void ChangeItemValue(Item item);

	void Load();
	void Save();
	void Reset();

};

}



#endif /* INC_UTIL_TOUCH_MENU_H_ */
#endif
