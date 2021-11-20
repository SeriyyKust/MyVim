#pragma once
#ifndef _I_MODEL_
#define _I_MODEL_

#include "View.h"
#include "workwithfile.h"
#include "MyString/mystring.h"

class IModel : public Observable
{
public:
	IModel(int height, int width, BasicString* buffer) : y_cursor_(0), x_cursor_(0), height_(height), width_(width), index_cursor_(0), amount_line_(0),
		base_buffer_(buffer), first_line_screen_(0), last_line_screen_(height - 1), save_(false) {
		file_ = nullptr;
	};

    virtual void Start(const BasicString& path) = 0;
    virtual void Continuation(const IModel* latest_model, bool after_hex) = 0;
    virtual void End() = 0;
	virtual int HandlingSymbol(const char symbol_getch) = 0;

   


	bool FindYX(int& y, int& x)
	{
		y = 0;
		x = 1;
		int position_start_line = 0;
		while (position_start_line < index_cursor_ && position_start_line < base_buffer_->Size())
		{
			if (base_buffer_->At(position_start_line) == '\n')
			{
				y++;
				x = 0;
			}
			else if (x == width_)
			{
				y++;
				x = 0;
			}
			x++;
			position_start_line++;
		}
		x--;
		if (position_start_line == base_buffer_->Size())
			return false;
		return true;
	}

	bool FindIndexCursor(int& index_cursor, int y, int x)
	{
		index_cursor = 0;

		int amount_line = 0;
		int index_width = 1;

		while (amount_line < y && index_cursor < base_buffer_->Size())
		{
			if (base_buffer_->At(index_cursor) == '\n')
			{
				amount_line++;
				index_width = 0;
			}
			else if (index_width == width_)
			{
				amount_line++;
				index_width = 0;
			}
			index_width++;
			index_cursor++;
		}
		index_width = 1;
		while (index_cursor < base_buffer_->Size())
		{
			if (base_buffer_->At(index_cursor) == '\n' || index_width > width_ || index_width - 1 == x)
			{
				return true;
			}
			else
			{
				index_width++;
				index_cursor++;
			}
		}
		return true;
	}

	int WidthLine(int line)
	{
		int amount_line = 0;
		int position_start_line = 0;
		int index_width = 1;

		while (amount_line < line && position_start_line < base_buffer_->Size())
		{
			if (base_buffer_->At(position_start_line) == '\n')
			{
				amount_line++;
				index_width = 0;
			}
			else if (index_width == width_)
			{
				amount_line++;
				index_width = 0;
			}
			index_width++;
			position_start_line++;
		}
		index_width = 1;
		int position_end_line = position_start_line;
		while (position_end_line < base_buffer_->Size() && base_buffer_->At(position_end_line) != '\n' && index_width <= width_)
		{
			index_width++;
			position_end_line++;
		}
		return position_end_line - position_start_line;
	}

	void MoveRight()
	{
		if (index_cursor_ >= base_buffer_->Size())
			return;
		index_cursor_++;
		FindYX(y_cursor_, x_cursor_);
		return;
	}

	void MoveLeft()
	{
		if (index_cursor_ - 1 < 0)
			return ;
		index_cursor_--;
		FindYX(y_cursor_, x_cursor_);
		return ;
	}

	void MoveUp()
	{
		if (y_cursor_ - 1 < 0)
			return ;
		y_cursor_--;
		int size_line = WidthLine(y_cursor_);
		if (size_line >= x_cursor_)
		{
			FindIndexCursor(index_cursor_, y_cursor_, x_cursor_);
			return;
		}
		else
		{
			x_cursor_ = size_line;
			FindIndexCursor(index_cursor_, y_cursor_, x_cursor_);
			return;
		}
	}

	void MoveDown()
	{
		if (y_cursor_ + 1 >= amount_line_)
			return ;
		y_cursor_++;
		int size_line = WidthLine(y_cursor_);
		if (size_line >= x_cursor_)
		{
			FindIndexCursor(index_cursor_, y_cursor_, x_cursor_);
			return;
		}
		else
		{
			x_cursor_ = size_line;
			FindIndexCursor(index_cursor_, y_cursor_, x_cursor_);
			return;
		}
	}

	int Screen—rossing—heck()
	{
		if (y_cursor_ < first_line_screen_)
		{
			first_line_screen_--;
			last_line_screen_--;
			return -1;
		}
		else if (y_cursor_ > last_line_screen_)
		{
			first_line_screen_++;
			last_line_screen_++;
			return 1;
		}
		return 0;
	}

	int Translate(BasicString string)
	{
		if (string.Size() == 0)
			return 0;
		for (int i = 0; i < string.Size(); i++)
		{
			if (string[i] < '0' || string[i] > '9')
				return -1;
		}
		int number = 0;
		for (int i = 0; i < string.Size(); i++)
		{
			number = (number * 10) + ((int)(string[i])-48);
		}
		return number;
	}

	inline int GetFirstLineScreen() const 
	{
		return first_line_screen_;
	}
	inline int GetLastLineScreen() const 
	{
		return last_line_screen_;
	}
	inline int GetHeight() const
	{
		return height_;
	}
	inline int GetWidth() const
	{
		return width_;
	}
	inline int GetYCursor() const
	{
		return y_cursor_;
	}
	inline int GetXCursor() const
	{
		return x_cursor_;
	}
	inline int GetIndexCursor() const
	{
		return index_cursor_;
	}
	inline int GetAmountLine() const
	{
		return amount_line_;
	}
	inline bool GetSave() const 
	{ 
		return save_; 
	}
	inline BasicString GetFilePath() const
	{
		return file_->GetPath();
	}

protected:
	BasicString* base_buffer_;
	WorkWithFile* file_;
	int index_cursor_;
	int y_cursor_;
	int x_cursor_;
	int height_;
	int width_;
	int amount_line_;

	int first_line_screen_;
	int last_line_screen_;
	bool save_;
};


#endif // _I_MODEL_