/*
 * PasswordWindowNumeric.c
 *
 *  Created on: Sep 18, 2016
 *      Author: John Smith
 */

#include <stdlib.h>
#include "PasswordWindowNumeric.h"
#include "lib/gfx/window_def.h"
#include "lib/gfx/button_def.h"

/******************************************************************************/
/*
 * Example of use:
 *
 * Construct the window
 *
 * MainWindow = parent window.
 * PasswordWindowNumeric = name of structure to maintain all ressources for this window.
 * 3 = Vertical space between numeric buttons.
 * 3 = Horizontal space between numeric buttons.
 *     new_password_window_numeric(MainWindow, PasswordWindowNumeric, 3, 3);
 *
 */
/******************************************************************************/
bool password_window_numeric_idle(password_window_numeric_t *PassWindow)
{
	if(PassWindow->Window->Visible == false)
		return false;
	if(PassWindow->after_wrong_password == true && PassWindow->Text->modifyed)
		String.Set(PassWindow->TextBox->Caption.Text, PassWindow->Text->text);
	char character = 0;
	if(PassWindow->Btn1->Events.CursorUp)
	{
		PassWindow->Btn1->Events.CursorUp = false;
		character = '1';
	}
	else if(PassWindow->Btn2->Events.CursorUp)
	{
		PassWindow->Btn2->Events.CursorUp = false;
		character = '2';
	}
	else if(PassWindow->Btn3->Events.CursorUp)
	{
		PassWindow->Btn3->Events.CursorUp = false;
		character = '3';
	}
	else if(PassWindow->Btn4->Events.CursorUp)
	{
		PassWindow->Btn4->Events.CursorUp = false;
		character = '4';
	}
	else if(PassWindow->Btn5->Events.CursorUp)
	{
		PassWindow->Btn5->Events.CursorUp = false;
		character = '5';
	}
	else if(PassWindow->Btn6->Events.CursorUp)
	{
		PassWindow->Btn6->Events.CursorUp = false;
		character = '6';
	}
	else if(PassWindow->Btn7->Events.CursorUp)
	{
		PassWindow->Btn7->Events.CursorUp = false;
		character = '7';
	}
	else if(PassWindow->Btn8->Events.CursorUp)
	{
		PassWindow->Btn8->Events.CursorUp = false;
		character = '8';
	}
	else if(PassWindow->Btn9->Events.CursorUp)
	{
		PassWindow->Btn9->Events.CursorUp = false;
		character = '9';
	}
	else if(PassWindow->BtnBack->Events.CursorUp || PassWindow->BtnBack->Events.CursorMove)
	{
		PassWindow->BtnBack->Events.CursorUp = false;
		PassWindow->BtnBack->Events.CursorMove = false;
		if(PassWindow->TextBox->Caption.Text->len > 0)
		{
			char *tmp = String.Substring(PassWindow->Text, 0, PassWindow->Text->len - 1);
			String.Set(PassWindow->Text, tmp);
			if(tmp)
				free(tmp);
			if(PassWindow->after_wrong_password == false)
			{
				int cnt = 0;
				String.Clear(PassWindow->TextBox->Caption.Text);
				for(; cnt < PassWindow->Text->len; cnt++)
				{
					String.AppendChar(PassWindow->TextBox->Caption.Text, '*');
				}
			}
			else
			{
				String.Set(PassWindow->TextBox->Caption.Text, PassWindow->Text->text);
			}
			return false;
		}
	}
	else if(PassWindow->Btn0->Events.CursorUp)
	{
		PassWindow->Btn0->Events.CursorUp = false;
		character = '0';
	}
	else if(PassWindow->BtnOk->Events.CursorUp)
	{
		PassWindow->BtnOk->Events.CursorUp = false;
		return true;
	}
	if(character != 0)
	{
		PassWindow->after_wrong_password = false;
		String.AppendChar(PassWindow->Text, character);
		int cnt = 0;
		String.Clear(PassWindow->TextBox->Caption.Text);
		for(; cnt < PassWindow->Text->len; cnt++)
		{
			String.AppendChar(PassWindow->TextBox->Caption.Text, '*');
		}
	}
	return false;
}
