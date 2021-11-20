#pragma once

#ifndef _WORKWITHFILE_H_
#define _WORKWITHFILE_H_

#include "MyString/mystring.h"


class WorkWithFile
{
public:
	WorkWithFile(BasicString path_to_file);

	BasicString GetName();
	BasicString GetPath();
	int GetAmountLine(int width);
	BasicString GetLine(int number_line, int width);
	
	void WriteFromBuffer(const char * str, int length);
	
	void DeleteFile(BasicString path_to_file);
	void NewPath(BasicString path_to_file);
	bool Open();


	~WorkWithFile() {};

private:
	BasicString name_file_;
	BasicString path_to_file_;

};

#endif // _WORKWITHFILE_H_