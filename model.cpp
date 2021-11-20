#include "model.h"

int amount_line_check(BasicString * string, int width, bool type)
{
	int index = 0;
	int amount_symbol = 0;
	int amount_line = 0;
	while (index < string->Size())
	{
		if (string->At(index) == '\n')
		{
			if (type)
			{
				amount_line++;
				amount_symbol = 0;
			}
			else
			{
				amount_symbol++;
				if (amount_symbol == width)
				{
					amount_line++;
					amount_symbol = 0;
				}
			}
		}
		else
		{
			amount_symbol++;
			if (amount_symbol == width)
			{
				amount_line++;
				amount_symbol = 0;
			}
		}
		index++;
	}
	if (amount_symbol > 0)
		amount_line++;
	return amount_line;
}

////////////////////////////////////////////////////
///////////////////////////////////////////////////////
////////////TTTTTTTTTTTTTTTTTTTTTTTT?/////////////////////
//PART 1@@@@@@@@@@@@@@@
void NavigationEdit::Start(const BasicString& path)
{
	AddObserver(screen_);
	base_buffer_->Clear();
	file_ = new WorkWithFile(path);
	amount_line_ = file_->GetAmountLine(width_);
	BasicString line;
	int length = 0;
	for (int i = 0; i < amount_line_; i++)
	{
		line = file_->GetLine(i, width_);
		ChangeLine(line, i, 0, 0, 1);
		base_buffer_->Insert(length, line.CStr());
		length = base_buffer_->Size();
		line.Clear();
	}
	line.Clear();
	for (int i = amount_line_; i <height_; i++)
		ChangeLine(line, i, 0, 0, 1);
	ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "NAVIGATION AND EDIT", 0, 1);
	DeleteObserver();
}

BasicString NavigationEdit::GetLine(int number_line, int width)
{
	int index = 0;
	BasicString line;
	if (base_buffer_->Size() == 0)
	{
		return line;
	}
	char buffer;
	int amount_symbol = 0;
	int amount_line = 0;
	int amount = 0;
	while (index < base_buffer_->Size() && amount_line < number_line)
	{
		buffer = base_buffer_->At(index++);
		amount++;
		if (buffer == '\n')
		{
			amount_line++;
			amount_symbol = 0;
		}
		else
		{
			amount_symbol++;
			if (amount_symbol == width)
			{
				amount_line++;
				amount_symbol = 0;
			}
		}
	}
	if (amount_line > number_line)
		return line;
	while (index < base_buffer_->Size())
	{
		buffer = base_buffer_->At(index++);
		if (buffer == '\n')
		{
			line.PushBack(buffer);
			return line;
		}
		else
		{
			amount_symbol++;
			line.PushBack(buffer);
			if (amount_symbol == width)
			{
				amount_line++;
				amount_symbol = 0;
				return line;
			}
		}
	}
	line.PopBack();
	return line;
}

void NavigationEdit::Continuation(const IModel* latest_model, bool after_hex)
{
	first_line_screen_ = latest_model->GetFirstLineScreen();
	last_line_screen_ = latest_model->GetLastLineScreen();
	index_cursor_ = latest_model->GetIndexCursor();
	y_cursor_ = latest_model->GetYCursor();
	x_cursor_ = latest_model->GetXCursor();
	save_ = latest_model->GetSave();
	if (file_ == nullptr)
	{
		file_ = new WorkWithFile(latest_model->GetFilePath());
	}
	else if (!(strcmp((file_->GetPath()).CStr(), (latest_model->GetFilePath()).CStr()) == 0))
	{
		delete file_;
		BasicString newPath = latest_model->GetFilePath().CStr();
		Start(newPath);
	}
	AddObserver(screen_);
	if (after_hex)
	{
		y_cursor_ = latest_model->GetYCursor();
		x_cursor_ = latest_model->GetXCursor();
		first_line_screen_ = 0;
		last_line_screen_ = height_ - 1;
		int index = index_cursor_;
		index_cursor_ = 0;
		while (index > index_cursor_)
		{
			MoveRight();
			int check = Screen—rossing—heck();
			if (check > 0)
				ChangeCross(true, 0, 1);
			if (check < 0)
				ChangeCross(false, 0, 1);
		}
		FindYX(y_cursor_, x_cursor_);
		amount_line_ = amount_line_check(base_buffer_, width_, true);
		BasicString line;
		int i = 0;
		for ( ;i < amount_line_; i++)
		{
			line = GetLine(i, width_);
			ChangeLine(line, i, 0, 0, 1);
			line.Clear();
		}
		line.Clear();
		for (; i < height_; i++)
		{
			ChangeLine(line, i, 0, 0, 1);
		}
	}
	ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "NAVIGATION AND EDIT", 0, 1);
}

void NavigationEdit::End()
{
	DeleteObserver();
}

int NavigationEdit::HandlingSymbol(const char symbol_getch)
{
	amount_line_ = amount_line_check(base_buffer_, width_,true);
	if (symbol_getch == cEscape)
	{
		buffer_command_.Clear();
		return 0;
	}
	if (symbol_getch == '\r')
	{
		ChangeType(TypeUpdate::cHex);
		buffer_command_.Clear();
		return 5;
	}
	buffer_command_.Insert(buffer_command_.Size(), 1, symbol_getch);
	if (!CheckBufferCommand())
	{
		buffer_command_.Clear();
		return 0;
	}
	if (buffer_command_.At(0) == 'y')
	{
		if (buffer_command_.Size() == 2)
		{
			if (strcmp(buffer_command_.CStr(), "yw") == 0)
			{
				CopyWordToBuffer();
				buffer_command_.Clear();
			}
			buffer_command_.Clear();
		}
		else
		{
			CopyLineToBuffer();
		}
	}

	else if (buffer_command_.At(0) >= '0' && buffer_command_.At(0) <= '9')
	{
		if (!(buffer_command_.At(buffer_command_.Size() - 1) >= '0' && buffer_command_.At(buffer_command_.Size() - 1) <= '9'))
		{
			if (buffer_command_.At(buffer_command_.Size() - 1) == 'G')
			{
				int line = Translate(buffer_command_.Substr(0, buffer_command_.Size() - 1)) - 1;
				if (line + 1 > amount_line_)
				{
					line = amount_line_ - 1;
				}
				if (y_cursor_ < line)
				{
					while (y_cursor_ < line)
					{
						MoveDown();
						int check = Screen—rossing—heck();
						if (check > 0)
							ChangeCross(true, 0, 1);
						if (check < 0)
							ChangeCross(false, 0, 1);
						ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "NAVIGATION AND EDIT", 0, 1);
					}
				}
				else if (y_cursor_ > line)
				{
					while (y_cursor_ > line)
					{
						MoveUp();
						int check = Screen—rossing—heck();
						if (check > 0)
							ChangeCross(true, 0, 1);
						if (check < 0)
							ChangeCross(false, 0, 1);
						ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "NAVIGATION AND EDIT", 0, 1);
					}
				}
				buffer_command_.Clear();
			}
		}
		buffer_command_.Clear();
	}
	else if (buffer_command_.Size() == 1)
	{
		switch (symbol_getch)
		{
			case (cRight):
			{
				MoveRight();
				buffer_command_.Clear();
				break;
			}
			case ('l'):
			{
				MoveRight();
				buffer_command_.Clear();
				break;
			}
			case (cLeft):
			{
				MoveLeft();
				buffer_command_.Clear();
				break;
			}
			case ('h'):
			{
				MoveLeft();
				buffer_command_.Clear();
				break;
			}
			case (cUp):
			{
				MoveUp();
				buffer_command_.Clear();
				break;
			}
			case ('k'):
			{
				MoveUp();
				buffer_command_.Clear();
				break;
			}
			case (cDown):
			{
				MoveDown();
				buffer_command_.Clear();
				break;
			}
			case ('j'):
			{
				MoveDown();
				buffer_command_.Clear();
				break;
			}
			case ('G'):
			{
				GoToTheEnd();
				buffer_command_.Clear();
				break;
			}
			case ('^'):
			{
				GoToTheBeginningLine();
				buffer_command_.Clear();
				break;
			}
			case ('$'):
			{
				GoToTheEndLine();
				buffer_command_.Clear();
				break;
			}
			case ('w'):
			{
				GoToWordEnd();
				buffer_command_.Clear();
				break;
			}
			case ('b'):
			{
				GoToWordFront();
				buffer_command_.Clear();
				break;
			}
			case ('x'):
			{
				DeleteSymbols(index_cursor_, 1);
				save_ = true;
				buffer_command_.Clear();
				break;
			}
			case ('p'):
			{
				InsertFromBufferToLine();
				save_ = true;
				buffer_command_.Clear();
				break;
			}
			case ('i'):
			{
				MoveRight();
				int check = Screen—rossing—heck();
				if (check > 0)
					ChangeCross(true, 0, 1);
				if (check < 0)
					ChangeCross(false, 0, 1);
				ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
				buffer_command_.Clear();
				save_ = true;
				return 2;
				break;
			}
			case ('I'):
			{
				GoToTheBeginningLine();
				int check = Screen—rossing—heck();
				if (check > 0)
					ChangeCross(true, 0, 1);
				if (check < 0)
					ChangeCross(false, 0, 1);
				ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
				buffer_command_.Clear();
				save_ = true;
				return 2;
				break;
			}
			case ('A'):
			{
				GoToTheEndLine();
				if (x_cursor_ < (width_ - 1))
					index_cursor_++;
				FindYX(y_cursor_, x_cursor_);
				int check = Screen—rossing—heck();
				if (check > 0)
					ChangeCross(true, 0, 1);
				if (check < 0)
					ChangeCross(false, 0, 1);
				ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
				buffer_command_.Clear();
				save_ = true;
				return 2;
				break;
			}
			case ('S'):
			{

				GoToTheBeginningLine();
				int start_index = index_cursor_;
				GoToTheEndLine();
				int finish_index = index_cursor_;
				if (base_buffer_->Size() == start_index)
					return 2;
				base_buffer_->Erase(start_index, finish_index - start_index + 1);
				if (base_buffer_->Size() <= start_index)
					index_cursor_ = base_buffer_->Size() - 1;
				else
					index_cursor_ = start_index;
				//index_cursor_--;
				FindYX(y_cursor_, x_cursor_);
				if (base_buffer_->At(index_cursor_) != '\n')
					base_buffer_->Insert(index_cursor_, 1, '\n');
				FindYX(y_cursor_, x_cursor_);
				UpdateLines(y_cursor_+1);
				save_ = true;
				int check = Screen—rossing—heck();
				if (check > 0)
					ChangeCross(true, 0, 1);
				if (check < 0)
					ChangeCross(false, 0, 1);
				ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
				buffer_command_.Clear();
				return 2;
				break;
			}
			case ('r'):
			{
				ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
				DeleteSymbols(index_cursor_, 1);

				save_ = true;
				buffer_command_.Clear();
				break;
			}
		default:
			break;
		}
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true, 0, 1);
		if (check < 0)
			ChangeCross(false, 0, 1);
		ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "NAVIGATION AND EDIT", 0, 1);
	}
	else if (buffer_command_.Size() == 2)
	{
		if (strcmp(buffer_command_.CStr(), "gg") == 0)
		{
			GoToTheBeginning();
			buffer_command_.Clear();
		}
		else if (strcmp(buffer_command_.CStr(), "dd") == 0)
		{
			GoToTheBeginningLine();
			int start_index = index_cursor_;
			GoToTheEndLine();
			int finish_index = index_cursor_;
			DeleteSymbols(start_index, finish_index - start_index + 1);
			FindYX(y_cursor_, x_cursor_);
			if (base_buffer_->At(index_cursor_) == '\n')
			{
				base_buffer_->Erase(index_cursor_, 1);
				UpdateLines(y_cursor_ - 1);
			}
				
			save_ = true;
			buffer_command_.Clear();
		}
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true, 0, 1);
		if (check < 0)
			ChangeCross(false, 0, 1);
		ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "NAVIGATION AND EDIT", 0, 1);
	}
	else if (buffer_command_.Size() == 3)
	{
		if (strcmp(buffer_command_.CStr(), "diw") == 0)
		{
			if (base_buffer_->At(index_cursor_) == '\n')
			{
				DeleteSymbols(index_cursor_, 1);
			}
			else
			{
				GoToWordFront();
				int start_index = index_cursor_;
				GoToWordEnd();
				int finish_index = index_cursor_;
				if ((finish_index + 1) < base_buffer_->Size())
					++finish_index;

				DeleteSymbols(start_index, finish_index - start_index + 1);
			}
			save_ = true;
			buffer_command_.Clear();
		}
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true, 0, 1);
		if (check < 0)
			ChangeCross(false, 0, 1);
		ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "NAVIGATION AND EDIT", 0, 1);
	}
	return 0;
 }

void NavigationEdit::GoToTheEnd()
{
	index_cursor_ = base_buffer_->Size() - 1;
	int y = 0;
	FindYX(y, x_cursor_);
	while (y_cursor_ < y)
	{
		y_cursor_++;
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true, 0, 1);
		if (check < 0)
			ChangeCross(false, 0, 1);
	}
}

void NavigationEdit::GoToTheBeginning()
{
	index_cursor_ = 0;
	int y = y_cursor_;
	FindYX(y, x_cursor_);
	while (y_cursor_ > y)
	{
		--y_cursor_;
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true, 0, 1);
		if (check < 0)
			ChangeCross(false, 0, 1);
	}
}

void NavigationEdit::GoToTheBeginningLine()
{
	while (x_cursor_ > 0)
	{
		--x_cursor_;
		--index_cursor_;
	}
}

void NavigationEdit::GoToTheEndLine()
{
	while (index_cursor_ <  base_buffer_->Size() &&  x_cursor_ < width_ && base_buffer_->At(index_cursor_) != '\n')
	{
		++x_cursor_;
		++index_cursor_;
	}
	if (x_cursor_ != 0)
	{
		--x_cursor_;
		--index_cursor_;
	}
}

void NavigationEdit::GoToWordEnd()
{
	int index = index_cursor_ + 1;
	while (index < base_buffer_->Size() && (base_buffer_->At(index) == ' ' || base_buffer_->At(index) == '\n'))
		index++;
	if (index < base_buffer_->Size() && (base_buffer_->At(index) != ' ' && base_buffer_->At(index) != '\n'))
	{
		while (index < base_buffer_->Size() && (base_buffer_->At(index) != ' ' && base_buffer_->At(index) != '\n'))
			index++;
		index_cursor_ = index - 1;
		FindYX(y_cursor_, x_cursor_);
		return;
	}
	return;
}

void NavigationEdit::GoToWordFront()
{
	int index = index_cursor_ - 1;
	while (index > -1 && (base_buffer_->At(index) == ' ' || base_buffer_->At(index) == '\n'))
		--index;
	if (index > -1 && (base_buffer_->At(index) != ' ' && base_buffer_->At(index) != '\n'))
	{
		while (index > -1 && (base_buffer_->At(index) != ' ' && base_buffer_->At(index) != '\n'))
			--index;
		index_cursor_ = index + 1;
		FindYX(y_cursor_, x_cursor_);
		return;
	}
	return;
}

void NavigationEdit::UpdateLines(int edit_line)
{
	int amount_line = 0;
	int position_start_line = 0;

	int index_width = 1;

	while (amount_line < (edit_line -1) && position_start_line < base_buffer_->Size())
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
	while (position_end_line < base_buffer_->Size())
	{
		if (base_buffer_->At(position_end_line) == '\n')
		{
			if (position_start_line != position_end_line)
			{
				ChangeLine(base_buffer_->Substr(position_start_line, position_end_line - position_start_line), amount_line,0, 0, 1);
			}
			else
			{
				BasicString space(" ");
				ChangeLine(space, amount_line,0, 0, 1);
			}
			amount_line++;
			index_width = 1;
			position_end_line++;
			position_start_line = position_end_line;
		}
		else if (index_width > width_)
		{
			if (position_start_line != position_end_line)
			{
				ChangeLine(base_buffer_->Substr(position_start_line, position_end_line - position_start_line), amount_line,0, 0, 1);
			}
			amount_line++;
			index_width = 1;
			position_start_line = position_end_line;
		}
		else
		{
			index_width++;
			position_end_line++;
		}
	}
	if (position_start_line != position_end_line)
	{
		ChangeLine(base_buffer_->Substr(position_start_line, position_end_line - position_start_line), amount_line,0, 0, 1);
		amount_line++;
	}
	if (amount_line < amount_line_)
	{
		BasicString empty;
		for (int i = amount_line; i < amount_line_ ; i++)
			ChangeLine(empty, i,0, 0, 1);
		amount_line_ = amount_line + 1;
	}
	if (base_buffer_->Size() == 0)
	{
		BasicString empty;
		ChangeLine(empty, 0,0, 0, 1);
		y_cursor_ = 0;
		x_cursor_ = 0;
		index_cursor_ = 0;
	}
}

void NavigationEdit::DeleteSymbols(int position, int count)
{
	if (base_buffer_->Size() == position)
		return;
	base_buffer_->Erase(position, count);
	if (base_buffer_->Size() <= position)
		index_cursor_ = base_buffer_->Size() - 1;
	else
		index_cursor_ = position;
	FindYX(y_cursor_, x_cursor_);
	UpdateLines(y_cursor_);
}

void NavigationEdit::CopyLineToBuffer()
{
	int index_cursor = index_cursor_;
	GoToTheBeginningLine();
	int index_cursor_start = index_cursor_;
	GoToTheEndLine();
	int index_cursor_finish = index_cursor_;
	buffer_line_ = base_buffer_->Substr(index_cursor_start, index_cursor_finish - index_cursor_start + 1);
	index_cursor_ = index_cursor;
}

void NavigationEdit::InsertFromBufferToLine()
{
	if (buffer_line_.Empty())
		return;
	base_buffer_->Insert(index_cursor_ + 1, buffer_line_.CStr());
	UpdateLines(y_cursor_);
}

void NavigationEdit::CopyWordToBuffer()
{
	int index_cursor = index_cursor_;
	GoToWordEnd();
	int index_cursor_finish = index_cursor_;
	buffer_line_ = base_buffer_->Substr(index_cursor, index_cursor_finish - index_cursor + 1);
	index_cursor_ = index_cursor;
	FindYX(y_cursor_, x_cursor_);
}

bool NavigationEdit::CheckBufferCommand()
{
	BasicString first_symbol = { '^','$','w','b','g','G','x','d','y','p','i','I','A','S','r',(char)(cRight),(char)(cUp),(char)(cLeft),(char)(cDown), 'l','k','j','h'};
	BasicString second_symbol = { 'g','i','d','w' };
	char symbol = 'w';
	if (buffer_command_[0] >= '0' && buffer_command_[0] <= '9')
	{
		for (int i = 0; i < buffer_command_.Size() - 1; i++)
		{
			if (!(buffer_command_[i] >= '0' && buffer_command_[i] <= '9'))
				return false;
		}
	}
	else
	{
		int size = buffer_command_.Size();
		if (size == 1)
		{
			for (int i = 0; i < first_symbol.Size(); i++)
			{
				if (first_symbol[i] == buffer_command_[0])
					return true;
			}
			return false;
		}
		else if (size == 2)
		{
			for (int i = 0; i < second_symbol.Size(); i++)
			{
				if (second_symbol[i] == buffer_command_[1])
					return true;
			}
			return false;
		}
		else if (size == 3)
		{
			if (symbol == buffer_command_[2])
				return true;
			return false;
		}
	}
}

////////////////////////////////////////////////////
///////////////////////////////////////////////////////
////////////TTTTTTTTTTTTTTTTTTTTTTTT?/////////////////////
//PART 2@@@@@@@@@@@@@@@

void CommandModel::Start(const BasicString& path)
{
	AddObserver(screen_);
	file_ = new WorkWithFile(path);
	amount_line_ = file_->GetAmountLine(width_);
	DeleteObserver();
}

void CommandModel::Continuation(const IModel* latest_model, bool after_hex)
{
	first_line_screen_ = latest_model->GetFirstLineScreen();
	last_line_screen_ = latest_model->GetLastLineScreen();
	index_cursor_ = latest_model->GetIndexCursor();
	y_cursor_ = latest_model->GetYCursor();
	x_cursor_ = latest_model->GetXCursor();
	save_ = latest_model->GetSave();
	if (file_ == nullptr)
	{
		file_ = new WorkWithFile(latest_model->GetFilePath());
	}
	AddObserver(screen_);
	if (buffer_command_.Size() != 0)
		buffer_command_.Clear();
	buffer_command_.PushBack(':');
	ChangeLine(buffer_command_, -1,0, 0, 1);
	ChangeCursor(-1, -1, file_->GetName(), "COMMAND", 0, 1);
}

void CommandModel::End()
{
	DeleteObserver();
}

void CommandModel::Error(bool type)
{
	buffer_command_.Clear();
	if(type)
		ChangeLine("Error: Invalid Command", -1,0, 0, 1);
	else
		ChangeLine("Error: File not saved (To exit without saving enter \"q!\")", -1,0, 0, 1);
	ChangeCursor(-1, -1, file_->GetName(), "COMMAND", 0, 1);
}

int CommandModel::HandlingSymbol(const char symbol_getch)
{
	amount_line_ = amount_line_check(base_buffer_, width_,true);
	if (symbol_getch == cBackSpace)
	{
		if (buffer_command_.Size() <= 1)
			return 1;
		buffer_command_.PopBack();
		ChangeLine(buffer_command_, -1,0, 0, 1);
		ChangeCursor(-1, -1, file_->GetName(), "COMMAND", 0, 1);
		return 0;
	}
	else if (symbol_getch != cEnter && symbol_getch != cEscape)
	{
		buffer_command_.PushBack(symbol_getch);
		ChangeLine(buffer_command_, -1,0, 0, 1);
		ChangeCursor(-1, -1, file_->GetName(), "COMMAND", 0, 1);
		return 0;
	}
	else
	{
		buffer_command_.Erase(0, 1);
		if (buffer_command_.Size() == 0)
			return 1;

		if (buffer_command_.At(0) >= '0' && buffer_command_.At(0) <= '9')
		{
			int number = Translate(buffer_command_);
			if (number < 0)
			{
				Error(true);
				return 3;
			}
			if (number > amount_line_)
			{
				y_cursor_ = amount_line_ - 1;
			}
			else
			{
				y_cursor_ = number - 1;
			}
			x_cursor_ - 0;
			return 1;		
		}

		switch (buffer_command_.At(0))
		{
		case 'o':
		{
			if (buffer_command_.At(1) != ' ')
			{
				Error(true);
				return 3;
			}
			else
			{
				if (save_)
				{
					SaveBuffer();
					save_ = false;
				}
				file_->NewPath(buffer_command_.Substr(2));
				return 2;
			}
			break;
		}
		case 'h':
		{
			if (buffer_command_.Size() > 1)
			{
				Error(true);
				return 3;
			}
			else
			{
				BasicString buf_help;
				buf_help = ":o filename - Open file filename\n:x - Write to current file and exit\n:w  - Write to current file\n:w filename - Write to filename\n:q - Go out. If the file has been changed, then the exit is possible only with q!\n:q! - Exit without saving\n:wq! - Write to current file and exit\n:number - Jump to line number\n:h - Display command help\n";

				int i = 0;
				int i_line = 0;
				BasicString line;
				while (i < buf_help.Size())
				{
					if (buf_help[i] == '\n')
					{
						ChangeLine(line, i_line, 0, 0, 1);
						i_line += 1;
						line.Clear();
					}
					else
					{
						line.PushBack(buf_help[i]);
					}
					i++;
				}
				line.Clear();
				for (int i = i_line; i < height_; i++)
					ChangeLine(line, i, 0, 0, 1);
				ChangeCursor(0, 0, file_->GetName(), "COMMAND", 0, 1);
				return 5;
			}
			break;
		}
		case 'x':
		{
			if (buffer_command_.Size() > 1)
			{
				Error(true);
				return 3;
			}
			if (save_)
			{
				SaveBuffer();
				save_ = false;
			}
				return 4;
			break;
		}

		case 'w':
		{
			if (buffer_command_.Size() > 1)
			{
				if (buffer_command_.At(1) != ' ')
				{
					if (strcmp(buffer_command_.CStr(), "wq!") == 0)
					{
						if (save_)
						{
							SaveBuffer();
							save_ = false;
						}
						return 4;
					}
					else
					{
						Error(true);
						return 3;
					}
				}
				else
				{
					BasicString tmp_path = file_->GetPath();
					file_->NewPath(buffer_command_.Substr(2));
					SaveBuffer();
					file_->NewPath(tmp_path);
					return 1;
				}
			}
			else
			{
				if (save_)
				{
					SaveBuffer();
					save_ = false;
				}
				return 1;
			}
			break;
		}
		case 'q':
		{
			if (buffer_command_.Size() > 1)
			{
				if (buffer_command_.Size() > 2)
				{
					Error(true);
					return 3;
				}
				else
				{
					if (buffer_command_.At(1) != '!')
					{
						Error(true);
						return 3;
					}
					else
					{
						return 4;
					}
				}
			}
			else
			{
				if (save_)
				{
					Error(false);
					return 3;
				}
				return 4;
			}
			break;
		}
		default:
			Error(true);
			return 3;
		}
	}
}

void  CommandModel::SaveBuffer()
{
	file_->WriteFromBuffer(base_buffer_->CStr(), base_buffer_->Size());
}


////////////////////////////////////////////////////
///////////////////////////////////////////////////////
////////////TTTTTTTTTTTTTTTTTTTTTTTT?/////////////////////
//PART 3@@@@@@@@@@@@@@@

void SearchModel::Start(const BasicString& path)
{
	AddObserver(screen_);
	file_ = new WorkWithFile(path);
	amount_line_ = file_->GetAmountLine(width_);
	DeleteObserver();
}

void SearchModel::Continuation(const IModel* latest_model, bool after_hex)
{
	first_line_screen_ = latest_model->GetFirstLineScreen();
	last_line_screen_ = latest_model->GetLastLineScreen();
	index_cursor_ = latest_model->GetIndexCursor();
	y_cursor_ = latest_model->GetYCursor();
	x_cursor_ = latest_model->GetXCursor();
	save_ = latest_model->GetSave();
	if (file_ == nullptr)
	{
		file_ = new WorkWithFile(latest_model->GetFilePath());
	}
	find_ = false;
	AddObserver(screen_);
	ChangeCursor(-1, -1, file_->GetName(), "SEARCH", 0, 1);
}

void SearchModel::End()
{
	buffer_search_.Clear();
	DeleteObserver();
}

int SearchModel::HandlingSymbol(const char symbol_getch)
{
	if (symbol_getch == cBackSpace)
	{
		if (buffer_search_.Size() <= 1)
			return 4;
		buffer_search_.PopBack();
		ChangeLine(buffer_search_, -1,0, 0, 1);
		ChangeCursor(-1, -1, file_->GetName(), "SEARCH", 0, 1);
		return 1;
	}
	if (buffer_search_.Size() == 0)
	{
		if (symbol_getch == '/')
		{
			type_ = true;
		}
		else if (symbol_getch == '?')
		{
			type_ = false;
		}
		else
		{
			return -1;
		}
		buffer_search_.PushBack(symbol_getch);
		ChangeLine(buffer_search_, -1,0, 0, 1);
		ChangeCursor(-1, -1, file_->GetName(), "SEARCH", 0, 1);
	}
	else
	{
		if (find_)
		{
			if (symbol_getch == 'n')
				type_ = true;
			if (symbol_getch == 'N')
				type_ = false;
		}
		if ((symbol_getch != cEnter && symbol_getch != cEscape) && (!find_))
		{
			buffer_search_.PushBack(symbol_getch);
			ChangeLine(buffer_search_, -1,0, 0, 1);
			ChangeCursor(-1, -1, file_->GetName(), "SEARCH", 0, 1);
			return 1;
		}
		else
		{
			if (!find_)
				buffer_search_.Erase(0, 1);
			if (buffer_search_.Size() == 0)
			{
				ChangeLine("Error: Invalid Command", -1,0, 0, 1);
				ChangeCursor(-1, -1, file_->GetName(), "SEARCH", 0, 1);
				return 3;
			}
			int index = 0;
			if (type_)
			{
				if (index_cursor_ + 1 == base_buffer_->Size())
				{
					return 3;
				}
				else
				{
					index_cursor_++;
				}
				 index = SearchToEnd(buffer_search_);
			}
			else
			{
				if (index_cursor_ - 1 < 0)
				{
					return 3;
				}
				else
				{
					index_cursor_--;
				}
				index = SearchToBeg(buffer_search_);
			}
			if (index == -1)
			{
				ChangeLine("Not found", -1,0, 0, 1);
				ChangeCursor(-1, -1, file_->GetName(), "SEARCH", 0, 1);
				find_ = true;
				return 3;
			}
			index_cursor_ = index;
			FindYX(y_cursor_, x_cursor_);
			ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "SEARCH", 0, 1);
			find_ = true;
			return 4;
		}
	}
}

int SearchModel::SearchToEnd(const BasicString& word)
{
	return base_buffer_->Find(word.CStr(), index_cursor_);
}

int SearchModel::SearchToBeg(const BasicString& word)
{
	BasicString reBuffer;
	int index = index_cursor_;
	while (index >= 0)
		reBuffer.PushBack(base_buffer_->At(index--));
	BasicString reWord;
	index = word.Size() - 1;
	while (index >= 0)
		reWord.PushBack(word[index--]);
	index = reBuffer.Find(reWord.CStr());
	if (index == -1)
		return -1;
	return(index_cursor_ - index - (reWord.Size() - 1));
}


////////////////////////////////////////////////////
///////////////////////////////////////////////////////
////////////TTTTTTTTTTTTTTTTTTTTTTTT?/////////////////////
//PART 4@@@@@@@@@@@@@@@

void EditModel::Start(const BasicString& path)
{
	AddObserver(screen_);
	file_ = new WorkWithFile(path);
	amount_line_ = file_->GetAmountLine(width_);
	ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
	DeleteObserver();
}

void EditModel::Continuation(const IModel* latest_model, bool after_hex)
{
	first_line_screen_ = latest_model->GetFirstLineScreen();
	last_line_screen_ = latest_model->GetLastLineScreen();
	index_cursor_ = latest_model->GetIndexCursor();
	y_cursor_ = latest_model->GetYCursor();
	x_cursor_ = latest_model->GetXCursor();
	save_ = latest_model->GetSave();
	if (file_ == nullptr)
	{
		file_ = new WorkWithFile(latest_model->GetFilePath());
	}
	else if (!(strcmp((file_->GetPath()).CStr(), (latest_model->GetFilePath()).CStr()) == 0))
	{
		delete file_;
		BasicString newPath = latest_model->GetFilePath().CStr();
	}
	AddObserver(screen_);
	ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
}

void EditModel::End()
{
	if (index_cursor_ != 0)
	{
		index_cursor_--;
		FindYX(y_cursor_, x_cursor_);
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true, 0, 1);
		if (check < 0)
			ChangeCross(false, 0, 1);
		ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
	}
	DeleteObserver();
}

int EditModel::HandlingSymbol(char symbol_getch)
{
	amount_line_ = amount_line_check(base_buffer_, width_,true);
	if (symbol_getch == cBackSpace)
	{
		if (index_cursor_ == 0)
		{
			return 1;
		}
		else
		{
			if (x_cursor_ == 0)
			{
				if (base_buffer_->At(index_cursor_ - 1) == '\n')
					x_cursor_ = WidthLine(y_cursor_ - 1);
				else
				{
					x_cursor_ = WidthLine(y_cursor_ - 1) - 1;
				}
				--y_cursor_;
			}
			else
			{
				--x_cursor_;
			}

			base_buffer_->Erase(index_cursor_ - 1, 1);
			index_cursor_--;
			FindYX(y_cursor_, x_cursor_);
			UpdateLines(y_cursor_);
			int check = Screen—rossing—heck();
			if (check > 0)
				ChangeCross(true, 0, 1);
			if (check < 0)
				ChangeCross(false, 0, 1);
			ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
			return 1;
		}
	}
	else if (symbol_getch == cRight)
	{
		MoveRight();
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true, 0, 1);
		if (check < 0)
			ChangeCross(false, 0, 1);
		ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
		return 1;
	}
	else if (symbol_getch == cLeft)
	{
		MoveLeft();
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true, 0, 1);
		if (check < 0)
			ChangeCross(false, 0, 1);
		ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
		return 1;
	}
	else if (symbol_getch == cUp || symbol_getch == cDown)
	{
		if (symbol_getch == cUp)
		{
			MoveUp();
		}
		else
		{
			MoveDown();
		}
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true, 0, 1);
		if (check < 0)
			ChangeCross(false, 0, 1);
		ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
		return 1;
	}
	else
	{
		base_buffer_->Insert(index_cursor_++, 1, symbol_getch);
		FindYX(y_cursor_, x_cursor_);
		UpdateLines(y_cursor_);
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true, 0, 1);
		if (check < 0)
			ChangeCross(false, 0, 1);
		ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "EDIT", 0, 1);
		return 1;

	}
	return 1;
}

void EditModel::UpdateLines(int edit_line)
{
	int amount_line = 0;
	int position_start_line = 0;

	int index_width = 1;

	while (amount_line < (edit_line - 1) && position_start_line < base_buffer_->Size())
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
	while (position_end_line < base_buffer_->Size())
	{
		if (base_buffer_->At(position_end_line) == '\n')
		{
			if (position_start_line != position_end_line)
			{
				ChangeLine(base_buffer_->Substr(position_start_line, position_end_line - position_start_line), amount_line,0, 0, 1);
			}
			else
			{
				BasicString space(" ");
				ChangeLine(space, amount_line,0, 0, 1);
			}
			amount_line++;
			index_width = 1;
			position_end_line++;
			position_start_line = position_end_line;
		}
		else if (index_width > width_)
		{
			if (position_start_line != position_end_line)
			{
				ChangeLine(base_buffer_->Substr(position_start_line, position_end_line - position_start_line), amount_line,0, 0, 1);
			}
			amount_line++;
			index_width = 1;
			position_start_line = position_end_line;
		}
		else
		{
			index_width++;
			position_end_line++;
		}
	}
	if (position_start_line != position_end_line)
	{
		ChangeLine(base_buffer_->Substr(position_start_line, position_end_line - position_start_line), amount_line,0, 0, 1);
		amount_line++;
	}
	if (amount_line < amount_line_)
	{
		BasicString empty;
		for (int i = amount_line; i < amount_line_; i++)
			ChangeLine(empty, i, 0, 0, 1);
		amount_line_ = amount_line + 1;
	}
	if (base_buffer_->Size() == 0)
	{
		BasicString empty;
		ChangeLine(empty, 0, 0, 0, 1);
		y_cursor_ = 0;
		x_cursor_ = 0;
		index_cursor_ = 0;
	}
}


////////////////////////////////////////////////////
///////////////////////////////////////////////////////
////////////TTTTTTTTTTTTTTTTTTTTTTTT?/////////////////////
//PART 4@@@@@@@@@@@@@@@


void HexModel::Start(const BasicString& path)
{
	AddObserver(screen_);
	file_ = new WorkWithFile(path);
	amount_line_ = file_->GetAmountLine(width_);
	BasicString line;
	int length = 0;
	for (int i = 0; i < amount_line_; i++)
	{
		line = file_->GetLine(i, width_);
		ChangeLine(line, i,0, 0, 1);
		base_buffer_->Insert(length, line.CStr());
		length = base_buffer_->Size();
		line.Clear();
	}
	ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "HEX", 0, 1);
	DeleteObserver();
}

void HexModel::Continuation(const IModel* latest_model, bool after_hex)
{
	width_old_ = latest_model->GetWidth();
	height_old_ = latest_model->GetHeight();
	first_line_screen_ = 0;
	last_line_screen_ = height_-1;
	index_cursor_ = latest_model->GetIndexCursor();
	AddObserver(screen_);
	TranslateBuffer();
	TranslateLineScreenToHex();
	FindYX(y_cursor_, x_cursor_);
	save_ = latest_model->GetSave();
	if (file_ == nullptr)
	{
		file_ = new WorkWithFile(latest_model->GetFilePath());
	}
	else if (!(strcmp((file_->GetPath()).CStr(), (latest_model->GetFilePath()).CStr()) == 0))
	{
		delete file_;
		BasicString newPath = latest_model->GetFilePath().CStr();
		file_ = new WorkWithFile(newPath);
	}
	FindYX(y_cursor_, x_cursor_);
	amount_line_ = amount_line_check(&hex_base_buffer_, width_,false);
	BasicString line;
	int length = 0;
	for (int i = 0; i < 150; i++)
	{
		if (i < amount_line_)
		{	
			if ((length + width_) < hex_base_buffer_.Size())
				line = hex_base_buffer_.Substr(length, width_);
			else
				line = hex_base_buffer_.Substr(length);
			if (index_cursor_ >= length && index_cursor_ < (length + width_))
				ChangeLine(line, i, index_cursor_, 0, 1);
			else
				ChangeLine(line, i, -1, 0, 1);
			length += width_;
			line.Clear();
		}
		else
		{
			line.Clear();
			ChangeLine(line, i, -1, 0, 1);
		}
	}
	FindYX(y_cursor_, x_cursor_);
	ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "HEX", 0, 1);
}

void HexModel::End()
{
	ChangeType(TypeUpdate::cNorm);
	int index = index_cursor_;
	while (index >= 0)
	{
		if (hex_base_buffer_.At(index) == (char)(0x0d))
		{
			index_cursor_--;
		}
		index--;
	}
	DeleteObserver();
}

int HexModel::HandlingSymbol(const char symbol_getch)
{
	if (!type)
	{
		int check = Search(symbol_getch);
		if (check == 0)
			return 0;
		else if (check == 1)
		{
			type = true;
		}
	}
	amount_line_ = amount_line_check(&hex_base_buffer_, width_,false);
	buffer_command_.Append(1, symbol_getch);
	if (symbol_getch == cEscape)
	{
		buffer_command_.Clear();
		return 0;
	}
	else if (buffer_command_.At(0) >= '0' && buffer_command_.At(0) <= '9')
	{
		if (!(buffer_command_.At(buffer_command_.Size() - 1) >= '0' && buffer_command_.At(buffer_command_.Size() - 1) <= '9'))
		{
			if (buffer_command_.At(buffer_command_.Size() - 1) == 'G')
			{
				int index = Translate(buffer_command_.Substr(0, buffer_command_.Size() - 1)) - 1;
				if (index >= hex_base_buffer_.Size())
				{
					if (hex_base_buffer_.Size() == 0)
					{
						index = 0;
					}
					else
					{
						index = hex_base_buffer_.Size() - 1;
					}
				}

				int now_index_cursor = index_cursor_;
				GoToTheBeginningLine();
				int start = index_cursor_;
				GoToTheEndLine();
				int finish = index_cursor_;
				index_cursor_ = now_index_cursor;
				FindYX(y_cursor_, x_cursor_);
				ChangeLine(hex_base_buffer_.Substr(start, finish - start + 1), y_cursor_, -1, 0, 1);


				if (index_cursor_< index)
				{
					while (index_cursor_ < index)
					{
						MoveRight();
						int check = Screen—rossing—heck();
						if (check > 0)
							ChangeCross(true, 0, 1);
						if (check < 0)
							ChangeCross(false, 0, 1);
					}
				}
				else if (index_cursor_ > index)
				{
					while (index_cursor_ > index)
					{
						MoveLeft();
						int check = Screen—rossing—heck();
						if (check > 0)
							ChangeCross(true, 0, 1);
						if (check < 0)
							ChangeCross(false, 0, 1);
					}
				}
				now_index_cursor = index_cursor_;
				GoToTheBeginningLine();
				start = index_cursor_;
				GoToTheEndLine();
				finish = index_cursor_;
				index_cursor_ = now_index_cursor;
				FindYX(y_cursor_, x_cursor_);
				ChangeLine(hex_base_buffer_.Substr(start, finish - start + 1), y_cursor_, index_cursor_, 0, 1);
				ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "HEX", 0, 1);
				buffer_command_.Clear();
			}
			else
			{
				buffer_command_.Clear();
				int now_index_cursor = index_cursor_;
				GoToTheBeginningLine();
				int start = index_cursor_;
				GoToTheEndLine();
				int finish = index_cursor_;
				index_cursor_ = now_index_cursor;
				FindYX(y_cursor_, x_cursor_);
				ChangeLine(hex_base_buffer_.Substr(start, finish - start + 1), y_cursor_, -1, 0, 1);
				switch (symbol_getch)
				{
				case (cRight):
				{
					MoveRight();
					break;
				}
				case (cLeft):
				{
					MoveLeft();
					break;
				}
				case (cUp):
				{
					MoveUp();
					break;
				}
				case (cDown):
				{
					MoveDown();
					break;
				}
				default:
					break;
				}
				int check = Screen—rossing—heck();
				if (check > 0)
					ChangeCross(true, 0, 1);
				if (check < 0)
					ChangeCross(false, 0, 1);
				now_index_cursor = index_cursor_;
				GoToTheBeginningLine();
				start = index_cursor_;
				GoToTheEndLine();
				finish = index_cursor_;
				index_cursor_ = now_index_cursor;
				FindYX(y_cursor_, x_cursor_);
				ChangeLine(hex_base_buffer_.Substr(start, finish - start + 1), y_cursor_, index_cursor_, 0, 1);
				ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "HEX", 0, 1);
			}
		}
	}
	else if (symbol_getch == '/')
	{
		buffer_command_.Clear();
		Search(symbol_getch);
		type = false;
	}
	else 
	{
		buffer_command_.Clear();
		int now_index_cursor = index_cursor_;
		GoToTheBeginningLine();
		int start = index_cursor_;
		GoToTheEndLine();
		int finish = index_cursor_;
		index_cursor_ = now_index_cursor;
		FindYX(y_cursor_, x_cursor_);
		ChangeLine(hex_base_buffer_.Substr(start, finish - start + 1), y_cursor_, -1, 0, 1);
		switch (symbol_getch)
		{
		case (cRight):
		{
			MoveRight();
			break;
		}
		case (cLeft):
		{
			MoveLeft();
			break;
		}
		case (cUp):
		{
			MoveUp();
			break;
		}
		case (cDown):
		{
			MoveDown();
			break;
		}
		default:
			break;
		}
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true, 0, 1);
		if (check < 0)
			ChangeCross(false, 0, 1);
		now_index_cursor = index_cursor_;
		GoToTheBeginningLine();
		start = index_cursor_;
		GoToTheEndLine();
		finish = index_cursor_;
		index_cursor_ = now_index_cursor;
		FindYX(y_cursor_, x_cursor_);
		ChangeLine(hex_base_buffer_.Substr(start, finish - start + 1), y_cursor_, index_cursor_, 0, 1);
		ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "HEX", 0, 1);
	}
	return 0;
}

void HexModel::GoToTheBeginningLine()
{
	while (x_cursor_ > 0)
	{
		--x_cursor_;
		--index_cursor_;
	}
}

void HexModel::GoToTheEndLine()
{
	while (index_cursor_ < hex_base_buffer_.Size() && x_cursor_ < width_)
	{
		++x_cursor_;
		++index_cursor_;
	}
	if (x_cursor_ != 0)
	{
		--x_cursor_;
		--index_cursor_;
	}
}

void HexModel::TranslateLineScreenToHex()
{
	int index = index_cursor_;
	index_cursor_ = 0;
	while (index > index_cursor_)
	{
		MoveRight();
		int check = Screen—rossing—heck();
		if (check > 0)
			ChangeCross(true,0,1);
		if (check < 0)
			ChangeCross(false, 0, 1);
	}
}


void HexModel::TranslateBuffer()
{
	for (int i = 0; i < base_buffer_->Size(); i++)
	{
		if (base_buffer_->At(i) == '\n')
		{
			hex_base_buffer_.Append(1, (char)(0x0d));
			hex_base_buffer_.Append(1, (char)(0x0a));
			if (i <= index_cursor_)
			{
				index_cursor_++;
			}
		}
		//else if (base_buffer_->At(i) == '\t')
		//{
		//	hex_base_buffer_.Append(1, ' ');
		//	hex_base_buffer_.Append(1, (char)(0x09));
		//}
		else
		{
			hex_base_buffer_.Append(1, base_buffer_->At(i));
		}
	}
}

bool HexModel::FindYX(int& y, int& x)
{
	y = 0;
	x = 1;
	int position_start_line = 0;
	while (position_start_line < index_cursor_ && position_start_line < hex_base_buffer_.Size())
	{
		if (x == width_)
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

void HexModel::MoveRight()
{
	if (index_cursor_ + 1 >= hex_base_buffer_.Size())
		return;
	index_cursor_++;
	FindYX(y_cursor_, x_cursor_);
	return;
}

void HexModel::MoveLeft()
{
	if (index_cursor_ - 1 < 0)
		return;
	index_cursor_--;
	FindYX(y_cursor_, x_cursor_);
	return;
}

int HexModel::WidthLine(int line)
{
	int amount_line = 0;
	int position_start_line = 0;
	int index_width = 1;

	while (amount_line < line && position_start_line < hex_base_buffer_.Size())
	{
		if (index_width == width_)
		{
			amount_line++;
			index_width = 0;
		}
		index_width++;
		position_start_line++;
	}
	index_width = 1;
	int position_end_line = position_start_line;
	while (position_end_line < hex_base_buffer_.Size() && index_width <= width_)
	{
		index_width++;
		position_end_line++;
	}
	return position_end_line - position_start_line;
}

void HexModel::MoveUp()
{
	if (y_cursor_ - 1 < 0)
		return;
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

bool HexModel::FindIndexCursor(int& index_cursor, int y, int x)
{
	index_cursor = 0;

	int amount_line = 0;
	int index_width = 1;

	while (amount_line < y && index_cursor < hex_base_buffer_.Size())
	{
		if (index_width == width_)
		{
			amount_line++;
			index_width = 0;
		}
		index_width++;
		index_cursor++;
	}
	index_width = 1;
	while (index_cursor < hex_base_buffer_.Size())
	{
		if (index_width > width_ || index_width - 1 == x)
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

void HexModel::MoveDown()
{
	if (y_cursor_ + 1 >= amount_line_)
		return;
	y_cursor_++;
	int size_line = WidthLine(y_cursor_) - 1;
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

int HexModel::Screen—rossing—heck()
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

int TranslateToNOneSymbol(char symbol)
{
	if (symbol >= '0' && symbol < '9')
	{
		return ((int)(symbol)- 48);
	}
	switch (symbol)
	{
	case 'A': return 10;
		break;
	case 'B': return 11;
		break;
	case 'C': return 12;
		break;
	case 'D': return 13;
		break;
	case 'E': return 14;
		break;
	case 'F': return 15;
		break;

	}
}

int HexModel::Search(char symbol)
{
	if (symbol == cEnter)
	{
		BasicString word;
		buffer_command_.Erase(0, 1);
		int index = 0;
		while (index < buffer_command_.Size())
		{
			int number = (TranslateToNOneSymbol(buffer_command_[index]) * 16) + (TranslateToNOneSymbol(buffer_command_[index + 1]));
			index += 2;



			word.Append(1, (char)(number));
		}
		index = SearchToEnd(word);
		if (index < 0)
		{
			buffer_command_.Clear();
			return index;
		}
		int now_index_cursor = index_cursor_;
		GoToTheBeginningLine();
		int start = index_cursor_;
		GoToTheEndLine();
		int finish = index_cursor_;
		index_cursor_ = now_index_cursor;
		FindYX(y_cursor_, x_cursor_);
		ChangeLine(hex_base_buffer_.Substr(start, finish - start + 1), y_cursor_, -1, 0, 1);


		if (index_cursor_ < index)
		{
			while (index_cursor_ < index)
			{
				MoveRight();
				int check = Screen—rossing—heck();
				if (check > 0)
					ChangeCross(true, 0, 1);
				if (check < 0)
					ChangeCross(false, 0, 1);
			}
		}
		else if (index_cursor_ > index)
		{
			while (index_cursor_ > index)
			{
				MoveLeft();
				int check = Screen—rossing—heck();
				if (check > 0)
					ChangeCross(true, 0, 1);
				if (check < 0)
					ChangeCross(false, 0, 1);
			}
		}
		now_index_cursor = index_cursor_;
		GoToTheBeginningLine();
		start = index_cursor_;
		GoToTheEndLine();
		finish = index_cursor_;
		index_cursor_ = now_index_cursor;
		FindYX(y_cursor_, x_cursor_);
		ChangeLine(hex_base_buffer_.Substr(start, finish - start + 1), y_cursor_, index_cursor_, 0, 1);
		ChangeCursor(y_cursor_, x_cursor_, file_->GetName(), "HEX", 0, 1);
		buffer_command_.Clear();
		return 1;
	}
	else
	{
		buffer_command_.Append(1,symbol);
		ChangeLine(buffer_command_, -1, 0, 0, 1);
		ChangeCursor(-1, -1, file_->GetName(), "HEX", 0, 1);
		return 0;
	}
}

int HexModel::SearchToEnd(const BasicString& word)
{
	return hex_base_buffer_.Find(word.CStr(), index_cursor_);
}
