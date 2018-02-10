/*
 * touch_menu.cpp
 *
 *  Created on: Aug 22, 2017
 *      Author: LeeChunHei
 */

#include <libutil/touch_menu.h>

#if MK60F15
namespace libutil {

Touch_Menu::Touch_Menu(libsc::k60::TouchScreenLcd* lcd, libbase::k60::Flash* flash) {
	this->lcd = lcd;
	this->flash = flash;
	main_menu.menu_name = "main menu";
}

Touch_Menu::Touch_Menu(libsc::k60::TouchScreenLcd* lcd) {
	this->lcd = lcd;
	main_menu.menu_name = "main menu";
}

void Touch_Menu::AddItem(char *name, uint8_t *value, Menu *menu) {
	Item item;
	item.name = name;
	item.type = uint8;
	item.value_index = uint8_data.size();
	uint8_data.push_back(value);
	uint8_backup.push_back(*value);
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Touch_Menu::AddItem(char *name, int8_t *value, Menu *menu) {
	Item item;
	item.name = name;
	item.type = int8;
	item.value_index = int8_data.size();
	int8_data.push_back(value);
	int8_backup.push_back(*value);
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Touch_Menu::AddItem(char *name, uint16_t *value, Menu *menu) {
	Item item;
	item.name = name;
	item.type = uint16;
	item.value_index = uint16_data.size();
	uint16_data.push_back(value);
	uint16_backup.push_back(*value);
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Touch_Menu::AddItem(char *name, int16_t *value, Menu *menu) {
	Item item;
	item.name = name;
	item.type = int16;
	item.value_index = int16_data.size();
	int16_data.push_back(value);
	int16_backup.push_back(*value);
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Touch_Menu::AddItem(char *name, uint32_t *value, Menu *menu) {
	Item item;
	item.name = name;
	item.type = uint32;
	item.value_index = uint32_data.size();
	uint32_data.push_back(value);
	uint32_backup.push_back(*value);
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Touch_Menu::AddItem(char *name, int32_t *value, Menu *menu) {
	Item item;
	item.name = name;
	item.type = int32;
	item.value_index = int32_data.size();
	int32_data.push_back(value);
	int32_backup.push_back(*value);
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Touch_Menu::AddItem(char *name, float *value, Menu *menu) {
	Item item;
	item.name = name;
	item.type = flp;
	item.value_index = float_data.size();
	float_data.push_back(value);
	float_backup.push_back(*value);
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Touch_Menu::AddItem(char *name, bool *value, char *true_text, char *false_text, Menu *menu) {
	Item item;
	item.name = name;
	item.type = boolean;
	item.value_index = bool_data.size();
	bool_data.push_back(value);
	bool_backup.push_back(*value);
	item.true_text = true_text;
	item.false_text = false_text;
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Touch_Menu::AddItem(char *menu_name, Menu *menu) {
	Item item;
	item.name = menu_name;
	item.type = var_type::menu;
	item.sub_menu = new Menu;
	item.sub_menu[0].menu_name = menu_name;
	menu->menu_items.push_back(item);
}

void Touch_Menu::AddItem(char *name, std::function<void()> f, Menu *menu) {
	Item item;
	item.name = name;
	item.type = var_type::func;
	item.value_index = func_vector.size();
	func_vector.push_back(f);
	menu->menu_items.push_back(item);
}

void Touch_Menu::EnterMenu(Menu* menu, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t char_size) {
	libsc::System::DelayMs(100);
	Load();
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->char_size = char_size;
	lcd->Clear(libsc::k60::TouchScreenLcd::BLACK);
	lcd->POINT_COLOR = libsc::k60::TouchScreenLcd::WHITE;
	lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLUE;
	max_string_length = (width / (char_size / 2));
	max_row = height / char_size;
	lcd->upper_bound = y;
	lcd->left_bound = x;
	lcd->Fill(x, y, x + width, y + char_size, libsc::k60::TouchScreenLcd::BLUE);
	lcd->ShowString(x + (width - (strlen(menu->menu_name) * char_size / 2)) / 2, y, strlen(menu->menu_name) * char_size / 2, char_size, char_size, menu->menu_name, 0);
	lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLACK;
	lcd->upper_bound = y + char_size;
	lcd->left_bound = x;
	lcd->height = y + height;
	lcd->width = x + width;
	first_item_num = 0;
	first_row_y = y + char_size;
	focus = 0;
	for (uint8_t i = 0; i <= max_row && i < menu->menu_items.size(); i++) {
		PrintItem(menu->menu_items[i], i);
	}
	MenuAction(menu);
	Save();
	lcd->Display_Dir(0);
}

void Touch_Menu::MenuAction(Menu *menu) {
	int16_t y_moved = 0;
	int16_t x_moved = 0;
	uint16_t y_last_pos = 0;
	uint16_t x_last_pos = 0;
	bool maybe_tap = true;
	bool tapped = false;
	bool pressed = false;
	libsc::Timer::TimerInt release_time = libsc::System::Time();
	libsc::Timer::TimerInt enter_time = 0;
	uint16_t tapped_x = 0;
	uint16_t tapped_y = 0;
	while (1) {
		if (lcd->Scan(0)) {
			for (uint8_t t = 0; t < 5; t++) {
				if ((lcd->touch_status) & (1 << t)) {
					if (lcd->touch_x[t] < width && lcd->touch_y[t] < height && lcd->touch_x[t] >= x && lcd->touch_y[t] >= y) {
						if (y_last_pos || x_last_pos) {
							y_moved = lcd->touch_y[t] - y_last_pos;
							x_moved = lcd->touch_x[t] - x_last_pos;
						} else {
							maybe_tap = true;
						}
						release_time = libsc::System::Time();
						pressed = true;
						y_last_pos = lcd->touch_y[t];
						x_last_pos = lcd->touch_x[t];
					}
				}
			}
			tapped = false;
		} else if (libsc::System::Time() - release_time > 100) {
			if (std::abs(y_moved) < 10 && std::abs(x_moved) < 10 && pressed && maybe_tap && libsc::System::Time() - enter_time > 200) {
				tapped = true;
				tapped_x = x_last_pos;
				tapped_y = y_last_pos;
				pressed = false;
			}
			y_moved = 0;
			x_moved = 0;
			y_last_pos = 0;
			x_last_pos = 0;
		}
		if (y_moved || x_moved && libsc::System::Time() - enter_time > 200) {
			if (!(y_moved > 0 && !first_item_num && first_row_y == y + char_size) && !(y_moved < 0 && (menu->menu_items.size() - first_item_num) < max_row)) {
				first_row_y += y_moved;
				first_item_num -= first_row_y / char_size;
				if (first_item_num < 0) {
					first_item_num = 0;
					first_row_y = y + char_size;
				} else if ((menu->menu_items.size() - first_item_num) <= max_row) {
					first_item_num = menu->menu_items.size() - max_row;
					first_row_y = y + height % char_size;
				} else {
					first_row_y %= char_size;
				}
				for (uint8_t i = 0; i <= max_row + 1 && i + first_item_num < menu->menu_items.size(); i++) {
					PrintItem(menu->menu_items[first_item_num + i], i);
				}
			}
			if (std::abs(y_moved) > 5 || std::abs(x_moved) > 5) {
				maybe_tap = false;
			}
			if (x_moved > 30) {
				break;
			}
			y_moved = 0;
			x_moved = 0;
		}
		if (tapped && libsc::System::Time() - enter_time > 200) {
			for (uint8_t i = 0; i < 10; i++) {
				lcd->Scan(0);
				libsc::System::DelayMs(5);
			}
			if (tapped_y < y + char_size && flash) {
				Reset();
				Load();
				lcd->upper_bound = y;
				lcd->Clear(libsc::k60::TouchScreenLcd::BLACK);
				lcd->POINT_COLOR = libsc::k60::TouchScreenLcd::WHITE;
				lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLUE;
				lcd->Fill(x, y, x + width, y + char_size, libsc::k60::TouchScreenLcd::BLUE);
				lcd->ShowString(x + (width - (strlen(menu->menu_name) * char_size / 2)) / 2, y, strlen(menu->menu_name) * char_size / 2, char_size, char_size, menu->menu_name, 0);
				lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLACK;
				max_string_length = (width / (char_size / 2));
				lcd->upper_bound = y + char_size;
				for (uint8_t i = 0; i <= max_row + 1 && i + first_item_num < menu->menu_items.size(); i++) {
					PrintItem(menu->menu_items[first_item_num + i], i);
				}
			} else if ((first_item_num + (tapped_y - first_row_y) / char_size) < menu->menu_items.size()) {
				if (menu->menu_items[first_item_num + (tapped_y - first_row_y) / char_size].type == boolean) {
					*bool_data[menu->menu_items[first_item_num + (tapped_y - first_row_y) / char_size].value_index] = !*bool_data[menu->menu_items[first_item_num + (tapped_y - first_row_y) / char_size].value_index];
					PrintItem(menu->menu_items[first_item_num + (tapped_y - first_row_y) / char_size], (tapped_y - first_row_y) / char_size);
				} else if (menu->menu_items[first_item_num + (tapped_y - first_row_y) / char_size].type == var_type::menu) {
					Save();
					EnterMenu(menu->menu_items[first_item_num + (tapped_y - first_row_y) / char_size].sub_menu, x, y, width, height, char_size);
					lcd->Clear(libsc::k60::TouchScreenLcd::BLACK);
					lcd->POINT_COLOR = libsc::k60::TouchScreenLcd::WHITE;
					lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLUE;
					max_string_length = (width / (char_size / 2));
					max_row = height / char_size;
					lcd->upper_bound = y;
					lcd->left_bound = x;
					lcd->Fill(x, y, x + width, y + char_size, libsc::k60::TouchScreenLcd::BLUE);
					lcd->ShowString(x + (width - (strlen(menu->menu_name) * char_size / 2)) / 2, y, strlen(menu->menu_name) * char_size / 2, char_size, char_size, menu->menu_name, 0);
					lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLACK;
					lcd->upper_bound = y + char_size;
					lcd->left_bound = x;
					lcd->height = y + height;
					lcd->width = x + width;
					first_item_num = 0;
					first_row_y = y + char_size;
					focus = 0;
					for (uint8_t i = 0; i <= max_row && i < menu->menu_items.size(); i++) {
						PrintItem(menu->menu_items[i], i);
					}
				} else {
					lcd->upper_bound = y;
					if (menu->menu_items[first_item_num + (tapped_y - first_row_y) / char_size].type == var_type::func) {
						lcd->Clear(libsc::k60::TouchScreenLcd::BLACK);
						lcd->POINT_COLOR = libsc::k60::TouchScreenLcd::WHITE;
						lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLACK;
						std::function < void() > f = func_vector[menu->menu_items[first_item_num + (tapped_y - first_row_y) / char_size].value_index];
						f();
					} else {
						ChangeItemValue(menu->menu_items[first_item_num + (tapped_y - first_row_y) / char_size]);
					}
					tapped = false;
					lcd->Clear(libsc::k60::TouchScreenLcd::BLACK);
					lcd->POINT_COLOR = libsc::k60::TouchScreenLcd::WHITE;
					lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLUE;
					lcd->upper_bound = y;
					lcd->Fill(x, y, x + width, y + char_size, libsc::k60::TouchScreenLcd::BLUE);
					lcd->ShowString(x + (width - (strlen(menu->menu_name) * char_size / 2)) / 2, y, strlen(menu->menu_name) * char_size / 2, char_size, char_size, menu->menu_name, 0);
					lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLACK;
					lcd->upper_bound = y + char_size;
					for (uint8_t i = 0; i <= max_row + 1 && i + first_item_num < menu->menu_items.size(); i++) {
						PrintItem(menu->menu_items[first_item_num + i], i);
					}
					Save();
				}
			}
			tapped = false;
			enter_time = libsc::System::Time();
			y_moved = 0;
			x_moved = 0;
			y_last_pos = 0;
			x_last_pos = 0;
			pressed = false;
			maybe_tap = false;
			libsc::System::DelayMs(100);
			for (uint8_t i = 0; i < 10; i++) {
				lcd->Scan(0);
				libsc::System::DelayMs(5);
			}
		}
		libsc::System::DelayMs(5);
	}
}

/*
 * Print the item name and value
 */
void Touch_Menu::PrintItem(Item item, uint8_t row) {
	char buf[20] = { };
	int8_t space_needed = 0;
	if (row <= max_row + 1) {
		char item_string[100] = { };
		space_needed = max_string_length - strlen(item.name);
		switch (item.type) {
		case var_type::boolean:
			break;
		case var_type::flp:
			sprintf(buf, "%.3f", *float_data[item.value_index]);
			break;
		case var_type::int16:
			sprintf(buf, "%d", *int16_data[item.value_index]);
			break;
		case var_type::int32:
			sprintf(buf, "%d", *int32_data[item.value_index]);
			break;
		case var_type::int8:
			sprintf(buf, "%d", *int8_data[item.value_index]);
			break;
		case var_type::menu:
			break;
		case var_type::uint16:
			sprintf(buf, "%d", *uint16_data[item.value_index]);
			break;
		case var_type::uint32:
			sprintf(buf, "%d", *uint32_data[item.value_index]);
			break;
		case var_type::uint8:
			sprintf(buf, "%d", *uint8_data[item.value_index]);
			break;
		case var_type::func:
			break;
		}
		if (item.type == var_type::boolean) {
			if (*bool_data[item.value_index]) {
				space_needed -= strlen(item.true_text);
			} else {
				space_needed -= strlen(item.false_text);
			}
		} else {
			space_needed -= strlen(buf);
		}
		if (space_needed < 0) {
			space_needed = 0;
		}
		strcat(item_string, item.name);
		for (uint8_t i = 0; i < space_needed; i++) {
			strcat(item_string, " ");
		}
		if (item.type == var_type::boolean) {
			if (*bool_data[item.value_index]) {
				strcat(item_string, item.true_text);
			} else {
				strcat(item_string, item.false_text);
			}
		} else {
			strcat(item_string, buf);
		}
		lcd->ShowString(x, char_size * row + first_row_y, width, char_size, char_size, item_string, 0);
	} else {
		return;
	}
}

void Touch_Menu::ChangeItemValue(Item item) {
	lcd->Clear(libsc::k60::TouchScreenLcd::BLACK);
	lcd->POINT_COLOR = libsc::k60::TouchScreenLcd::WHITE;
	lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLUE;
	lcd->Fill(x, y, x + width, y + char_size, libsc::k60::TouchScreenLcd::BLUE);
	lcd->ShowString(x + (width - (strlen(item.name) * char_size / 2)) / 2, y, strlen(item.name) * char_size / 2, char_size, char_size, item.name, 0);
	char value[20] = { };
	bool is_positive = true;
	switch (item.type) {
	case var_type::boolean:
		break;
	case var_type::flp:
		is_positive = !(*float_data[item.value_index] < 0);
		sprintf(value, "%.3f", *float_data[item.value_index] < 0 ? -(*float_data[item.value_index]) : (*float_data[item.value_index]));
		break;
	case var_type::int16:
		is_positive = !(*int16_data[item.value_index] < 0);
		sprintf(value, "%d", *int16_data[item.value_index] < 0 ? -(*int16_data[item.value_index]) : (*int16_data[item.value_index]));
		break;
	case var_type::int32:
		is_positive = !(*int32_data[item.value_index] < 0);
		sprintf(value, "%d", *int32_data[item.value_index] < 0 ? -(*int32_data[item.value_index]) : (*int32_data[item.value_index]));
		break;
	case var_type::int8:
		is_positive = !(*int8_data[item.value_index] < 0);
		sprintf(value, "%d", *int8_data[item.value_index] < 0 ? -(*int8_data[item.value_index]) : (*int8_data[item.value_index]));
		break;
	case var_type::menu:
		break;
	case var_type::uint16:
		sprintf(value, "%d", *uint16_data[item.value_index]);
		break;
	case var_type::uint32:
		sprintf(value, "%d", *uint32_data[item.value_index]);
		break;
	case var_type::uint8:
		sprintf(value, "%d", *uint8_data[item.value_index]);
		break;
	case var_type::func:
		break;
	}
	lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLACK;
	lcd->ShowString(x + (width - (strlen(value) * char_size / 2)) / 2, y + (height - width / 3 * 4 - char_size * 2) / 2 + char_size, strlen(value) * char_size / 2, char_size, char_size, value, 0);
	lcd->ShowChar(x + (width - (strlen(value) * char_size / 2)) / 2 - char_size / 2, y + (height - width / 3 * 4 - char_size * 2) / 2 + char_size, is_positive ? '+' : '-', char_size, 0);
	lcd->Fill(x + 1, y + height - width / 3 * 4 + 1, x + width / 3 - 1, y + height - width - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->Fill(x + width / 3 + 1, y + height - width / 3 * 4 + 1, x + width / 3 * 2 - 1, y + height - width - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->Fill(x + width / 3 * 2 + 1, y + height - width / 3 * 4 + 1, x + width - 1, y + height - width - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->Fill(x + 1, y + height - width + 1, x + width / 3 - 1, y + height - width / 3 * 2 - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->Fill(x + width / 3 + 1, y + height - width + 1, x + width / 3 * 2 - 1, y + height - width / 3 * 2 - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->Fill(x + width / 3 * 2 + 1, y + height - width + 1, x + width - 1, y + height - width / 3 * 2 - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->Fill(x + 1, y + height - width / 3 * 2 + 1, x + width / 3 - 1, y + height - width / 3 - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->Fill(x + width / 3 + 1, y + height - width / 3 * 2 + 1, x + width / 3 * 2 - 1, y + height - width / 3 - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->Fill(x + width / 3 * 2 + 1, y + height - width / 3 * 2 + 1, x + width - 1, y + height - width / 3 - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->Fill(x + 1, y + height - width / 3 + 1, x + width / 3 - 1, y + height - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->Fill(x + width / 3 + 1, y + height - width / 3 + 1, x + width / 3 * 2 - 1, y + height - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->Fill(x + width / 3 * 2 + 1, y + height - width / 3 + 1, x + width - 1, y + height - 1, libsc::k60::TouchScreenLcd::LGRAY);
	lcd->POINT_COLOR = libsc::k60::TouchScreenLcd::BLACK;
	lcd->ShowChar(x + (width / 3 - char_size / 2) / 2, y + height - width / 3 * 4 + (width / 3 - char_size) / 2, '1', 48, 1);
	lcd->ShowChar(x + (width / 3 - char_size / 2) / 2 + width / 3, y + height - width / 3 * 4 + (width / 3 - char_size) / 2, '2', 48, 1);
	lcd->ShowChar(x + (width / 3 - char_size / 2) / 2 + width / 3 * 2, y + height - width / 3 * 4 + (width / 3 - char_size) / 2, '3', 48, 1);
	lcd->ShowChar(x + (width / 3 - char_size / 2) / 2, y + height - width / 3 * 4 + (width / 3 - char_size) / 2 + width / 3, '4', 48, 1);
	lcd->ShowChar(x + (width / 3 - char_size / 2) / 2 + width / 3, y + height - width / 3 * 4 + (width / 3 - char_size) / 2 + width / 3, '5', 48, 1);
	lcd->ShowChar(x + (width / 3 - char_size / 2) / 2 + width / 3 * 2, y + height - width / 3 * 4 + (width / 3 - char_size) / 2 + width / 3, '6', 48, 1);
	lcd->ShowChar(x + (width / 3 - char_size / 2) / 2, y + height - width / 3 * 4 + (width / 3 - char_size) / 2 + width / 3 * 2, '7', 48, 1);
	lcd->ShowChar(x + (width / 3 - char_size / 2) / 2 + width / 3, y + height - width / 3 * 4 + (width / 3 - char_size) / 2 + width / 3 * 2, '8', 48, 1);
	lcd->ShowChar(x + (width / 3 - char_size / 2) / 2 + width / 3 * 2, y + height - width / 3 * 4 + (width / 3 - char_size) / 2 + width / 3 * 2, '9', 48, 1);
	lcd->ShowChar(x + (width / 3 - char_size / 2) / 2, y + height - width / 3 * 4 + (width / 3 - char_size) / 2 + width, '.', 48, 1);
	lcd->ShowChar(x + (width / 3 - char_size / 2) / 2 + width / 3, y + height - width / 3 * 4 + (width / 3 - char_size) / 2 + width, '0', 48, 1);
	lcd->ShowString((width / 3 - char_size / 2 * 3) / 2 + width / 3 * 2, y + height - width / 3 * 4 + (width / 3 - char_size) / 2 + width, char_size * 3, char_size, 48, "Del", 1);
	int8_t char_i = 0;
	uint16_t y_last_pos = 0;
	uint16_t x_last_pos = 0;
	int16_t y_moved = 0;
	int16_t x_moved = 0;
	libsc::Timer::TimerInt release_time = 0;
	bool maybe_tap = true;
	bool pressed = false;
	bool tap = false;
	uint16_t tapped_x;
	uint16_t tapped_y;
	lcd->POINT_COLOR = libsc::k60::TouchScreenLcd::WHITE;
	bool dot_clicked = false;
	uint8_t float_resolution = 1;
	while (1) {
		if (lcd->Scan(0)) {
			for (uint8_t t = 0; t < 5; t++) {
				if ((lcd->touch_status) & (1 << t)) {
					if (lcd->touch_x[t] < width && lcd->touch_y[t] < height && lcd->touch_x[t] >= x && lcd->touch_y[t] >= y + char_size) {
						if (y_last_pos && x_last_pos) {
							y_moved = lcd->touch_y[t] - y_last_pos;
							x_moved = lcd->touch_x[t] - x_last_pos;
						} else {
							maybe_tap = true;
						}
						y_last_pos = lcd->touch_y[t];
						x_last_pos = lcd->touch_x[t];
						release_time = libsc::System::Time();
						pressed = true;
						tap = false;
					}
				}
			}
		} else if (libsc::System::Time() - release_time > 100) {
			if (std::abs(y_moved) < 10 && std::abs(x_moved) < 10 && pressed && maybe_tap) {
				tap = true;
				pressed = false;
				tapped_x = x_last_pos;
				tapped_y = y_last_pos;
			}
			y_last_pos = 0;
			x_last_pos = 0;
			y_moved = 0;
			x_moved = 0;
		}
		if (tap) {
			if (tapped_y > height - width / 3 * 4) {
				uint8_t row = ((tapped_y - (height - width / 3 * 4)) / (width / 3));
				uint8_t col = (tapped_x / (width / 3));
				uint8_t num = col + row * 3 + 1;
				if (num < 10) {
					value[char_i] = num + '0';
					value[++char_i] = '\0';
					if (dot_clicked) {
						for (uint8_t i = 0; i < float_resolution; i++) {
							num *= 0.1;
						}
						float_resolution++;
					}
				} else if (num == 11) {
					value[char_i] = '0';
					value[++char_i] = '\0';
					if (dot_clicked) {
						float_resolution++;
					}
				} else if (num == 10 && item.type == var_type::flp && !dot_clicked) {
					value[char_i] = '.';
					value[++char_i] = '\0';
					dot_clicked = true;
				} else if (num == 12 && char_i > 0) {
					if (dot_clicked) {
						float_resolution--;
						if (float_resolution < 1) {
							dot_clicked = false;
							float_resolution = 1;
						}
					}
					value[--char_i] = '\0';
				}
				lcd->Fill(x, y + char_size, width - 1, height - width / 3 * 4, libsc::k60::TouchScreenLcd::BLACK);
				lcd->ShowString(x + (width - (strlen(value) * char_size / 2)) / 2, y + (height - width / 3 * 4 - char_size * 2) / 2 + char_size, strlen(value) * char_size / 2, char_size, char_size, value, 0);
				lcd->ShowChar(x + (width - (strlen(value) * char_size / 2)) / 2 - char_size / 2, y + (height - width / 3 * 4 - char_size * 2) / 2 + char_size, is_positive ? '+' : '-', char_size, 0);
			} else if (tapped_y > char_size && (item.type == var_type::flp || item.type == var_type::int16 || item.type == var_type::int32 || item.type == var_type::int8)) {
				is_positive = !is_positive;
				if (char_i) {
					lcd->ShowChar(x + (width - (strlen(value) * char_size / 2)) / 2 - char_size / 2, y + (height - width / 3 * 4 - char_size * 2) / 2 + char_size, is_positive ? '+' : '-', char_size, 0);

				} else {
					lcd->ShowChar(x + (width - (strlen(value) * char_size / 2)) / 2 - char_size / 2, y + (height - width / 3 * 4 - char_size * 2) / 2 + char_size, is_positive ? '+' : '-', char_size, 0);

				}
			}
			tap = false;
			tapped_y = 0;
			tapped_x = 0;
		}
		if (x_moved > 30) {
			break;
		}
	}
	float changed_value = 0;
	bool after_dot = false;
	for (uint8_t i = 0; i < char_i ? char_i : strlen(value); i++) {
		if (value[i] == '.') {
			after_dot = true;
			float_resolution = i;
		} else if (value[i] == '\0') {
			break;
		} else {
			if (after_dot) {
				float num = value[i] - '0';
				for (uint8_t f = 0; f < i - float_resolution; f++) {
					num *= 0.1;
				}
				changed_value += num;
			} else {
				changed_value *= 10;
				changed_value += value[i] - '0';
			}
		}
	}
	switch (item.type) {
	case var_type::boolean:
		break;
	case var_type::flp:
		*float_data[item.value_index] = is_positive ? changed_value : -changed_value;
		break;
	case var_type::int16:
		*int16_data[item.value_index] = is_positive ? changed_value : -changed_value;
		break;
	case var_type::int32:
		*int32_data[item.value_index] = is_positive ? changed_value : -changed_value;
		break;
	case var_type::int8:
		*int8_data[item.value_index] = is_positive ? changed_value : -changed_value;
		break;
	case var_type::menu:
		break;
	case var_type::uint16:
		*uint16_data[item.value_index] = changed_value;
		break;
	case var_type::uint32:
		*uint32_data[item.value_index] = changed_value;
		break;
	case var_type::uint8:
		*uint8_data[item.value_index] = changed_value;
		break;
	case var_type::func:
		break;
	}
}

void Touch_Menu::Load() {
	if (flash == nullptr)
		return;

	int start = 0;
	Byte *buff = new Byte[flash_sum];
	flash->Read(buff, flash_sum);
	for (int i = 0; i < uint8_data.size(); i++) {
		uint8_t* v = uint8_data[i];
		volatile uint8_t temp = 0;
		memcpy((unsigned char*) &temp, buff + start, sizeof(*v));
		start += sizeof(*v);
		if (temp == temp)
			*v = temp;
	}
	for (int i = 0; i < uint16_data.size(); i++) {
		uint16_t* v = uint16_data[i];
		volatile uint16_t temp = 0;
		memcpy((unsigned char*) &temp, buff + start, sizeof(*v));
		start += sizeof(*v);
		if (temp == temp)
			*v = temp;
	}
	for (int i = 0; i < uint32_data.size(); i++) {
		uint32_t* v = uint32_data[i];
		volatile uint32_t temp = 0;
		memcpy((unsigned char*) &temp, buff + start, sizeof(*v));
		start += sizeof(*v);
		if (temp == temp)
			*v = temp;
	}
	for (int i = 0; i < int8_data.size(); i++) {
		int8_t* v = int8_data[i];
		volatile int8_t temp = 0;
		memcpy((unsigned char*) &temp, buff + start, sizeof(*v));
		start += sizeof(*v);
		if (temp == temp)
			*v = temp;
	}
	for (int i = 0; i < int16_data.size(); i++) {
		int16_t* v = int16_data[i];
		volatile int16_t temp = 0;
		memcpy((unsigned char*) &temp, buff + start, sizeof(*v));
		start += sizeof(*v);
		if (temp == temp)
			*v = temp;
	}
	for (int i = 0; i < int32_data.size(); i++) {
		int32_t* v = int32_data[i];
		volatile int32_t temp = 0;
		memcpy((unsigned char*) &temp, buff + start, sizeof(*v));
		start += sizeof(*v);
		if (temp == temp)
			*v = temp;
	}
	for (int i = 0; i < float_data.size(); i++) {
		float* v = float_data[i];
		volatile float temp = 0;
		memcpy((unsigned char*) &temp, buff + start, sizeof(*v));
		start += sizeof(*v);
		if (temp == temp)
			*v = temp;
	}
	for (int i = 0; i < bool_data.size(); i++) {
		bool* v = bool_data[i];
		volatile bool temp = 0;
		memcpy((unsigned char*) &temp, buff + start, sizeof(*v));
		start += sizeof(*v);
		if (temp == temp)
			*v = temp;
	}

	delete buff;
}

void Touch_Menu::Save() {
	if (flash == nullptr)
		return;

	int start = 0;
	Byte *buff = new Byte[flash_sum];
	for (int i = 0; i < uint8_data.size(); i++) {
		uint8_t* v = uint8_data[i];
		memcpy(buff + start, (unsigned char*) v, sizeof(*v));
		start += sizeof(*v);
	}
	for (int i = 0; i < uint16_data.size(); i++) {
		uint16_t* v = uint16_data[i];
		memcpy(buff + start, (unsigned char*) v, sizeof(*v));
		start += sizeof(*v);
	}
	for (int i = 0; i < uint32_data.size(); i++) {
		uint32_t* v = uint32_data[i];
		memcpy(buff + start, (unsigned char*) v, sizeof(*v));
		start += sizeof(*v);
	}
	for (int i = 0; i < int8_data.size(); i++) {
		int8_t* v = int8_data[i];
		memcpy(buff + start, (unsigned char*) v, sizeof(*v));
		start += sizeof(*v);
	}
	for (int i = 0; i < int16_data.size(); i++) {
		int16_t* v = int16_data[i];
		memcpy(buff + start, (unsigned char*) v, sizeof(*v));
		start += sizeof(*v);
	}
	for (int i = 0; i < int32_data.size(); i++) {
		int32_t* v = int32_data[i];
		memcpy(buff + start, (unsigned char*) v, sizeof(*v));
		start += sizeof(*v);
	}
	for (int i = 0; i < float_data.size(); i++) {
		float* v = float_data[i];
		memcpy(buff + start, (unsigned char*) v, sizeof(*v));
		start += sizeof(*v);
	}
	for (int i = 0; i < bool_data.size(); i++) {
		bool* v = bool_data[i];
		memcpy(buff + start, (unsigned char*) v, sizeof(*v));
		start += sizeof(*v);
	}
	flash->Write(buff, flash_sum);
	libsc::System::DelayMs(100);

	delete buff;
}

void Touch_Menu::Reset() {
	lcd->upper_bound = y;
	lcd->Fill((width - 4 * char_size) / 2 + x, y + (height - char_size * 3) / 2, (width - 4 * char_size) / 2 + x + 4 * char_size, y + (height - char_size * 3) / 2 + 3 * char_size, libsc::k60::TouchScreenLcd::BLACK);
	lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::RED;
	lcd->POINT_COLOR = libsc::k60::TouchScreenLcd::BLACK;
	lcd->ShowString((width - 4 * char_size) / 2 + x, y + (height - char_size * 3) / 2, 4 * char_size, char_size, char_size, " Reset? ", 0);
	lcd->BACK_COLOR = libsc::k60::TouchScreenLcd::BLACK;
	lcd->POINT_COLOR = libsc::k60::TouchScreenLcd::WHITE;
	lcd->ShowString((width - 3 * char_size / 2) / 2 + x, y + (height - char_size * 3) / 2 + char_size, 3 * char_size, char_size, char_size, "Yes", 0);
	lcd->ShowString((width - char_size) / 2 + x, y + (height - char_size * 3) / 2 + char_size * 2, 3 * char_size, char_size, char_size, "No", 0);
	lcd->DrawRectangle((width - 4 * char_size) / 2 + x, y + (height - char_size * 3) / 2, (width - 4 * char_size) / 2 + x + 4 * char_size, y + (height - char_size * 3) / 2 + 3 * char_size);
	lcd->DrawLine((width - 4 * char_size) / 2 + x, y + (height - char_size * 3) / 2 + char_size * 2, (width - 4 * char_size) / 2 + x + 4 * char_size, y + (height - char_size * 3) / 2 + char_size * 2);
	lcd->upper_bound = y + char_size;
	libsc::System::DelayMs(400);
	uint16_t y_last_pos = 0;
	uint16_t x_last_pos = 0;
	int16_t y_moved = 0;
	int16_t x_moved = 0;
	libsc::Timer::TimerInt release_time = 0;
	bool maybe_tap = true;
	bool pressed = false;
	bool tap = false;
	uint16_t tapped_x;
	uint16_t tapped_y;
	lcd->POINT_COLOR = libsc::k60::TouchScreenLcd::WHITE;
	while (1) {
		if (lcd->Scan(0)) {
			for (uint8_t t = 0; t < 5; t++) {
				if ((lcd->touch_status) & (1 << t)) {
					if (lcd->touch_x[t] < width && lcd->touch_y[t] < height && lcd->touch_x[t] >= x && lcd->touch_y[t] >= y + char_size) {
						if (y_last_pos && x_last_pos) {
							y_moved = lcd->touch_y[t] - y_last_pos;
							x_moved = lcd->touch_x[t] - x_last_pos;
						} else {
							maybe_tap = true;
						}
						y_last_pos = lcd->touch_y[t];
						x_last_pos = lcd->touch_x[t];
						release_time = libsc::System::Time();
						pressed = true;
						tap = false;
					}
				}
			}
		} else if (libsc::System::Time() - release_time > 100) {
			if (std::abs(y_moved) < 10 && std::abs(x_moved) < 10 && pressed && maybe_tap) {
				tap = true;
				pressed = false;
				tapped_x = x_last_pos;
				tapped_y = y_last_pos;
			}
			y_last_pos = 0;
			x_last_pos = 0;
			y_moved = 0;
			x_moved = 0;
		}
		if (tap) {
			if (tapped_x > (width - 4 * char_size) / 2 + x && tapped_x < (width - 4 * char_size) / 2 + x + 4 * char_size && tapped_y > y + (height - char_size * 3) / 2 + char_size && tapped_y < y + (height - char_size * 3) / 2 + char_size * 2) {
				if (flash == nullptr)
					return;

				int start = 0;
				Byte *buff = new Byte[flash_sum];
				for (int i = 0; i < uint8_backup.size(); i++) {
					uint8_t value = uint8_backup[i];
					uint8_t* v = &value;
					memcpy(buff + start, (unsigned char*) v, sizeof(*v));
					start += sizeof(*v);
				}
				for (int i = 0; i < uint16_backup.size(); i++) {
					uint16_t value = uint16_backup[i];
					uint16_t* v = &value;
					memcpy(buff + start, (unsigned char*) v, sizeof(*v));
					start += sizeof(*v);
				}
				for (int i = 0; i < uint32_backup.size(); i++) {
					uint32_t value = uint32_backup[i];
					uint32_t* v = &value;
					memcpy(buff + start, (unsigned char*) v, sizeof(*v));
					start += sizeof(*v);
				}
				for (int i = 0; i < int8_backup.size(); i++) {
					int8_t value = int8_backup[i];
					int8_t* v = &value;
					memcpy(buff + start, (unsigned char*) v, sizeof(*v));
					start += sizeof(*v);
				}
				for (int i = 0; i < int16_backup.size(); i++) {
					int16_t value = int16_backup[i];
					int16_t* v = &value;
					memcpy(buff + start, (unsigned char*) v, sizeof(*v));
					start += sizeof(*v);
				}
				for (int i = 0; i < int32_backup.size(); i++) {
					int32_t value = int32_backup[i];
					int32_t* v = &value;
					memcpy(buff + start, (unsigned char*) v, sizeof(*v));
					start += sizeof(*v);
				}
				for (int i = 0; i < float_backup.size(); i++) {
					float value = float_backup[i];
					float* v = &value;
					memcpy(buff + start, (unsigned char*) v, sizeof(*v));
					start += sizeof(*v);
				}
				for (int i = 0; i < bool_backup.size(); i++) {
					bool value = bool_backup[i];
					bool* v = &value;
					memcpy(buff + start, (unsigned char*) v, sizeof(*v));
					start += sizeof(*v);
				}
				flash->Write(buff, flash_sum);
				libsc::System::DelayMs(100);

				delete buff;
				break;
			} else {
				break;
			}
			tap = false;
			tapped_y = 0;
			tapped_x = 0;
		}
		if (std::abs(y_moved) > 10 || std::abs(x_moved) > 10) {
			maybe_tap = false;
		}
	}
}

}
#endif
