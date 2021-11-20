#pragma once
#ifndef _MODEL_H_
#define _MODEL_H_

#include "imodel.h"
#include "observable.h"
//#include "View.h"

#include "workwithfile.h"

const int cEnter = 0x0A;
const int cBackSpace = 0x08;
const int cEscape = 0x1B;

const int cUp = 0x03;
const int cDown = 0x02;
const int cLeft = 0x04;
const int cRight = 0x05;

/*
class ReadModel : public IModel, public Observable
{
public:
	ReadModel(int height, int width, View* screen, BasicString * buffer) : IModel(height, width,  buffer){ screen_ = screen; };
	~ReadModel() {};

	virtual void Start(BasicString path);
	virtual void Continuation(BasicString path, int index_cursos, int first_line, int last_line);
	virtual void End(BasicString path, int& first_line, int& last_line);
	virtual void Cross();
	virtual int HandlingSymbol(char symbol_getch, int index_cursor);

	bool FindWordAhead();
	bool FindWordBehind();

	void GoToTheEnd();
	void GoToTheBeginning();

private:
	WorkWithFile* file_;
	View* screen_;
};

class EditModel : public IModel, public Observable
{
public:
	EditModel(int height, int width, View* screen, BasicString* buffer) : IModel(height, width,  buffer) { screen_ = screen; };
	~EditModel() {};


	virtual int HandlingSymbol(char symbol_getch, int index_cursor);
	virtual void Cross();
	virtual void Start(BasicString path);
	virtual void Continuation(BasicString path, int index_cursos, int first_line, int last_line);

	

	virtual void End(BasicString path, int& first_line, int& last_line);
private:
	WorkWithFile* file_;
	View* screen_;
	

};



*/

class NavigationEdit : public IModel
{
public:
	NavigationEdit(int height, int width, BasicString* buffer, View* screen) : IModel(height, width, buffer) 
	{ 
		screen_ = screen;
	};

	virtual void Start(const BasicString & path) override;
	virtual void Continuation(const IModel* latest_model, bool after_hex) override;
	virtual void End()override;
	virtual int HandlingSymbol(const char symbol_getch) override;

	BasicString GetLine(int number_line, int width);
	
	void GoToTheBeginning();
	void GoToTheEnd();
	void GoToTheBeginningLine();
	void GoToTheEndLine();
	void GoToWordEnd();
	void GoToWordFront();

	void DeleteSymbols(int position, int count);
	void UpdateLines(int edit_line);
	void CopyLineToBuffer();
	void CopyWordToBuffer();
	void InsertFromBufferToLine();

	bool CheckBufferCommand();

	~NavigationEdit() {};
private:
	View* screen_;
	BasicString buffer_command_;
	BasicString buffer_line_;
	
};

class CommandModel : public IModel
{
public:
	CommandModel(int height, int width, BasicString* buffer, View* screen) : IModel(height, width, buffer) { screen_ = screen; };
	virtual void Start(const BasicString& path) override;
	virtual void Continuation(const IModel* latest_model, bool after_hex) override;
	virtual void End()override;
	virtual int HandlingSymbol(const char symbol_getch) override;
	void Error(bool type);
	void SaveBuffer();
	~CommandModel() {};
private:
	View* screen_;
	BasicString buffer_command_;
};

class SearchModel : public IModel
{
	enum class SearchDirection
	{
		cToEnd = 0,
		cToBeg = 1
	};
public:
	SearchModel(int height, int width, BasicString* buffer, View* screen) : IModel(height, width, buffer) { screen_ = screen; type_ = true; find_ = false; };
	virtual void Start(const BasicString& path) override;
	virtual void Continuation(const IModel* latest_model, bool after_hex) override;
	virtual void End()override;
	virtual int HandlingSymbol(const char symbol_getch) override;
	int SearchToEnd(const BasicString& word);
	int SearchToBeg(const BasicString& word);
	~SearchModel() {};
private:
	bool type_;
	bool find_;
	View* screen_;
	BasicString buffer_search_;
};


class EditModel : public IModel
{
public:
	EditModel(int height, int width, BasicString* buffer, View* screen) : IModel(height, width, buffer) { screen_ = screen; };
	virtual void Start(const BasicString& path) override;
	virtual void Continuation(const IModel* latest_model, bool after_hex) override;
	virtual void End()override;
	void UpdateLines(int edit_line);
	virtual int HandlingSymbol(const char symbol_getch) override;
	~EditModel() {};
private:
	View* screen_;
	BasicString buffer_line_;
};

class HexModel : public IModel
{
public:
	HexModel(int height, int width, BasicString* buffer, View* screen) : IModel(height, width, buffer) { screen_ = screen; type = true; };
	virtual void Start(const BasicString& path) override;
	virtual void Continuation(const IModel* latest_model, bool after_hex) override;
	virtual void End()override;
	void GoToTheBeginningLine();
	void GoToTheEndLine();
	virtual int HandlingSymbol(const char symbol_getch) override;
	void TranslateLineScreenToHex();
	void TranslateBuffer();
	bool FindYX(int& y, int& x);

	int WidthLine(int line);
	bool FindIndexCursor(int& index_cursor, int y, int x);

	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();
	int Screen—rossing—heck();

	int Search(char symbol);
	int SearchToEnd(const BasicString& word);


	~HexModel() {};
private:
	View* screen_;
	BasicString hex_base_buffer_;
	BasicString buffer_command_;
	BasicString buffer_search_;
	int width_old_;
	int height_old_;
	bool type;
};

#endif // _MODEL_H_
