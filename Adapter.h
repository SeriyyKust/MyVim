#pragma once
#ifndef _ADAPTER_H_
#define _ADAPTER_H_


#include <curses.h>
#include "iadapter.h"
#include <vector>



class AdapterPDCur : public IAdapterPDCur
{
public:
	AdapterPDCur();
	~AdapterPDCur();
	void Print(int id_screen, int y, int x, char symbol, int color_pair) const override;
	virtual void Print(int id_screen, int y, int x, const char* string, int color_pair) const override;
	virtual void Print(int id_screen, int y, int x, const BasicString string, int color_pair) const override;
	virtual void Print(int id_screen, int y, int x, int number, int color_pair) const override;
	virtual void PrintX(int id_screen, int y, int x, int number, int color_pair) const override;

	virtual void SetCursor(int y, int x) override;

	virtual int GetMaxHeightMainWin() const override;
	virtual int GetMaxWidthMainWin() const override;

	virtual void CreateColorPair(int id_pair, int text_color, int background_color) const override;
	virtual int CrateWin(int y, int x, int height, int width) override;
	virtual int CratePad(int y, int x, int height, int width) override;


	virtual bool Refresh()override;
	virtual bool RefreshWin(const int id_win) override;
	virtual bool RefreshPad(int id_pad, int py, int px, int s1y, int s1x, int s2y, int s2x) override;

	virtual void ClearLine(int id_screen, int y, int x, int width, int color_pair)override;
	virtual void ClearAll()override;

	virtual int GetchSymbol() override;

	virtual void ViewCursor(int y, int x, char symbol, int color_pair, int id_screen)  override;
	virtual int MvScan(int y, int x, int id_screen) override;

private:
	std::vector<WINDOW*> windows_;
};
#endif
