#pragma once
#ifndef _I_ADAPTER_
#define _I_ADAPTER_

#include "MyString/mystring.h"


class IAdapterPDCur
{
public:
    virtual void Print(int id_screen, int y, int x, char symbol, int color_pair) const = 0;
    virtual void Print(int id_screen, int y, int x, const char* string, int color_pair) const = 0;
    virtual void Print(int id_screen, int y, int x, const BasicString string, int color_pair) const = 0;
    virtual void Print(int id_screen, int y, int x, int number, int color_pair) const = 0;
    virtual void PrintX(int id_screen, int y, int x, int number, int color_pair) const = 0;

    virtual void SetCursor(int y, int x) = 0;

    virtual int GetMaxHeightMainWin() const = 0;
    virtual int GetMaxWidthMainWin() const = 0;

    virtual void CreateColorPair(int id_pair, int text_color, int background_color) const = 0;
    virtual int CrateWin(int y, int x, int height, int width) = 0;
    virtual int CratePad(int y, int x, int height, int width) = 0;

    virtual bool Refresh() = 0;
    virtual bool RefreshWin(const int id_win) = 0;
    virtual bool RefreshPad(int id_pad, int py, int px, int s1y, int s1x, int s2y, int s2x) = 0;
    
    virtual void ClearLine(int id_screen, int y, int x, int width, int color_pair)= 0;
    virtual void ClearAll() = 0;

    virtual int GetchSymbol() = 0;

    virtual void ViewCursor(int y, int x, char symbol, int color_pair, int id_screen) = 0;
    virtual int MvScan(int y, int x, int id_screen) = 0;


};


#endif // _I_ADAPTER_