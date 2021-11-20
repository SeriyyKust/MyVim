#pragma once
#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include "MyString/mystring.h"

enum class TypeUpdate
{
	cNorm = 0,
	cHex = 1
};

class Observer
{
public:
	virtual void UpdateCursor(int y, int x, BasicString name, BasicString type) = 0;
	virtual void UpdateLine(BasicString string, int line, int hex_cursor) = 0;
	virtual void UpdateCross(bool step) = 0;
	virtual void UpdateType(TypeUpdate type) = 0;

};
#endif // _OBSERVER_H_
