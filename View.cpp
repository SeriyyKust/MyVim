#include "View.h"

Area::Area(IAdapterPDCur* area_screen, int y, int x, int height, int width, int color_pair, int color_text, int color_background, TypeWindow type) :
	area_screen_(area_screen) , start_y_(y), start_x_(x), height_(height), width_(width), color_pair_(color_pair)
{
	if (type == TypeWindow::cPad)
	{
		id_screen_ = area_screen_->CratePad(start_y_, start_x_, height_, width_);
	}
	else
	{
		id_screen_ = area_screen_->CrateWin(start_y_, start_x_, height_, width_);
	}
	area_screen->CreateColorPair(color_pair_, color_text, color_background);

	length_numbering_ = 3;
	scrollcross_ = 0;

	y_cursor_ = 0;
	x_cursor_ = length_numbering_;
}
/*
void AreaNumbering::AreaBeginApp()
{
	area_screen_->Print(id_screen_, start_y_, start_x_, " 1", color_pair_);
	used_line_[0] = true;
	for (int i = 1; i < height_; i++)
		area_screen_->Print(id_screen_, start_y_ + i, start_x_, "~", color_pair_);
	area_screen_->RefreshPad(id_screen_, 0 + scrollcross_, 0, start_y_ , start_x_, area_screen_->GetMaxHeightMainWin() - 3, width_);
}

void AreaText::AreaBeginApp()
{
	if (y_cursor_ < height_ + 3 && x_cursor_ < width_ + length_numbering_ && x_cursor_ >= 0 && y_cursor_ >= 0)
	{
		//area_screen_->ViewCursor(y_cursor_, x_cursor_, 3, id_screen_);
	}
	area_screen_->RefreshPad(id_screen_, 0 + scrollcross_, 0, start_y_, start_x_, area_screen_->GetMaxHeightMainWin() - 3, width_);
}

void AreaCommands::AreaBeginApp()
{
	area_screen_->Print(id_screen_, start_y_, start_x_, " ", color_pair_);
	area_screen_->RefreshWin(id_screen_);
}

void AreaState::AreaBeginApp()
{
	area_screen_->Print(id_screen_, start_y_, start_x_, "State: none", color_pair_);
	area_screen_->RefreshWin(id_screen_);
	area_screen_->SetCursor(y_cursor_, x_cursor_);
	area_screen_->Refresh();
}
*/
void Area::GetCursor(int& y, int& x)
{
	y = y_cursor_;
	x = x_cursor_ - length_numbering_;
}



BasicString Translate(int number)
{
	BasicString return_string;
	int d = 1;
	while (number / d != 0)
		d *= 10;
	d /= 10;
	while (d != 0)
	{
		int tmp = number / d;
		return_string.PushBack((char)(tmp + 48));
		number %= d;
		d /= 10;
	}
	return return_string;
}


/////// View 

View::View(IAdapterPDCur* screen) : screen_(screen)
{
	//height_ = screen_->GetMaxHeightMainWin();

	height_ = cMaxAmountLine;
	width_ = screen_->GetMaxWidthMainWin();
	length_numbering_ = 3;
	numbering_ = new AreaNumbering(screen_, 0, 0, height_ - 3, length_numbering_);
	text_ = new AreaText(screen_, 0, length_numbering_, height_ - 3, width_ );
	command_line_ = new AreaCommands(screen_, screen_->GetMaxHeightMainWin() - 3, 0, 1, width_);
	state_ = new AreaState(screen_, screen_->GetMaxHeightMainWin() - 2, 0, 2, width_);
	type_ = TypeUpdate::cNorm;




	int column_width_shift = 9;
	int column_width_hex_symbol = 50;
	int column_width_base_symbol = 16;
	int column_width_orig_symbol = 15;
	int hieght_shift_win = 2;
	
	hex_hat_table_ = new HexHatTable(screen_, 0, 0, column_width_shift, 0, column_width_shift, column_width_hex_symbol, 0, column_width_hex_symbol + column_width_shift,
		column_width_base_symbol, hieght_shift_win);
	hex_shift_lines_ = new HexShiftLines(screen_, hieght_shift_win, 0, height_, column_width_shift, column_width_base_symbol);
	hex_base_line_ = new HexBaseLines(screen_, hieght_shift_win, column_width_shift, height_, column_width_hex_symbol, column_width_base_symbol);
	hex_string_lines_ = new HexStringLines(screen_, hieght_shift_win, column_width_shift + column_width_hex_symbol, height_, column_width_base_symbol);
	hex_commands_ = new HexCommands(screen_, screen_->GetMaxHeightMainWin() - 3, 0, 1, column_width_shift + column_width_hex_symbol + column_width_base_symbol + 2);
	hex_state_ = new HexState(screen_, screen_->GetMaxHeightMainWin() - 2, 0, 2, column_width_shift + column_width_hex_symbol + column_width_base_symbol + 2, hieght_shift_win, column_width_shift + column_width_hex_symbol);

	AddObserver(text_);
	AddObserver(numbering_);
	AddObserver(command_line_);
	AddObserver(state_);
	AddObserver(hex_hat_table_);
	AddObserver(hex_shift_lines_);
	AddObserver(hex_base_line_);
	AddObserver(hex_string_lines_);
	AddObserver(hex_commands_);
	AddObserver(hex_state_);
}


void View::UpdateLine(BasicString string, int line, int hex_cursor)
{
	if (type_ == TypeUpdate::cNorm)
		ChangeLine(string, line, hex_cursor,0,4);
	else
		ChangeLine(string, line, hex_cursor, 4, 10);
}

void View::UpdateCursor(int y, int x, BasicString name, BasicString type)
{
	if (type_ == TypeUpdate::cNorm)
		ChangeCursor(y, x, name, type, 0, 4);
	else
		ChangeCursor(y, x, name, type, 4, 10);
}

void View::UpdateCross(bool type)
{
	if (type_ == TypeUpdate::cNorm)
		ChangeCross(type,0,4);
	else
		ChangeCross(type, 4,10);
}

void View::UpdateType(TypeUpdate type)
{
	screen_->ClearAll();
	type_ = type;
}

View::~View() {
	delete numbering_;
	delete text_;
	delete command_line_;
	delete state_;
}


//Normal
/// /////////////////////////////////////////////////////////


void AreaNumbering::UpdateLine(BasicString string, int line, int hex_cursor)
{
	if (line > -1)
	{
		if (string.Size() == 0)
		{
			for (int i = line; i < area_screen_->GetMaxHeightMainWin() - 3; i++)
			{
				area_screen_->ClearLine(id_screen_, i, 0, area_screen_->GetMaxWidthMainWin(), color_pair_);
				area_screen_->Print(id_screen_, i, start_x_, "~ ", color_pair_);
				area_screen_->RefreshPad(id_screen_, scrollcross_, 0, start_y_, start_x_, area_screen_->GetMaxHeightMainWin() - 4, width_);
			}
		}
		else
		{
			if (line > 8)
			{

				area_screen_->Print(id_screen_, line, start_x_, line + 1, color_pair_);
				//area_screen_->Print(line, start_x_ + 2, ' ', color_pair_);
			}
			else
			{
				area_screen_->Print(id_screen_, line, start_x_, ' ', color_pair_);
				area_screen_->Print(id_screen_, line, start_x_ + 1, line + 1, color_pair_);
				//area_screen_->Print(line, start_x_ + 2, ' ', color_pair_);

			}
			area_screen_->RefreshPad(id_screen_, scrollcross_, 0, start_y_, start_x_, area_screen_->GetMaxHeightMainWin() - 4, width_);
		}
	}
}

void AreaText::UpdateLine(BasicString string, int line, int hex_cursor)
{
	if (line > -1)
	{
		area_screen_->ClearLine(id_screen_, line, start_x_, width_, color_pair_);
		if (string.Size() != 0)
		{
			area_screen_->Print(id_screen_, line, start_x_, string, color_pair_);
		}
		area_screen_->RefreshPad(id_screen_, scrollcross_, 0, start_y_, start_x_ - length_numbering_, area_screen_->GetMaxHeightMainWin() - 4, width_);
	}
}

void AreaCommands::UpdateLine(BasicString string, int line, int hex_cursor)
{
	if (line == -1)
	{
		area_screen_->ClearLine(id_screen_, 0, 0, width_, color_pair_);
		if (string.Size() != 0)
		{
			area_screen_->Print(id_screen_, 0, 0, string, color_pair_);
		}
		length_commands_ = string.Size();
		area_screen_->SetCursor(start_y_, length_commands_);
		area_screen_->RefreshWin(id_screen_);
	}
}

void AreaState::UpdateLine(BasicString string, int line, int hex_cursor)
{
	if (line > -1)
	{
		area_screen_->RefreshWin(id_screen_);
		area_screen_->Refresh();
	}
}


void AreaNumbering::UpdateCursor(int y, int x, BasicString name, BasicString type)
{
	if (y < height_ + scrollcross_ + 3 && x < area_screen_->GetMaxWidthMainWin() )
	{
		if (y > 8)
		{
			area_screen_->Print(id_screen_, y, start_x_, y + 1, color_pair_);
			//area_screen_->Print(y, start_x_ + 2, ' ', color_pair_);
			y_cursor_ = y;
			x_cursor_ = x;
			area_screen_->RefreshPad(id_screen_, scrollcross_, 0, start_y_, start_x_, area_screen_->GetMaxHeightMainWin() - 4, width_);
		}
		else
		{
			area_screen_->Print(id_screen_, y, start_x_, ' ', color_pair_);
			area_screen_->Print(id_screen_, y, start_x_ + 1, y + 1, color_pair_);
			//area_screen_->Print(y, start_x_ + 2, ' ', color_pair_);
			y_cursor_ = y;
			x_cursor_ = x;
			area_screen_->RefreshPad(id_screen_, scrollcross_, 0, start_y_, start_x_, area_screen_->GetMaxHeightMainWin() - 4, width_);
		}
		
	}
}

void AreaText::UpdateCursor(int y, int x, BasicString name, BasicString type)
{
	if (y >= 0)
	{
		area_screen_->RefreshPad(id_screen_, scrollcross_, 0, start_y_, start_x_ - length_numbering_, area_screen_->GetMaxHeightMainWin() - 4, width_);
	}
}

void AreaCommands::UpdateCursor(int y, int x, BasicString name, BasicString type)
{
	if (y >= 0 && x >= 0)
	{
		area_screen_->ClearLine(id_screen_, 0, 0, width_, color_pair_);
		area_screen_->RefreshWin(id_screen_);
	}
	else
	{
		area_screen_->SetCursor(start_y_, length_commands_);
	}

}

void AreaState::UpdateCursor(int y, int x, BasicString name, BasicString type)
{
	if (x >= 0)
	{
		if (x < width_)
		{
			area_screen_->ClearLine(id_screen_, 0, 0, width_, color_pair_);
			area_screen_->ClearLine(id_screen_, 1, 0, width_, color_pair_);
			BasicString state(name);
			state.Insert(0, 1, '[');
			state.Insert(state.Size(), 1, ']');
			area_screen_->Print(id_screen_, 0, start_x_, state, color_pair_);

			state.Clear();
			state = type;
			state.Insert(0, 2, '-');
			state.Insert(state.Size(), 2, '-');
			area_screen_->Print(id_screen_, 1, start_x_, state, color_pair_);

			state = "Position: ";
			state += Translate(y + 1).CStr();
			state.PushBack('/');
			state += Translate(x + 1).CStr();

			int length = width_ - state.Size();
			area_screen_->Print(id_screen_, 1, length, state, color_pair_);
			y_cursor_ = y;
			x_cursor_ = x + length_numbering_;
			area_screen_->SetCursor(y_cursor_ - scrollcross_, x_cursor_);
			area_screen_->RefreshWin(id_screen_);
			area_screen_->Refresh();
		}
	}
	else
	{
		area_screen_->ClearLine(id_screen_, 0, 0, width_, color_pair_);
		area_screen_->ClearLine(id_screen_, 1, 0, width_, color_pair_);
		BasicString state(name);
		state.Insert(0, 1, '[');
		state.Insert(state.Size(), 1, ']');
		area_screen_->Print(id_screen_, 0, start_x_, state, color_pair_);

		state.Clear();
		state = type;
		state.Insert(0, 2, '-');
		state.Insert(state.Size(), 2, '-');
		area_screen_->Print(id_screen_, 1, start_x_, state, color_pair_);

		y_cursor_ = y;
		x_cursor_ = x + length_numbering_;
		area_screen_->RefreshWin(id_screen_);
		area_screen_->Refresh();
	}
}




/////HEX//////////////////
///////////////////////////////////
char TranslateToXOneSymbol(int number)
{
	if (number > 15)
		return 0;
	if (number < 10)
	{
		return ((char)(number + 48));
	}
	switch (number)
	{
	case 10: return 'A';
		break;
	case 11: return 'B';
		break;
	case 12: return 'C';
		break;
	case 13: return 'D';
		break;
	case 14: return 'E';
		break;
	case 15: return 'F';
		break;

	}
}
BasicString TranslateToX(int number, int width)
{
	BasicString return_line;
	while (number != 0 && return_line.Size() < width)
	{
		int os = number % 16;
		char symbol = TranslateToXOneSymbol(os);
		return_line.Insert(0, 1, symbol);
		number /= 16;
	}
	return_line.Insert(0, width - return_line.Size(), '0');
	return return_line;
}

HexShiftLines::HexShiftLines(IAdapterPDCur* area_screen, int y_start, int x_start, int hieght, int width, int width_hex_byte) :
	Area(area_screen, y_start + 1, x_start, hieght, width + 1, 4, COLOR_WHITE, COLOR_BLUE, TypeWindow::cPad)
{
	BasicString line;
	int number = 0;
	for (int i = 0; i < hieght; i++)
	{
		line = TranslateToX(number, width);
		line.Append(1, '|');
		area_screen_->Print(id_screen_, i, 0, line, color_pair_);
		line.Clear();
		number += width_hex_byte;
	}
	start_y_ = y_start + 1;
	start_x_ = x_start;
	width_hex_byte_ = width_hex_byte;
};


void HexHatTable::UpdateLine(BasicString string, int line, int hex_cursor)
{
	if (hex_cursor > -1)
	{
		if (line > -1)
		{
			BasicString line_string;
			line_string = TranslateToX(hex_cursor, width_full_shift_);
			line_string.Append(1, '|');
			area_screen_->Print(id_screen_, height_all_ - 1, x_full_shift_, line_string, color_pair_);
		}
	}
}

void HexShiftLines::UpdateLine(BasicString string, int line, int hex_cursor)
{
	if (line > -1)
	{
		if (hex_cursor > -1)
		{
			int number = hex_cursor;
			string.Clear();
			string = TranslateToX(number, width_ - 1);
			string.Append(1, '|');
			area_screen_->Print(id_screen_, line, 0, string, color_pair_);
		}
		else
		{
			int number = line * width_hex_byte_;
			string.Clear();
			string = TranslateToX(number, width_ - 1);
			string.Append(1, '|');
			area_screen_->Print(id_screen_, line, 0, string, color_pair_);
		}
	}
}

void HexBaseLines::UpdateLine(BasicString string, int line, int hex_cursor)
{
	if (line > -1)
	{
		int amount_block = 0;
		BasicString finish_string;
		if (string.Size() == 0)
		{
			for (int i = 1; i <= width_; i++)
			{
				finish_string.Append("00");
				i += 2;
				finish_string.Append(" ");
				amount_block++;
				if (amount_block == 4)
				{
					finish_string.Append(" ");
					i++;
					amount_block = 0;
				}
			}
		}
		else
		{
			int index = 0;
			for (int j = 0; j < width_; j++)
			{
				if (index < string.Size())
				{
					finish_string.Append(TranslateToX((int)(string[index++]), 2).CStr());
					finish_string.Append(" ");
					j += 2;
					amount_block++;
					if (amount_block == 4)
					{
						finish_string.Append(" ");
						j++;
						amount_block = 0;
					}
				}
				else
				{
					finish_string.Append("00");
					j += 2;
					finish_string.Append(" ");
					amount_block++;
					if (amount_block == 4)
					{
						finish_string.Append(" ");
						j++;
						amount_block = 0;
					}
				}
			}
		}
		finish_string.PopBack();
		finish_string.PopBack();
		finish_string.Append("|");
		area_screen_->ClearLine(id_screen_, line, 0, width_, color_pair_);
		area_screen_->Print(id_screen_, line, 0, finish_string, color_pair_);

		if (hex_cursor > -1)
		{
			hex_cursor -= (16) * line;
			finish_string.Clear();
			finish_string.Append(TranslateToX((int)(string[hex_cursor]), 2).CStr());
			int index = 0;
			amount_block = 0;
			for (int j = 0; j < width_; j++)
			{
				if (index == hex_cursor)
				{
					area_screen_->Print(id_screen_, line, j, finish_string, add_color_pair_);
					break;
				}
				j += 2;
				index++;
				amount_block++;
				if (amount_block == 4)
				{
					j++;
					amount_block = 0;
				}
			}

		}
		
	}

}

void HexStringLines::UpdateLine(BasicString string, int line, int hex_cursor)
{
	if (line > -1)
	{
		int amount_block = 0;
		BasicString finish_string;
		if (string.Size() == 0)
		{
			finish_string.Append(width_, ' ');
		}
		else
		{
			for (int i = 0; i < string.Size(); i++)
			{
				if (string[i] == (char)(0x0d) || string[i] == (char)(0x0a) || string[i] == (char)(0x09))
				{
					finish_string.Append(1, '.');
				}
				else
				{
					finish_string.Append(1, string[i]);
				}
			}
		}
		area_screen_->ClearLine(id_screen_, line, 0, width_, color_pair_);
		area_screen_->Print(id_screen_, line, 0, finish_string, color_pair_);
	}
}

void HexState::UpdateLine(BasicString string, int line, int hex_cursor)
{
	if (line > -1)
	{
		area_screen_->RefreshWin(id_screen_);
		area_screen_->Refresh();
	}
}

void HexCommands::UpdateLine(BasicString string, int line, int hex_cursor)
{
	if (line == -1)
	{
		area_screen_->ClearLine(id_screen_, 0, 0, width_, color_pair_);
		if (string.Size() != 0)
		{
			area_screen_->Print(id_screen_, 0, 0, string, color_pair_);
		}
		length_commands_ = string.Size();
		area_screen_->SetCursor(start_y_, length_commands_);
		area_screen_->RefreshWin(id_screen_);
	}
}




void HexHatTable::UpdateCursor(int y, int x, BasicString name, BasicString type)
{
	area_screen_->RefreshWin(id_screen_);
}

void HexShiftLines::UpdateCursor(int y, int x, BasicString name, BasicString type)
{
	area_screen_->RefreshPad(id_screen_, scrollcross_, 0, start_y_, start_x_, area_screen_->GetMaxHeightMainWin() - start_y_ - 1, width_ - 1);
}

void HexBaseLines::UpdateCursor(int y, int x, BasicString name, BasicString type)
{
	area_screen_->RefreshPad(id_screen_, scrollcross_, 0, start_y_, start_x_, area_screen_->GetMaxHeightMainWin() - start_y_ - 1, width_ + start_x_);
}

void HexStringLines::UpdateCursor(int y, int x, BasicString name, BasicString type)
{
	area_screen_->RefreshPad(id_screen_, scrollcross_, 0, start_y_, start_x_, area_screen_->GetMaxHeightMainWin() - start_y_ - 1, width_ + start_x_);
}

void  HexCommands::UpdateCursor(int y, int x, BasicString name, BasicString type)
{
	area_screen_->RefreshWin(id_screen_);
}

void HexState::UpdateCursor(int y, int x, BasicString name, BasicString type)
{
	if (x >= 0)
	{
		if (x < width_)
		{
			area_screen_->ClearLine(id_screen_, 0, 0, width_, color_pair_);
			area_screen_->ClearLine(id_screen_, 1, 0, width_, color_pair_);
			BasicString state(name);
			state.Insert(0, 1, '[');
			state.Insert(state.Size(), 1, ']');
			area_screen_->Print(id_screen_, 0, start_x_, state, color_pair_);

			state.Clear();
			state = type;
			state.Insert(0, 2, '-');
			state.Insert(state.Size(), 2, '-');
			area_screen_->Print(id_screen_, 1, start_x_, state, color_pair_);

			state = "Position: ";
			state += Translate(y + 1).CStr();
			state.PushBack('/');
			state += Translate(x + 1).CStr();

			int length = width_ - state.Size();
			area_screen_->Print(id_screen_, 1, length, state, color_pair_);
			y_cursor_ = y;
			x_cursor_ = x;
			area_screen_->SetCursor(y_start_string_lines_+ y_cursor_ - scrollcross_,x_start_string_lines_+ x_cursor_);
			area_screen_->RefreshWin(id_screen_);
			area_screen_->Refresh();
		}
	}
	else
	{
		area_screen_->ClearLine(id_screen_, 0, 0, width_, color_pair_);
		area_screen_->ClearLine(id_screen_, 1, 0, width_, color_pair_);
		BasicString state(name);
		state.Insert(0, 1, '[');
		state.Insert(state.Size(), 1, ']');
		area_screen_->Print(id_screen_, 0, start_x_, state, color_pair_);

		state.Clear();
		state = type;
		state.Insert(0, 2, '-');
		state.Insert(state.Size(), 2, '-');
		area_screen_->Print(id_screen_, 1, start_x_, state, color_pair_);

		y_cursor_ = y;
		x_cursor_ = x;
		area_screen_->RefreshWin(id_screen_);
		area_screen_->Refresh();
	}
}




