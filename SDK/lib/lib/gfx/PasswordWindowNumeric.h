/*
 * PasswordWindowNumeric.h
 *
 *  Created on: Sep 18, 2016
 *      Author: John Smith
 */


#ifndef PASSWORDWINDOWNUMERIC_H_
#define PASSWORDWINDOWNUMERIC_H_

#include "lib/gfx/window_def.h"

typedef struct password_window_numeric_s
{
	tButton *TextBox;
	tButton *Btn1;
	tButton *Btn2;
	tButton *Btn3;
	tButton *Btn4;
	tButton *Btn5;
	tButton *Btn6;
	tButton *Btn7;
	tButton *Btn8;
	tButton *Btn9;
	tButton *BtnBack;
	tButton *Btn0;
	tButton *BtnOk;
	tWindow *ParentWindow;
	tWindow *Window;
	int min_chars;
	int max_chars;
	String_t* Text;
	bool after_wrong_password;
	bool text_refrashed;
}password_window_numeric_t;

#define new_password_window_numeric(window_addr, window_name, btn_space_x, btn_space_y)\
	password_window_numeric_t *window_name = (password_window_numeric_t *)calloc(1, sizeof(password_window_numeric_t));\
	window_new_window(window_addr, window_name##_window);\
	window_name##_window->Position.X = 0;\
	window_name##_window->Position.Y = 0;\
	window_name##_window->Size.X = window_addr->Size.X - 5;\
	window_name##_window->Size.Y = window_name##_window->Size.Y - 5 - window_name##_window->Internals.Header.Size.Y;\
	window_name##_window->Internals.FullScreen = true;\
	window_name##_window->AllowHScroll = false;\
	window_name##_window->AllowVScroll = false;\
	window_name##_window->HideHScroll = true;\
	window_name##_window->HideVScroll = true;\
	window_name##_window->MaxMinEnabled = false;\
	window_name##_window->MinimizeButonEnabled = false;\
	window_name##_window->CloseButonEnabled = false;\
	window_name##_window->Caption.TextAlign = Align_Center;\
	String.Set(window_name##_window->Caption.Text, "Numeric password");\
	\
	int pass_window_numeric_keys_upleft_X = 5;\
	int pass_window_numeric_keys_upleft_Y = 5;\
	int pass_window_numeric_btn_high = (window_name##_window->Size.Y - window_name##_window->Internals.Header.Size.Y - pass_window_numeric_keys_upleft_Y - (btn_space_y * 6)) / 5;\
	window_new_button(window_name##_window, TextBoxPass);\
	TextBoxPass->Position.X = pass_window_numeric_keys_upleft_X;\
	TextBoxPass->Position.Y = pass_window_numeric_keys_upleft_Y;\
	TextBoxPass->Size.X = window_name##_window->Size.X - (pass_window_numeric_keys_upleft_X * 2) - btn_space_x;\
	TextBoxPass->Size.Y = pass_window_numeric_btn_high;\
	TextBoxPass->Enabled = false;\
	String.Set(TextBoxPass->Caption.Text, "");\
	\
	pass_window_numeric_keys_upleft_Y += TextBoxPass->Size.Y + btn_space_y;\
	int pass_window_numeric_btn_width = (window_name##_window->Size.X - pass_window_numeric_keys_upleft_X - (btn_space_x * 4)) / 3;\
	\
	window_new_button(window_name##_window, WinPassNumBtn1);\
	WinPassNumBtn1->Position.X = pass_window_numeric_keys_upleft_X;\
	WinPassNumBtn1->Position.Y = pass_window_numeric_keys_upleft_Y;\
	WinPassNumBtn1->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtn1->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtn1->Caption.Text, "1");\
	window_new_button(window_name##_window, WinPassNumBtn2);\
	WinPassNumBtn2->Position.X = pass_window_numeric_keys_upleft_X + btn_space_x + pass_window_numeric_btn_width;\
	WinPassNumBtn2->Position.Y = pass_window_numeric_keys_upleft_Y;\
	WinPassNumBtn2->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtn2->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtn2->Caption.Text, "2");\
	window_new_button(window_name##_window, WinPassNumBtn3);\
	WinPassNumBtn3->Position.X = pass_window_numeric_keys_upleft_X + ((btn_space_x + pass_window_numeric_btn_width) * 2);\
	WinPassNumBtn3->Position.Y = pass_window_numeric_keys_upleft_Y;\
	WinPassNumBtn3->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtn3->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtn3->Caption.Text, "3");\
	\
	window_new_button(window_name##_window, WinPassNumBtn4);\
	WinPassNumBtn4->Position.X = pass_window_numeric_keys_upleft_X;\
	WinPassNumBtn4->Position.Y = pass_window_numeric_keys_upleft_Y + btn_space_y + pass_window_numeric_btn_high;\
	WinPassNumBtn4->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtn4->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtn4->Caption.Text, "4");\
	window_new_button(window_name##_window, WinPassNumBtn5);\
	WinPassNumBtn5->Position.X = pass_window_numeric_keys_upleft_X + btn_space_x + pass_window_numeric_btn_width;\
	WinPassNumBtn5->Position.Y = pass_window_numeric_keys_upleft_Y + btn_space_y + pass_window_numeric_btn_high;\
	WinPassNumBtn5->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtn5->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtn5->Caption.Text, "5");\
	window_new_button(window_name##_window, WinPassNumBtn6);\
	WinPassNumBtn6->Position.X = pass_window_numeric_keys_upleft_X + ((btn_space_x + pass_window_numeric_btn_width) * 2);\
	WinPassNumBtn6->Position.Y = pass_window_numeric_keys_upleft_Y + btn_space_y + pass_window_numeric_btn_high;\
	WinPassNumBtn6->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtn6->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtn6->Caption.Text, "6");\
\
	window_new_button(window_name##_window, WinPassNumBtn7);\
	WinPassNumBtn7->Position.X = pass_window_numeric_keys_upleft_X;\
	WinPassNumBtn7->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 2);\
	WinPassNumBtn7->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtn7->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtn7->Caption.Text, "7");\
	window_new_button(window_name##_window, WinPassNumBtn8);\
	WinPassNumBtn8->Position.X = pass_window_numeric_keys_upleft_X + btn_space_x + pass_window_numeric_btn_width;\
	WinPassNumBtn8->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 2);\
	WinPassNumBtn8->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtn8->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtn8->Caption.Text, "8");\
	window_new_button(window_name##_window, WinPassNumBtn9);\
	WinPassNumBtn9->Position.X = pass_window_numeric_keys_upleft_X + ((btn_space_x + pass_window_numeric_btn_width) * 2);\
	WinPassNumBtn9->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 2);\
	WinPassNumBtn9->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtn9->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtn9->Caption.Text, "9");\
\
	window_new_button(window_name##_window, WinPassNumBtnBack);\
	WinPassNumBtnBack->Position.X = pass_window_numeric_keys_upleft_X;\
	WinPassNumBtnBack->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 3);\
	WinPassNumBtnBack->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtnBack->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtnBack->Caption.Text, "Back");\
	window_new_button(window_name##_window, WinPassNumBtn0);\
	WinPassNumBtn0->Position.X = pass_window_numeric_keys_upleft_X + btn_space_x + pass_window_numeric_btn_width;\
	WinPassNumBtn0->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 3);\
	WinPassNumBtn0->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtn0->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtn0->Caption.Text, "0");\
	window_new_button(window_name##_window, WinPassNumBtnOk);\
	WinPassNumBtnOk->Position.X = pass_window_numeric_keys_upleft_X + ((btn_space_x + pass_window_numeric_btn_width) * 2);\
	WinPassNumBtnOk->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 3);\
	WinPassNumBtnOk->Size.X = pass_window_numeric_btn_width;\
	WinPassNumBtnOk->Size.Y = pass_window_numeric_btn_high;\
	String.Set(WinPassNumBtnOk->Caption.Text, "Ok");\
\
	window_name->TextBox = TextBoxPass;\
	window_name->Btn1 = WinPassNumBtn1;\
	window_name->Btn2 = WinPassNumBtn2;\
	window_name->Btn3 = WinPassNumBtn3;\
	window_name->Btn4 = WinPassNumBtn4;\
	window_name->Btn5 = WinPassNumBtn5;\
	window_name->Btn6 = WinPassNumBtn6;\
	window_name->Btn7 = WinPassNumBtn7;\
	window_name->Btn8 = WinPassNumBtn8;\
	window_name->Btn9 = WinPassNumBtn9;\
	window_name->BtnBack = WinPassNumBtnBack;\
	window_name->Btn0 = WinPassNumBtn0;\
	window_name->BtnOk = WinPassNumBtnOk;\
	window_name->ParentWindow = window_addr;\
	window_name->Window = window_name##_window;\
	window_name->min_chars = 4;\
	window_name->max_chars = 16;\
	window_name->Text = String.Set(window_name->Text, "");

bool password_window_numeric_idle(password_window_numeric_t *PassWindow);

#endif /* PASSWORDWINDOWNUMERIC_H_ */
