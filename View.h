#pragma once
#ifndef _VIEW_H_
#define _VIEW_H_

#include "Adapter.h"
#include "observable.h"
#include "observer.h"
#include "imodel.h"

const int cMaxAmountLine = 150;

class Area : public Observer
{
public:
	enum class TypeWindow
	{
		cWin = 0,
		cPad = 1
	};
	Area(IAdapterPDCur* area_screen, int y, int x, int height, int width, int color_pair, int color_text, int color_background, TypeWindow type);
	inline int width() const 
	{
		return width_;
	}
	inline int height() const 
	{
		return height_;
	}
	virtual void UpdateCross(bool step)
	{
		if (step)
			scrollcross_++;
		else
			scrollcross_--;
	}
	void GetCursor(int& y, int& x);
	virtual void UpdateType(TypeUpdate type){}

	~Area() {};

protected:
	IAdapterPDCur* area_screen_;
	int start_y_;
	int start_x_;
	int height_;
	int width_;
	int color_pair_;
	int id_screen_;

	int scrollcross_;
	int length_numbering_;

	int y_cursor_;
	int x_cursor_;
};

class AreaText : public Area
{
public :
	AreaText(IAdapterPDCur* area_screen, int y, int x, int height, int width) :
		Area(area_screen, y, x, height, width,1,COLOR_WHITE,COLOR_BLACK, TypeWindow::cPad) {}
	virtual void FinishScreen() {}
	virtual void UpdateLine(BasicString string, int line, int hex_cursor) override;
	virtual void UpdateType(TypeUpdate type){}
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type)override;
	~AreaText() {};
private:
};

class AreaNumbering : public Area
{
public :
	AreaNumbering(IAdapterPDCur* area_screen, int y, int x, int height, int width) :
		Area(area_screen, y, x, height, width, 2, COLOR_CYAN, COLOR_BLACK, TypeWindow::cPad) 
	{
		for (int i = 0; i < height; i++)
			area_screen_->Print(id_screen_, i, start_x_, "~ ", color_pair_);
	}
	
	virtual void FinishScreen() {}
	virtual void UpdateLine(BasicString string, int line, int hex_cursor) override;
	virtual void UpdateType(TypeUpdate type){}
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type)override;
	~AreaNumbering() {};
private:
};

class AreaState : public Area
{
public:
	AreaState(IAdapterPDCur* area_screen, int y, int x, int height, int width) :
		Area(area_screen, y, x, height, width, 1, COLOR_WHITE, COLOR_BLACK, TypeWindow::cWin) {};
	
	virtual void FinishScreen() {}
	virtual void UpdateLine(BasicString string, int line, int hex_cursor) override;
	virtual void UpdateType(TypeUpdate type){}
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type)override;
	~AreaState() {};
};

class AreaCommands : public Area
{
public:
	AreaCommands(IAdapterPDCur* area_screen, int y, int x, int height, int width) :
		Area(area_screen, y, x, height, width, 2, COLOR_CYAN, COLOR_BLACK, TypeWindow::cWin) {
		length_commands_ = 0;
	};
	
	virtual void FinishScreen() {}
	virtual void UpdateLine(BasicString string, int line, int hex_cursor) override;
	virtual void UpdateType(TypeUpdate type){}
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type)override;
	~AreaCommands() {};
private:
	int length_commands_;
};




class HexHatTable : public Area
{
public:
	HexHatTable(IAdapterPDCur* area_screen, int y_full_shift, int x_full_shift,  int width_full_shift,
		int y_line_shift, int x_line_shift, int width_line_shift, int y_symbol_column, int x_symbol_column, int width_symbol_column, int height_all) :
		Area(area_screen, y_full_shift, x_full_shift, height_all  + 1, width_full_shift + width_line_shift + width_symbol_column + 2,
			4, COLOR_WHITE, COLOR_BLUE, TypeWindow::cWin) , y_full_shift_(y_full_shift), x_full_shift_(x_full_shift), width_full_shift_(width_full_shift),
		y_line_shift_(y_line_shift), x_line_shift_(x_line_shift), width_line_shift_(width_line_shift), y_symbol_column_(y_symbol_column), x_symbol_column_(x_symbol_column),
		width_symbol_column_(width_symbol_column), height_all_(height_all)
	{
		y_cursor_ = 0; x_cursor_ = 0;
		BasicString line_string;
		for (int i = 0; i < height_all_ - 1; i++)
		{
			line_string.Append(width_full_shift_, ' ');
			line_string.Append(1, '|');
			line_string.Append(width_line_shift_, ' ');
			line_string.Append(1, '|');
			line_string.Append(width_symbol_column_, ' ');
			area_screen_->Print(id_screen_, y_full_shift_, x_full_shift_, line_string, color_pair_);
		}
		line_string.Clear();
		line_string.Append(width_full_shift_, '0');
		line_string.Append(1, '|');
		area_screen_->Print(id_screen_, height_all_ - 1, x_full_shift_, line_string, color_pair_);
		line_string.Clear();
		int amount_block = 0;
		int number = 0;
		for (int i = 1; i <= width_line_shift_; i++)
		{

			area_screen_->PrintX(id_screen_, height_all_ - 1, x_line_shift_ + i, number, color_pair_);
			i += 2;
			number++;
			area_screen_->Print(id_screen_, height_all_ - 1, x_line_shift_ + i, ' ', color_pair_);
			amount_block++;
			if (amount_block == 4)
			{
				area_screen_->Print(id_screen_, height_all_ - 1, x_line_shift_ + i, "  ", color_pair_);
				i++;
				amount_block = 0;
			}
		}
		line_string.Clear();
		line_string.Append(1, '|');
		line_string += " SYMBOLS";
		line_string.Append(width_symbol_column_ - line_string.Size() + 1, ' ');
		area_screen_->Print(id_screen_, height_all_ - 1, x_symbol_column_ + 1, line_string, color_pair_);
		line_string.Clear();
		line_string.Append(width_full_shift_, '-');
		line_string.Append(1, '|');
		line_string.Append(width_line_shift_, '-');
		line_string.Append(1, '|');
		line_string.Append(width_symbol_column_, '-');
		area_screen_->Print(id_screen_, height_all_, x_full_shift_, line_string, color_pair_);
	};
	

	virtual void UpdateLine(BasicString string, int line, int hex_cursor) override;
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type) override;

	virtual void UpdateType(TypeUpdate type){}
	~HexHatTable() {};
private:
	int y_full_shift_;
	int x_full_shift_;
	int width_full_shift_;
	int y_line_shift_;
	int x_line_shift_;
	int width_line_shift_;

	int y_symbol_column_;
	int x_symbol_column_;
	int width_symbol_column_;
	int height_all_;
};

class HexShiftLines : public Area
{
public:
	HexShiftLines(IAdapterPDCur* area_screen, int y_start, int x_start, int hieght ,int width, int width_hex_byte);


	virtual void UpdateLine(BasicString string, int line, int hex_cursor) override;
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type) override;




	virtual void FinishScreen() {}
	virtual void AreaBeginApp() {};
	virtual void UpdateType(TypeUpdate type){}
	

	~HexShiftLines() {};

private:
	int width_hex_byte_;
};

class HexBaseLines : public Area
{
public:
	HexBaseLines(IAdapterPDCur* area_screen, int y_start, int x_start, int hieght, int width, int amount_elements_on_line) :
		Area(area_screen, y_start+1, x_start+1, hieght, width + 1, 4, COLOR_WHITE, COLOR_BLUE, TypeWindow::cPad)
	{
		BasicString line;
		int amount_block = 0;
		for (int i = 0; i < hieght; i++)
		{
			UpdateLine(line, i, -1);
		}
		start_y_ = y_start + 1;
		start_x_ = x_start + 1;
		area_screen_->CreateColorPair(5, COLOR_BLUE, COLOR_WHITE);
		add_color_pair_ = 5;
	};

	virtual void UpdateLine(BasicString string, int line, int hex_cursor) override;
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type) override;



	virtual void FinishScreen() {}
	virtual void AreaBeginApp() {};
	virtual void UpdateType(TypeUpdate type){}
	~HexBaseLines() {};
private:
	int add_color_pair_;
};

class HexStringLines : public Area
{
public:
	HexStringLines(IAdapterPDCur* area_screen, int y_start, int x_start, int hieght, int width) :
		Area(area_screen, y_start + 1, x_start + 2, hieght, width, 4, COLOR_WHITE, COLOR_BLUE, TypeWindow::cPad)
	{
		BasicString line;
		int amount_block = 0;
		for (int i = 0; i < hieght; i++)
		{
			UpdateLine(line, i, -1);
		}
		start_y_ = y_start + 1;
		start_x_ = x_start + 2;
	};
	virtual void UpdateLine(BasicString string, int line, int hex_cursor) override;
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type) override;


	virtual void FinishScreen() {}
	virtual void UpdateType(TypeUpdate type){}
	
	~HexStringLines() {};
private:

};

class HexState : public Area
{
public:
	HexState(IAdapterPDCur* area_screen, int y, int x, int height, int width, int y_start_string_lines, int x_start_string_lines) :
		Area(area_screen, y, x, height, width, 4, COLOR_WHITE, COLOR_BLUE, TypeWindow::cWin), y_start_string_lines_(y_start_string_lines + 1), x_start_string_lines_(x_start_string_lines + 2)
	{
		area_screen_->ClearLine(id_screen_, 0, 0, width_,color_pair_);
		area_screen_->ClearLine(id_screen_, 1, 0, width_, color_pair_);
		BasicString state;
		state.Insert(0, 1, '[');
		state.Insert(state.Size(), 1, ']');
		state.Append(width_ - state.Size(), ' ');
		area_screen_->Print(id_screen_, 0, start_x_, state, color_pair_);

		state.Clear();
		state = "HEX";
		state.Insert(0, 2, '-');
		state.Insert(state.Size(), 2, '-');

		state.Append(width_ - state.Size() - strlen("Position: 0/0"),' ');

		state.Append("Position: 0/0");

		area_screen_->Print(id_screen_, 1, start_x_, state, color_pair_);
		y_cursor_ = y;
		x_cursor_ = x;
	};

	virtual void UpdateLine(BasicString string, int line, int hex_cursor) override;
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type) override;


	virtual void FinishScreen() {}
	virtual void UpdateType(TypeUpdate type){}
	

	~HexState() {};
private:
	int y_start_string_lines_;
	int x_start_string_lines_;
};

class HexCommands : public Area
{
public:
	HexCommands(IAdapterPDCur* area_screen, int y, int x, int height, int width) :
		Area(area_screen, y, x, height, width, 4, COLOR_CYAN, COLOR_BLUE, TypeWindow::cWin)
	{
		BasicString string;
		string.Append(width_, ' ');
		area_screen_->ClearLine(id_screen_, 0, 0, width_, color_pair_);
		if (string.Size() != 0)
		{
			area_screen_->Print(id_screen_, 0, 0, string, color_pair_);
		}
		length_commands_ = string.Size();
	};
	virtual void UpdateLine(BasicString string, int line, int hex_cursor) override;
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type) override;



	virtual void UpdateType(TypeUpdate type){}
	
	virtual void FinishScreen() {}
	~HexCommands() {};
private:
	int length_commands_;
};


class View : public Observable, public Observer
{
public:
	View(IAdapterPDCur* screen);
	virtual void UpdateLine(BasicString string, int line, int hex_cursor) override;
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type)override;
	virtual void UpdateCross(bool type)override;

	virtual void UpdateType(TypeUpdate type)override;





	~View();
private:
	IAdapterPDCur* screen_;
	AreaText* text_;
	AreaNumbering* numbering_;
	AreaState* state_;
	AreaCommands* command_line_;
	int height_;
	int width_;
	int length_numbering_;

	TypeUpdate type_;

	HexHatTable* hex_hat_table_;
	HexShiftLines* hex_shift_lines_;
	HexBaseLines* hex_base_line_;
	HexStringLines* hex_string_lines_;
	HexState* hex_state_;
	HexCommands* hex_commands_;
};
#endif //_VIEW_H_

