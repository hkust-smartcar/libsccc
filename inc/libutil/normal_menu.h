/*
 * menu.h
 *
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 *
 * Author: Leslie (LeeChunHei)
 *
 * Menu (v2.0) class
 * Console-based GUI for value tuning.
 *
 */

#ifndef INC_UTIL_MENU_H_
#define INC_UTIL_MENU_H_

#include <functional>
#include <string.h>
#include <vector>
#include <cstdio>
#include <memory>
#include <libsc/system.h>
#include <libsc/joystick.h>
#include <libsc/battery_meter.h>
#include <libsc/st7735r.h>
#include <libsc/lcd_console.h>
#include <libbase/k60/flash.h>

namespace libutil {

using namespace std;

class Menu {
private:
	/*
	 * Type of the item's data
	 */
	enum var_type {
		uint8, int8, uint16, int16, uint32, int32, flp, boolean, menu, func
	};

	struct Items;

	/*
	 * Item structure that stores all the necessary information
	 */
	struct Item {
		char *name = nullptr;
		var_type type = menu;
		uint8_t value_index = 0;
		float interval = 0;
		char* true_text = nullptr;
		char* false_text = nullptr;
		Items *sub_menu = nullptr;
	};

	/*
	 * Items structure that stores more item, act like a sub menu
	 */
	struct Items {
		std::vector<Item> menu_items;
		char *menu_name = nullptr;
	};
public:
	/*
	 * Default contructor
	 * Pass lcd, lcd console, joystick and flash(keep the data memory even the device shut down) into menu class
	 * Pass flash as nullptr if don't want the data saved at flash
	 */
	Menu(libsc::St7735r *lcd, libsc::LcdConsole *console, libsc::Joystick *joystick, libsc::BatteryMeter *battery_meter, libbase::k60::Flash *flash);

	Items main_menu;

	/*
	 * Add items to the menu
	 * Use override function to allow different type of data added to the menu
	 *
	 * @param *menu the menu you want the item add in
	 */
	void AddItem(char *name, uint8_t *value, uint8_t interval, Items *menu);
	void AddItem(char *name, int8_t *value, uint8_t interval, Items *menu);
	void AddItem(char *name, uint16_t *value, uint16_t interval, Items *menu);
	void AddItem(char *name, int16_t *value, uint16_t interval, Items *menu);
	void AddItem(char *name, uint32_t *value, uint32_t interval, Items *menu);
	void AddItem(char *name, int32_t *value, uint32_t interval, Items *menu);
	void AddItem(char *name, float *value, float interval, Items *menu);
	void AddItem(char *name, bool *value, char *true_text, char *false_text, Items *menu);
	void AddItem(char *menu_name, Items *menu);
	void AddItem(char *name, std::function<void()> f, Items *menu);

	/*
	 * Entry of menu
	 */
	void EnterMenu(Items *menu);

private:
	libsc::St7735r *lcd;
	libsc::LcdConsole *console;
	uint8_t max_string_width = 15; //first char count as one
	uint8_t max_row = 8; //first row count as one
	libsc::Joystick *joystick;
	libsc::BatteryMeter *battery_meter;
	libbase::k60::Flash *flash;
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
	int8_t focus = 0;
	bool item_selected = false;

	void PrintItem(Item item, uint8_t row);

	void MenuAction(Items *menu);

	void ChangeItemValue(Item item);

	void Load();
	void Save();
	void Reset();
};

/*
 * Remainder!!!
 *
 * As in St7735r each row can only print 15 characters,
 * so please do not name the item's name too long,
 * or else it will broke the whole menu.
 *
 * Also the St7735r library in libsccc is modified,
 * so that this menu class can work better.
 * Check your St7735r library for function call SetCursorColumn,
 * if you can't find it, please add the following code to St7735r library:
 *
 *	 void SetCursorColumn(const uint8_t col) {
 *	 	 m_cursor_x = col;
 *	 }
 *
 */

/*
 * Manual for this menu class:
 *
 * When your code goes to the function EnterMenu(Items *menu),
 * you will see your screen print a menu there,
 * you can use the joystick to control the focus.
 * If you change to change the value of the item,
 * you need to click the joystick to trigger kSelect,
 * then the focus item should change to green background,
 * at that time you can move your joystick left or right to decrease or increase the value selected.
 * When you click the joystick again, it would change back to white background,
 * this mean you are not selecting anything now.
 * Remaind that in this situation, move the joystick to the left will exit the menu showing now.
 *
 * This menu class will automatically store the value you changed in flash,
 * so all the changes is stored even you had shut the device.
 * But this storing operation will only be trigger when you had completely exit the menu(to prevent frequent write to the flash).
 *
 * Also every time after you add some items to the menu, please remember to reset the menu.
 * The method to reset is:
 * Long click the joystick before enter the menu,
 * release when you see the screen ask if you want to reset,
 * select true to reset the flash and update your value to the menu class.
 *
 * If you forgot to reset after add items to the menu, following may happens:
 * The board keeps restarting after you exit the menu or before the menu can show up;
 * The new added item is missing;
 * Suddenly hard fault;
 *
 * So please remember to reset your menu at the right time!!!
 */

/*
 Sample code of using this class:

 //The data you want to add into menu
 uint8_t _u8bit = 8;
 int8_t _8bit = -8;
 bool _bool = true;

 //Init the menu object
 util::Menu menu(&lcd, &console, &joystick, &flash);

 //Add the data to the menu, if you want to see it at the main menu, pass object's main menu Items to this funcion
 menu.AddItem("_u8bit", &_u8bit, 1, &menu.main_menu);

 //Add a sub menu to the menu
 menu.AddItem("level2", &menu.main_menu);

 //Add data to the sub menu, as the sub menu is added as the second one, so the index is 1
 menu.AddItem("_8bit", &_8bit, 1, menu.main_menu.menu_items[1].sub_menu);

 //Add one more sub menu to the menu
 menu.AddItem("level2", &menu.main_menu);

 //Add data to the seconde sub menu, as the second menu is the third one on the main menu, so the index is 2
 //You can change the text that represent true and false;
 menu.AddItem("_bool", &_bool, "true", "false", menu.main_menu.menu_items[2].sub_menu);

 //After adding items into the menu, you can enter this menu any time every where using this function
 menu.EnterMenu(&menu.main_menu);

 */

}

#endif /* INC_UTIL_MENU_H_ */
