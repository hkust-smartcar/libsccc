/*
 * menu.cpp
 *
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 *
 * Author: Leslie (LeeChunHei)
 *
 * Implementation for Menu (v2.0) class.
 *
 */

#include <libutil/normal_menu.h>

namespace libutil {

Menu::Menu(bool is_landscape, libsc::St7735r *lcd, libsc::LcdConsole *console, libsc::Joystick *joystick, libsc::BatteryMeter *battery_meter, libbase::k60::Flash *flash) {
	this->is_landscape = is_landscape;
	max_string_width = is_landscape ? 19 : 15;
	max_row = is_landscape ? 6 : 8;
	this->lcd = lcd;
	this->console = console;
	this->joystick = joystick;
	this->battery_meter = battery_meter;
	this->flash = flash;
	main_menu.menu_name = "main menu";
}

void Menu::AddItem(char *name, uint8_t *value, uint8_t interval, Items *menu) {
	Item item;
	item.name = name;
	item.type = uint8;
	item.value_index = uint8_data.size();
	uint8_data.push_back(value);
	uint8_backup.push_back(*value);
	item.interval = (float) interval;
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Menu::AddItem(char *name, int8_t *value, uint8_t interval, Items *menu) {
	Item item;
	item.name = name;
	item.type = int8;
	item.value_index = int8_data.size();
	int8_data.push_back(value);
	int8_backup.push_back(*value);
	item.interval = (float) interval;
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Menu::AddItem(char *name, uint16_t *value, uint16_t interval, Items *menu) {
	Item item;
	item.name = name;
	item.type = uint16;
	item.value_index = uint16_data.size();
	uint16_data.push_back(value);
	uint16_backup.push_back(*value);
	item.interval = (float) interval;
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Menu::AddItem(char *name, int16_t *value, uint16_t interval, Items *menu) {
	Item item;
	item.name = name;
	item.type = int16;
	item.value_index = int16_data.size();
	int16_data.push_back(value);
	int16_backup.push_back(*value);
	item.interval = (float) interval;
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Menu::AddItem(char *name, uint32_t *value, uint32_t interval, Items *menu) {
	Item item;
	item.name = name;
	item.type = uint32;
	item.value_index = uint32_data.size();
	uint32_data.push_back(value);
	uint32_backup.push_back(*value);
	item.interval = (float) interval;
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Menu::AddItem(char *name, int32_t *value, uint32_t interval, Items *menu) {
	Item item;
	item.name = name;
	item.type = int32;
	item.value_index = int32_data.size();
	int32_data.push_back(value);
	int32_backup.push_back(*value);
	item.interval = (float) interval;
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Menu::AddItem(char *name, float *value, float interval, Items *menu) {
	Item item;
	item.name = name;
	item.type = flp;
	item.value_index = float_data.size();
	float_data.push_back(value);
	float_backup.push_back(*value);
	item.interval = (float) interval;
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Menu::AddItem(char *name, bool *value, char *true_text, char *false_text, Items *menu) {
	Item item;
	item.name = name;
	item.type = boolean;
	item.value_index = bool_data.size();
	bool_data.push_back(value);
	bool_backup.push_back(*value);
	item.interval = 1;
	item.true_text = true_text;
	item.false_text = false_text;
	menu->menu_items.push_back(item);
	flash_sum += sizeof(*value);
}

void Menu::AddItem(char *menu_name, Items *menu) {
	Item item;
	item.name = menu_name;
	item.type = var_type::menu;
	item.sub_menu = new Items;
	item.sub_menu[0].menu_name = menu_name;
	menu->menu_items.push_back(item);
}

void Menu::AddItem(char *name, std::function<void()> f, Items *menu) {
	Item item;
	item.name = name;
	item.type = var_type::func;
	item.value_index = func_vector.size();
	func_vector.push_back(f);
	menu->menu_items.push_back(item);
}

void Menu::EnterMenu(Items *menu) {
	libsc::System::DelayMs(100);
	if (menu == &this->main_menu && joystick->GetState() == libsc::Joystick::State::kSelect) {
		Reset();
	}
	if (menu == &this->main_menu)
		Load();
	console->Clear(true);
	focus = 0;
	item_selected = false;
	console->SetBgColor(libsc::Lcd::kBlue);
	console->SetTextColor(libsc::Lcd::kWhite);
	console->SetCursorRow(0);
	console->WriteString(is_landscape ? "                    " : "                ");
	console->SetCursorRow(0);
	console->SetCursorColumn(((max_string_width + 1 - strlen(menu->menu_name))) / 2);
	console->WriteString(menu->menu_name);
	console->SetBgColor(libsc::Lcd::kBlack);
	console->SetTextColor(libsc::Lcd::kWhite);
	for (uint8_t i = 0; i < max_row && i < menu->menu_items.size(); i++) {
		PrintItem(menu->menu_items[i], i);
	}
	console->SetBgColor(libsc::Lcd::kGray);
	console->SetCursorRow(max_row + 1);
	console->WriteString(is_landscape ? "                    " : "                ");
	console->SetCursorRow(max_row + 1);
	char min[2] = { };
	char sec[2] = { };
	libsc::Timer::TimerInt time_now = libsc::System::Time();
	time_now /= 1000;
	sprintf(min, "%d", time_now / 60);
	if (strlen(min) == 1)
		console->WriteChar('0');
	console->WriteString(min);
	console->WriteChar(':');
	sprintf(sec, "%d", time_now % 60);
	if (strlen(sec) == 1)
		console->WriteChar('0');
	console->WriteString(sec);
	char voltage_string[5] = { };
	float voltage = battery_meter->GetVoltage();
	console->SetTextColor(voltage <= 7.4 ? libsc::Lcd::kRed : libsc::Lcd::kGreen);
	sprintf(voltage_string, "%.2fV", voltage);
	console->SetCursorColumn(max_string_width - 4);
	console->WriteString(voltage_string);
	console->SetBgColor(libsc::Lcd::kBlack);
	console->SetTextColor(libsc::Lcd::kWhite);
	MenuAction(menu);
	Save();
	lcd->Clear();
	console->Clear(true);
}

/*
 * Print the item name and value
 */
void Menu::PrintItem(Item item, uint8_t row) {
	uint8_t focus = this->focus % max_row;
	char buf[5] = { };
	uint8_t space_needed = 0;
	if (row < max_row) {
		console->SetCursorRow(row + 1);
		if (focus == row) {
			if (item_selected) {
				console->SetBgColor(libsc::Lcd::kGreen);
			} else {
				console->SetBgColor(libsc::Lcd::kWhite);
			}
			console->SetTextColor(libsc::Lcd::kBlack);
		}
		console->WriteString(is_landscape ? "                    " : "                ");
		console->SetCursorRow(row + 1);
		console->WriteString(item.name);
		space_needed = max_string_width + 1 - strlen(item.name);
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
		for (uint8_t i = 0; i < space_needed; i++) {
			console->WriteString(" ");
		}
		if (item.type == var_type::boolean) {
			if (*bool_data[item.value_index]) {
				console->WriteString(item.true_text);
			} else {
				console->WriteString(item.false_text);
			}
		} else {
			console->WriteString(buf);
		}
		if (focus == row) {
			console->SetBgColor(libsc::Lcd::kBlack);
			console->SetTextColor(libsc::Lcd::kWhite);
		}
	} else {
		return;
	}
}

void Menu::MenuAction(Items *menu) {
	item_selected = false;
	focus = 0;
	libsc::Timer::TimerInt last_trigger_time = libsc::System::Time();
	libsc::Joystick::State joystick_state;
	libsc::Timer::TimerInt time_now = libsc::System::Time();
	while (1) {
		while (time_now != libsc::System::Time()) {
			time_now = libsc::System::Time();
			joystick_state = joystick->GetState();
			if (time_now % 1000 == 0) {
				console->SetBgColor(libsc::Lcd::kGray);
				console->SetCursorRow(max_row + 1);
				console->WriteString(is_landscape ? "                    " : "                ");
				console->SetCursorRow(max_row + 1);
				char min[2] = { };
				char sec[2] = { };
				time_now /= 1000;
				sprintf(min, "%d", time_now / 60);
				console->SetTextColor(libsc::Lcd::kWhite);
				if (strlen(min) == 1)
					console->WriteChar('0');
				console->WriteString(min);
				console->WriteChar(':');
				sprintf(sec, "%d", time_now % 60);
				if (strlen(sec) == 1)
					console->WriteChar('0');
				console->WriteString(sec);
				char voltage_string[5] = { };
				float voltage = battery_meter->GetVoltage();
				console->SetTextColor(voltage <= 7.4 ? libsc::Lcd::kRed : libsc::Lcd::kGreen);
				sprintf(voltage_string, "%.2fV", voltage);
				console->SetCursorColumn(max_string_width - 4);
				console->WriteString(voltage_string);
				console->SetBgColor(libsc::Lcd::kBlack);
				console->SetTextColor(libsc::Lcd::kWhite);
			}
			switch (joystick_state) {
			case libsc::Joystick::State::kDown:
				item_selected = false;
				focus++;
				if (focus == menu->menu_items.size()) {
					focus = 0;
					console->SetBgColor(libsc::Lcd::kBlack);
					console->SetTextColor(libsc::Lcd::kWhite);
					console->SetCursorRow(1);
					console->WriteString(is_landscape ? "                                                                                                                        " : "                                                                                                                                                ");
					for (uint8_t i = 0; i < max_row && i < menu->menu_items.size(); i++) {
						PrintItem(menu->menu_items[i], i);
					}
					{
						PrintItem(menu->menu_items[focus], focus % max_row);
						console->SetBgColor(libsc::Lcd::kGray);
						console->SetCursorRow(max_row + 1);
						console->WriteString(is_landscape ? "                    " : "                ");
						console->SetCursorRow(max_row + 1);
						char min[2] = { };
						char sec[2] = { };
						time_now /= 1000;
						sprintf(min, "%d", time_now / 60);
						console->SetTextColor(libsc::Lcd::kWhite);
						if (strlen(min) == 1)
							console->WriteChar('0');
						console->WriteString(min);
						console->WriteChar(':');
						sprintf(sec, "%d", time_now % 60);
						if (strlen(sec) == 1)
							console->WriteChar('0');
						console->WriteString(sec);
						char voltage_string[5] = { };
						float voltage = battery_meter->GetVoltage();
						console->SetTextColor(voltage <= 7.4 ? libsc::Lcd::kRed : libsc::Lcd::kGreen);
						sprintf(voltage_string, "%.2fV", voltage);
						console->SetCursorColumn(max_string_width - 4);
						console->WriteString(voltage_string);
						console->SetBgColor(libsc::Lcd::kBlack);
						console->SetTextColor(libsc::Lcd::kWhite);
					}
				} else if (focus != 0 && focus % max_row == 0) {
					console->SetBgColor(libsc::Lcd::kBlack);
					console->SetTextColor(libsc::Lcd::kWhite);
					console->SetCursorRow(1);
					console->WriteString(is_landscape ? "                                                                                                                        " : "                                                                                                                                                ");
					for (uint8_t i = 0; i < max_row && i + focus < menu->menu_items.size(); i++) {
						PrintItem(menu->menu_items[i + focus], i);
					}
					{
						PrintItem(menu->menu_items[focus], focus % max_row);
						console->SetBgColor(libsc::Lcd::kGray);
						console->SetCursorRow(max_row + 1);
						console->WriteString(is_landscape ? "                    " : "                ");
						console->SetCursorRow(max_row + 1);
						char min[2] = { };
						char sec[2] = { };
						time_now /= 1000;
						sprintf(min, "%d", time_now / 60);
						console->SetTextColor(libsc::Lcd::kWhite);
						if (strlen(min) == 1)
							console->WriteChar('0');
						console->WriteString(min);
						console->WriteChar(':');
						sprintf(sec, "%d", time_now % 60);
						if (strlen(sec) == 1)
							console->WriteChar('0');
						console->WriteString(sec);
						char voltage_string[5] = { };
						float voltage = battery_meter->GetVoltage();
						console->SetTextColor(voltage <= 7.4 ? libsc::Lcd::kRed : libsc::Lcd::kGreen);
						sprintf(voltage_string, "%.2fV", voltage);
						console->SetCursorColumn(max_string_width - 4);
						console->WriteString(voltage_string);
						console->SetBgColor(libsc::Lcd::kBlack);
						console->SetTextColor(libsc::Lcd::kWhite);
					}
				} else {
					PrintItem(menu->menu_items[focus - 1], focus % max_row - 1);
					PrintItem(menu->menu_items[focus], focus % max_row);
				}
				break;
			case libsc::Joystick::State::kUp:
				item_selected = false;
				focus--;
				if (focus < 0) {
					focus = menu->menu_items.size() - 1;
					console->SetBgColor(libsc::Lcd::kBlack);
					console->SetTextColor(libsc::Lcd::kWhite);
					console->SetCursorRow(1);
					console->WriteString(is_landscape ? "                                                                                                                        " : "                                                                                                                                                ");
					for (uint8_t i = 0; i < max_row && focus - focus % max_row + i < menu->menu_items.size(); i++) {
						PrintItem(menu->menu_items[focus - focus % max_row + i], i);
					}
					{
						PrintItem(menu->menu_items[focus], focus % max_row);
						console->SetBgColor(libsc::Lcd::kGray);
						console->SetCursorRow(max_row + 1);
						console->WriteString(is_landscape ? "                    " : "                ");
						console->SetCursorRow(max_row + 1);
						char min[2] = { };
						char sec[2] = { };
						time_now /= 1000;
						sprintf(min, "%d", time_now / 60);
						console->SetTextColor(libsc::Lcd::kWhite);
						if (strlen(min) == 1)
							console->WriteChar('0');
						console->WriteString(min);
						console->WriteChar(':');
						sprintf(sec, "%d", time_now % 60);
						if (strlen(sec) == 1)
							console->WriteChar('0');
						console->WriteString(sec);
						char voltage_string[5] = { };
						float voltage = battery_meter->GetVoltage();
						console->SetTextColor(voltage <= 7.4 ? libsc::Lcd::kRed : libsc::Lcd::kGreen);
						sprintf(voltage_string, "%.2fV", voltage);
						console->SetCursorColumn(max_string_width - 4);
						console->WriteString(voltage_string);
						console->SetBgColor(libsc::Lcd::kBlack);
						console->SetTextColor(libsc::Lcd::kWhite);
					}
				} else if (focus % max_row == 8) {
					console->SetBgColor(libsc::Lcd::kBlack);
					console->SetTextColor(libsc::Lcd::kWhite);
					console->SetCursorRow(1);
					console->WriteString(is_landscape ? "                                                                                                                        " : "                                                                                                                                                ");
					for (uint8_t i = 0; i < max_row && focus - focus % max_row + i < menu->menu_items.size(); i++) {
						PrintItem(menu->menu_items[focus - focus % max_row + i], i);
					}
					{
						PrintItem(menu->menu_items[focus], focus % max_row);
						console->SetBgColor(libsc::Lcd::kGray);
						console->SetCursorRow(max_row + 1);
						console->WriteString(is_landscape ? "                    " : "                ");
						console->SetCursorRow(max_row + 1);
						char min[2] = { };
						char sec[2] = { };
						time_now /= 1000;
						sprintf(min, "%d", time_now / 60);
						console->SetTextColor(libsc::Lcd::kWhite);
						if (strlen(min) == 1)
							console->WriteChar('0');
						console->WriteString(min);
						console->WriteChar(':');
						sprintf(sec, "%d", time_now % 60);
						if (strlen(sec) == 1)
							console->WriteChar('0');
						console->WriteString(sec);
						char voltage_string[5] = { };
						float voltage = battery_meter->GetVoltage();
						console->SetTextColor(voltage <= 7.4 ? libsc::Lcd::kRed : libsc::Lcd::kGreen);
						sprintf(voltage_string, "%.2fV", voltage);
						console->SetCursorColumn(max_string_width - 4);
						console->WriteString(voltage_string);
						console->SetBgColor(libsc::Lcd::kBlack);
						console->SetTextColor(libsc::Lcd::kWhite);
					}
				} else {
					PrintItem(menu->menu_items[focus + 1], focus % max_row + 1);
					PrintItem(menu->menu_items[focus], focus % max_row);
				}
				break;
			case libsc::Joystick::State::kLeft:
				if (item_selected) {
					switch (menu->menu_items[focus].type) {
					case var_type::boolean:
						*bool_data[menu->menu_items[focus].value_index] = !*bool_data[menu->menu_items[focus].value_index];
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::flp:
						*float_data[menu->menu_items[focus].value_index] -= menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::int16:
						*int16_data[menu->menu_items[focus].value_index] -= menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::int32:
						*int32_data[menu->menu_items[focus].value_index] -= menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::int8:
						*int8_data[menu->menu_items[focus].value_index] -= menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::menu:
						break;
					case var_type::uint16:
						*uint16_data[menu->menu_items[focus].value_index] -= menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::uint32:
						*uint32_data[menu->menu_items[focus].value_index] -= menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::uint8:
						*uint8_data[menu->menu_items[focus].value_index] -= menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::func:
						break;
					}
				} else {
					return;
				}
				break;
			case libsc::Joystick::State::kRight:
				if (item_selected) {
					switch (menu->menu_items[focus].type) {
					case var_type::boolean:
						*bool_data[menu->menu_items[focus].value_index] = !*bool_data[menu->menu_items[focus].value_index];
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::flp:
						*float_data[menu->menu_items[focus].value_index] += menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::int16:
						*int16_data[menu->menu_items[focus].value_index] += menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::int32:
						*int32_data[menu->menu_items[focus].value_index] += menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::int8:
						*int8_data[menu->menu_items[focus].value_index] += menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::menu:
						break;
					case var_type::uint16:
						*uint16_data[menu->menu_items[focus].value_index] += menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::uint32:
						*uint32_data[menu->menu_items[focus].value_index] += menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::uint8:
						*uint8_data[menu->menu_items[focus].value_index] += menu->menu_items[focus].interval;
						PrintItem(menu->menu_items[focus], focus % max_row);
						break;
					case var_type::func:
						break;
					}
				}
				break;
			case libsc::Joystick::State::kSelect:
				item_selected = !item_selected;
				break;
			case libsc::Joystick::State::kIdle:
				break;
			}
			if (joystick_state == libsc::Joystick::State::kSelect) {
				libsc::System::DelayMs(100);
				if (joystick->GetState() == libsc::Joystick::State::kSelect) {
					libsc::System::DelayMs(250);
					if (joystick->GetState() == libsc::Joystick::State::kSelect) {
						Reset(menu->menu_items[focus]);
						Load();
						item_selected = false;
						focus = 0;
						console->SetBgColor(libsc::Lcd::kBlue);
						console->SetTextColor(libsc::Lcd::kWhite);
						console->SetCursorRow(0);
						console->WriteString(is_landscape ? "                    " : "                ");
						console->SetCursorRow(0);
						console->SetCursorColumn(((max_string_width + 1 - strlen(menu->menu_name))) / 2);
						console->WriteString(menu->menu_name);
						console->SetBgColor(libsc::Lcd::kBlack);
						console->SetTextColor(libsc::Lcd::kWhite);
						for (uint8_t i = 0; i < max_row && i < menu->menu_items.size(); i++) {
							PrintItem(menu->menu_items[i], i);
						}
						{
							PrintItem(menu->menu_items[focus], focus % max_row);
							console->SetBgColor(libsc::Lcd::kGray);
							console->SetCursorRow(max_row + 1);
							console->WriteString(is_landscape ? "                    " : "                ");
							console->SetCursorRow(max_row + 1);
							char min[2] = { };
							char sec[2] = { };
							time_now /= 1000;
							sprintf(min, "%d", time_now / 60);
							console->SetTextColor(libsc::Lcd::kWhite);
							if (strlen(min) == 1)
								console->WriteChar('0');
							console->WriteString(min);
							console->WriteChar(':');
							sprintf(sec, "%d", time_now % 60);
							if (strlen(sec) == 1)
								console->WriteChar('0');
							console->WriteString(sec);
							char voltage_string[5] = { };
							float voltage = battery_meter->GetVoltage();
							console->SetTextColor(voltage <= 7.4 ? libsc::Lcd::kRed : libsc::Lcd::kGreen);
							sprintf(voltage_string, "%.2fV", voltage);
							console->SetCursorColumn(max_string_width - 4);
							console->WriteString(voltage_string);
							console->SetBgColor(libsc::Lcd::kBlack);
							console->SetTextColor(libsc::Lcd::kWhite);
						}
					}
				}
				if (item_selected && menu->menu_items[focus].type == var_type::menu) {
					if (menu->menu_items[focus].sub_menu != nullptr) {
						Save();
						Load();
						EnterMenu(menu->menu_items[focus].sub_menu);
						item_selected = false;
						focus = 0;
						console->SetBgColor(libsc::Lcd::kBlue);
						console->SetTextColor(libsc::Lcd::kWhite);
						console->SetCursorRow(0);
						console->WriteString(is_landscape ? "                    " : "                ");
						console->SetCursorRow(0);
						console->SetCursorColumn(((max_string_width + 1 - strlen(menu->menu_name))) / 2);
						console->WriteString(menu->menu_name);
						console->SetBgColor(libsc::Lcd::kBlack);
						console->SetTextColor(libsc::Lcd::kWhite);
						for (uint8_t i = 0; i < max_row && i < menu->menu_items.size(); i++) {
							PrintItem(menu->menu_items[i], i);
						}
						{
							PrintItem(menu->menu_items[focus], focus % max_row);
							console->SetBgColor(libsc::Lcd::kGray);
							console->SetCursorRow(max_row + 1);
							console->WriteString(is_landscape ? "                    " : "                ");
							console->SetCursorRow(max_row + 1);
							char min[2] = { };
							char sec[2] = { };
							time_now /= 1000;
							sprintf(min, "%d", time_now / 60);
							console->SetTextColor(libsc::Lcd::kWhite);
							if (strlen(min) == 1)
								console->WriteChar('0');
							console->WriteString(min);
							console->WriteChar(':');
							sprintf(sec, "%d", time_now % 60);
							if (strlen(sec) == 1)
								console->WriteChar('0');
							console->WriteString(sec);
							char voltage_string[5] = { };
							float voltage = battery_meter->GetVoltage();
							console->SetTextColor(voltage <= 7.4 ? libsc::Lcd::kRed : libsc::Lcd::kGreen);
							sprintf(voltage_string, "%.2fV", voltage);
							console->SetCursorColumn(max_string_width - 4);
							console->WriteString(voltage_string);
							console->SetBgColor(libsc::Lcd::kBlack);
							console->SetTextColor(libsc::Lcd::kWhite);
						}
					}
				} else if (item_selected && menu->menu_items[focus].type == var_type::func) {
					Save();
					Load();
					std::function < void() > f = func_vector[menu->menu_items[focus].value_index];
					f();
					libsc::System::DelayMs(500);
					item_selected = false;
					focus = 0;
					console->SetBgColor(libsc::Lcd::kBlue);
					console->SetTextColor(libsc::Lcd::kWhite);
					console->SetCursorRow(0);
					console->WriteString(is_landscape ? "                    " : "                ");
					console->SetCursorRow(0);
					console->SetCursorColumn(((max_string_width + 1 - strlen(menu->menu_name))) / 2);
					console->WriteString(menu->menu_name);
					console->SetBgColor(libsc::Lcd::kBlack);
					console->SetTextColor(libsc::Lcd::kWhite);
					for (uint8_t i = 0; i < max_row && i < menu->menu_items.size(); i++) {
						PrintItem(menu->menu_items[i], i);
					}
					{
						PrintItem(menu->menu_items[focus], focus % max_row);
						console->SetBgColor(libsc::Lcd::kGray);
						console->SetCursorRow(max_row + 1);
						console->WriteString(is_landscape ? "                    " : "                ");
						console->SetCursorRow(max_row + 1);
						char min[2] = { };
						char sec[2] = { };
						time_now /= 1000;
						sprintf(min, "%d", time_now / 60);
						console->SetTextColor(libsc::Lcd::kWhite);
						if (strlen(min) == 1)
							console->WriteChar('0');
						console->WriteString(min);
						console->WriteChar(':');
						sprintf(sec, "%d", time_now % 60);
						if (strlen(sec) == 1)
							console->WriteChar('0');
						console->WriteString(sec);
						char voltage_string[5] = { };
						float voltage = battery_meter->GetVoltage();
						console->SetTextColor(voltage <= 7.4 ? libsc::Lcd::kRed : libsc::Lcd::kGreen);
						sprintf(voltage_string, "%.2fV", voltage);
						console->SetCursorColumn(max_string_width - 4);
						console->WriteString(voltage_string);
						console->SetBgColor(libsc::Lcd::kBlack);
						console->SetTextColor(libsc::Lcd::kWhite);
					}
				}
				PrintItem(menu->menu_items[focus], focus % max_row);
				time_now = libsc::System::Time();
			} else if (joystick_state != libsc::Joystick::State::kIdle) {
				last_trigger_time = libsc::System::Time() - last_trigger_time;
				if (last_trigger_time < 60) {
					libsc::System::DelayMs(45);
				} else if (last_trigger_time < 110) {
					libsc::System::DelayMs(50);
				} else if (last_trigger_time < 160) {
					libsc::System::DelayMs(100);
				} else if (last_trigger_time < 200) {
					libsc::System::DelayMs(150);
				} else {
					libsc::System::DelayMs(200);
				}
				last_trigger_time = libsc::System::Time();
			}	
		}
	}
}

void Menu::Load() {
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

void Menu::Save() {
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

void Menu::Reset() {
	if (flash == nullptr)
		return;
	bool reset = false;
	console->Clear(true);
	console->SetBgColor(libsc::Lcd::kRed);
	console->SetTextColor(libsc::Lcd::kBlack);
	console->SetCursorRow(0);
	console->WriteString(is_landscape ? "                    " : "                ");
	console->SetCursorRow(0);
	console->WriteString(is_landscape ? "       Reset?       " : "     Reset?     ");
	console->SetBgColor(libsc::Lcd::kBlack);
	console->SetTextColor(libsc::Lcd::kWhite);
	console->SetCursorRow(1);
	console->WriteString(is_landscape ? "         Yes        " : "       Yes      ");
	console->SetBgColor(libsc::Lcd::kWhite);
	console->SetTextColor(libsc::Lcd::kBlack);
	console->SetCursorRow(2);
	console->WriteString(is_landscape ? "         No         " : "       No       ");
	libsc::System::DelayMs(800);
	while (1) {
		switch (joystick->GetState()) {
		case libsc::Joystick::State::kUp:
			reset = !reset;
			if (reset) {
				console->SetBgColor(libsc::Lcd::kWhite);
				console->SetTextColor(libsc::Lcd::kBlack);
				console->SetCursorRow(1);
				console->WriteString(is_landscape ? "         Yes        " : "       Yes      ");
				console->SetBgColor(libsc::Lcd::kBlack);
				console->SetTextColor(libsc::Lcd::kWhite);
				console->SetCursorRow(2);
				console->WriteString(is_landscape ? "         No         " : "       No       ");
			} else {
				console->SetBgColor(libsc::Lcd::kBlack);
				console->SetTextColor(libsc::Lcd::kWhite);
				console->SetCursorRow(1);
				console->WriteString(is_landscape ? "         Yes        " : "       Yes      ");
				console->SetBgColor(libsc::Lcd::kWhite);
				console->SetTextColor(libsc::Lcd::kBlack);
				console->SetCursorRow(2);
				console->WriteString(is_landscape ? "         No         " : "       No       ");
			}
			libsc::System::DelayMs(300);
			break;
		case libsc::Joystick::State::kDown:
			reset = !reset;
			if (reset) {
				console->SetBgColor(libsc::Lcd::kWhite);
				console->SetTextColor(libsc::Lcd::kBlack);
				console->SetCursorRow(1);
				console->WriteString(is_landscape ? "         Yes        " : "       Yes      ");
				console->SetBgColor(libsc::Lcd::kBlack);
				console->SetTextColor(libsc::Lcd::kWhite);
				console->SetCursorRow(2);
				console->WriteString(is_landscape ? "         No         " : "       No       ");
			} else {
				console->SetBgColor(libsc::Lcd::kBlack);
				console->SetTextColor(libsc::Lcd::kWhite);
				console->SetCursorRow(1);
				console->WriteString(is_landscape ? "         Yes        " : "       Yes      ");
				console->SetBgColor(libsc::Lcd::kWhite);
				console->SetTextColor(libsc::Lcd::kBlack);
				console->SetCursorRow(2);
				console->WriteString(is_landscape ? "         No         " : "       No       ");
			}
			libsc::System::DelayMs(300);
			break;
		case libsc::Joystick::State::kSelect:
			if (reset) {
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
				libsc::System::DelayMs(300);
				return;
			} else {
				libsc::System::DelayMs(300);
				return;
			}
			break;
		default:
			break;
		}
	}
}

void Menu::Reset(Item item) {
	if (flash == nullptr || item.type == var_type::func || item.type == var_type::menu)
		return;
	bool reset = false;
	console->Clear(true);
	console->SetBgColor(libsc::Lcd::kRed);
	console->SetTextColor(libsc::Lcd::kBlack);
	console->SetCursorRow(0);
	console->WriteString(is_landscape ? "                    " : "                ");
	console->SetCursorRow(0);
	console->WriteString(is_landscape ? "                    " : "                ");
	console->SetCursorRow(0);
	console->SetCursorColumn((max_string_width - 5 - strlen(item.name)) / 2);
	console->WriteString("Reset ");
	console->WriteString(item.name);
	console->SetBgColor(libsc::Lcd::kBlack);
	console->SetTextColor(libsc::Lcd::kWhite);
	console->SetCursorRow(1);
	console->WriteString(is_landscape ? "         Yes        " : "       Yes      ");
	console->SetBgColor(libsc::Lcd::kWhite);
	console->SetTextColor(libsc::Lcd::kBlack);
	console->SetCursorRow(2);
	console->WriteString(is_landscape ? "         No         " : "       No       ");
	libsc::System::DelayMs(800);
	while (1) {
		switch (joystick->GetState()) {
		case libsc::Joystick::State::kUp:
			reset = !reset;
			if (reset) {
				console->SetBgColor(libsc::Lcd::kWhite);
				console->SetTextColor(libsc::Lcd::kBlack);
				console->SetCursorRow(1);
				console->WriteString(is_landscape ? "         Yes        " : "       Yes      ");
				console->SetBgColor(libsc::Lcd::kBlack);
				console->SetTextColor(libsc::Lcd::kWhite);
				console->SetCursorRow(2);
				console->WriteString(is_landscape ? "         No         " : "       No       ");
			} else {
				console->SetBgColor(libsc::Lcd::kBlack);
				console->SetTextColor(libsc::Lcd::kWhite);
				console->SetCursorRow(1);
				console->WriteString(is_landscape ? "         Yes        " : "       Yes      ");
				console->SetBgColor(libsc::Lcd::kWhite);
				console->SetTextColor(libsc::Lcd::kBlack);
				console->SetCursorRow(2);
				console->WriteString(is_landscape ? "         No         " : "       No       ");
			}
			libsc::System::DelayMs(300);
			break;
		case libsc::Joystick::State::kDown:
			reset = !reset;
			if (reset) {
				console->SetBgColor(libsc::Lcd::kWhite);
				console->SetTextColor(libsc::Lcd::kBlack);
				console->SetCursorRow(1);
				console->WriteString(is_landscape ? "         Yes        " : "       Yes      ");
				console->SetBgColor(libsc::Lcd::kBlack);
				console->SetTextColor(libsc::Lcd::kWhite);
				console->SetCursorRow(2);
				console->WriteString(is_landscape ? "         No         " : "       No       ");
			} else {
				console->SetBgColor(libsc::Lcd::kBlack);
				console->SetTextColor(libsc::Lcd::kWhite);
				console->SetCursorRow(1);
				console->WriteString(is_landscape ? "         Yes        " : "       Yes      ");
				console->SetBgColor(libsc::Lcd::kWhite);
				console->SetTextColor(libsc::Lcd::kBlack);
				console->SetCursorRow(2);
				console->WriteString(is_landscape ? "         No         " : "       No       ");
			}
			libsc::System::DelayMs(300);
			break;
		case libsc::Joystick::State::kSelect:
			if (reset) {
				switch (item.type) {
				case var_type::boolean:
					*bool_data[item.value_index] = bool_backup[item.value_index];
					break;
				case var_type::flp:
					*float_data[item.value_index] = float_backup[item.value_index];
					break;
				case var_type::int16:
					*int16_data[item.value_index] = int16_backup[item.value_index];
					break;
				case var_type::int32:
					*int32_data[item.value_index] = int32_backup[item.value_index];
					break;
				case var_type::int8:
					*int8_data[item.value_index] = int8_backup[item.value_index];
					break;
				case var_type::uint16:
					*uint16_data[item.value_index] = uint16_backup[item.value_index];
					break;
				case var_type::uint32:
					*uint32_data[item.value_index] = uint32_backup[item.value_index];
					break;
				case var_type::uint8:
					*uint8_data[item.value_index] = uint8_backup[item.value_index];
					break;
				default:
					return;
				}

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
				libsc::System::DelayMs(300);

				return;
			} else {
				libsc::System::DelayMs(300);
				return;
			}
			break;
		default:
			break;
		}
	}
}

}

