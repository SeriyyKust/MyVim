#include "controller.h"

void Controller::StartApplication(BasicString path)
{
	models_[index_window_]->Start(path);
	int old_index_window = index_window_;
	char symbol_getch = 't';
	int check = 0;
	while (true)
	{
		models_[index_window_]->Continuation(models_[old_index_window],false);
		if (check != 3)
			symbol_getch = adapter_->GetchSymbol();
		while (!CheckSymbols(symbol_getch))
		{
			check = models_[index_window_]->HandlingSymbol(symbol_getch);
			if (type_ == TypeController::cCommand )
			{
				if (check == 4 || check == 1 || check == 2)
					break;
			}
			else if (type_ == TypeController::cReadEdit)
			{
				if (check == 2 || check == 5)
					break;
			}
			symbol_getch = adapter_->GetchSymbol();
			if (type_ == TypeController::cCommand)
			{
				if (check == 3)
					break;
				if (check == 5)
				{
					models_[index_window_]->End();
					old_index_window = index_window_;
					ChangeMode(TypeController::cReadEdit);
					models_[index_window_]->Start(path);
					models_[index_window_]->Continuation(models_[old_index_window], false);
				}
			}
			else if (type_ == TypeController::cSearch)
			{
				if (check == 3 || check == 4)
				{
					if (symbol_getch != 'n' && symbol_getch != 'N')
					{
						break;
					}
				}
					
			}
		}
		models_[index_window_]->End();
		if (type_ == TypeController::cCommand)
		{
			if (check == 4)
				break;
		}
		old_index_window = index_window_;
		if (type_ == TypeController::cHex)
		{
			ChangeMode(TypeController::cReadEdit);
			models_[index_window_]->Continuation(models_[old_index_window], true);
			old_index_window = index_window_;
		}
		else if (type_ == TypeController::cReadEdit)
		{
			if (check == 2)
			{
				ChangeMode(TypeController::cEdit);
			}
			else if (symbol_getch == ':')
			{
				ChangeMode(TypeController::cCommand);
				check = 0;
			}
			else if (symbol_getch == '/' || symbol_getch == '?')
			{
				ChangeMode(TypeController::cSearch);
				check = 3;
			} 
			else if (check == 5)
			{
				ChangeMode(TypeController::cHex);
			}
		}
		else 
		{
			ChangeMode(TypeController::cReadEdit);
		}
	}
}

void Controller::ChangeMode(TypeController type)
{
	type_ = type;
	index_window_ = (int)(type);
}

bool Controller::CheckSymbols(char symbol)
{
	if (index_window_ == 0)
	{
		if (symbol == ':' || symbol == '?' || symbol == '/')
			return true;
		else return false;
	}
	else
	{
		if (symbol == (char)(27))
			return true;
		return false;
	}
}