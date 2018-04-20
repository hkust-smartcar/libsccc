/*
 * touch_keyboard.cpp
 *
 *  Created on: Feb 11, 2018
 *      Author: dipsy
 */

#include "libutil/touch_keyboard.h"

namespace libutil{

string TouchKeyboard::ShowKeyboard(){
	uint8_t sum=0;
	time_t last_tap = 0;
	char last_key = '+';
	RenderKeyboard();
	PrintCurrentStr();
	while(1){
		if(pLcd->Scan(0) && pLcd->touch_status!=4){
			sum++;
			char key = GetKey(pLcd->touch_x[0],pLcd->touch_y[0]);
			if(key == last_key){
				if(System::Time()-last_tap<=300)continue;
			}
			last_key = key;
			last_tap = System::Time();
			switch(key){
			default:
				if(len<34){
					str[len++] = (cap_lock ? ToUpperCase(key) : key);
					str[len]='\0';
				}
				break;
			case '\b':
				if(len>0){
					str[--len] = '\0';
				}
				break;
			case '\t':
				RenderKeyboard(cap_lock = !cap_lock);
				break;
			case '\n':
				return string(str);
			case 0:
				break;
			}
			PrintCurrentStr();
		}

		else if(System::Time()%250 == 0){
			PrintCurrentStr();
		}
//		pLcd->ShowNum(0,100,len,4,48);
//		pLcd->ShowNum(0,150,last_tap,4,48);
//		pLcd->ShowNum(0,200,System::Time(),4,48);
//		pLcd->ShowNum(0,250,sum,4,48);
	}
}

string TouchKeyboard::ShowKeyboard(const string& s){
	memcpy(str,&*s.begin(),len = s.size());
	PrintCurrentStr();
	return ShowKeyboard();
}

void TouchKeyboard::RenderKeyboard(bool is_upper_case){
	for(int i=0; i<5; i++){
		const string& s = keys[i];
		for(int j=s.size()-1; j>=0;j--){
			char c = s[j];
			uint16_t x = pl[i]+ox+j*kw, y = oy+i*kh;
			pLcd->Fill(x, y, x+kw, y+kh, 0xFFFF);
			pLcd->POINT_COLOR = 0x0000;
			pLcd->BACK_COLOR = 0xFFFF;
			if(c!='\\'){
				pLcd->DrawLine(x,y,x,y+kh);
			}
			pLcd->DrawLine(x,y,x+kw,y);
			pLcd->DrawLine(x,y+kh,x+kw,y+kh);
			switch(c){
			default:
				if(is_upper_case)
					c = ToUpperCase(c);
				pLcd->ShowChar(x+5, y+kh-48,c,48,0);
				break;
			case '\b':
				pLcd->ShowString(x+5, y+kh-48, 400, 48, 48, "<-", 0);
				break;
			case '\t':
				pLcd->ShowString(x+5, y+kh-48, 400, 48, 48, "^", 0);
				break;
			case '\n':
				pLcd->ShowString(x+5, y+kh-48, 400, 48, 48, "Done", 0);
				break;
			case '\\':
				break;
			}

		}
	}
}

void TouchKeyboard::PrintCurrentStr(){
	char buff[36];
	strcpy(buff,str);
	pLcd->Fill(0,0,480,127,0xFFFF);
	if(System::Time()/500%2){
		strcat(buff,"_");
	}
	pLcd->ShowString(20, 20, 408, 48, 48, buff, 0);
	if(len>17)
		pLcd->ShowString(20, 70, 408, 48, 48, buff+17, 0);
}

char TouchKeyboard::GetKey(uint16_t x, uint16_t y){
	uint8_t i = (y-oy)/kh;
	uint8_t j = (x-ox-pl[i])/kw;
	if(i<0||j<0||i>=5||j>=keys[i].size())return 0;
	else return (keys[i][j] == '\\' ? GetKey(x-48,y) : keys[i][j]);
}

char TouchKeyboard::ToUpperCase(const char& c){
	if(c>='a'&&c<='z')return c-'a'+'A';
	else return c;
}

}


