#include "Adapter.h"

AdapterPDCur::AdapterPDCur() 
{
    initscr();
    noecho();
    keypad(stdscr, true);
    refresh();
    start_color();
}

AdapterPDCur::~AdapterPDCur() 
{
    for (auto& window : windows_) 
    {
        delwin(window);
    }
    endwin();
}


void AdapterPDCur::SetCursor(int y, int x) 
{
    move(y, x);
}

void AdapterPDCur::Print(int id_screen, int y, int x, const char* str, int color_pair) const
{
    wattron(windows_[id_screen],COLOR_PAIR(color_pair));
    mvwprintw(windows_[id_screen],y, x, "%s", str);
    wattroff(windows_[id_screen], COLOR_PAIR(color_pair));
}

void AdapterPDCur::Print(int id_screen, int y, int x, int number, int color_pair) const
{
    wattron(windows_[id_screen], COLOR_PAIR(color_pair));
    mvwprintw(windows_[id_screen], y, x, "%d", number);
    wattroff(windows_[id_screen], COLOR_PAIR(color_pair));
}

void AdapterPDCur::PrintX(int id_screen, int y, int x, int number, int color_pair) const
{
    if (number < 16)
    {
        wattron(windows_[id_screen], COLOR_PAIR(color_pair));
        mvwprintw(windows_[id_screen], y, x++, "%d", 0);
        wattroff(windows_[id_screen], COLOR_PAIR(color_pair));
    }
    wattron(windows_[id_screen], COLOR_PAIR(color_pair));
    mvwprintw(windows_[id_screen], y, x, "%x", number);
    wattroff(windows_[id_screen], COLOR_PAIR(color_pair));
}

void AdapterPDCur::Print(int id_screen, int y, int x, char c, int color_pair) const
{
    wattron(windows_[id_screen], COLOR_PAIR(color_pair));
    mvwprintw(windows_[id_screen], y, x, "%c", c);
    wattroff(windows_[id_screen],COLOR_PAIR(color_pair));
}

void AdapterPDCur::Print(int id_screen, int y, int x, const BasicString string, int color_pair) const
{
    if (string.Size() != 0)
    {
        wattron(windows_[id_screen],COLOR_PAIR(color_pair));
        mvwprintw(windows_[id_screen], y, x, "%s", string.CStr());
        wattroff(windows_[id_screen],COLOR_PAIR(color_pair));
    }
}

int AdapterPDCur::GetMaxHeightMainWin() const
{
    int tmp_height, tmp_width = 0;
    getmaxyx(stdscr, tmp_height, tmp_width);
    return tmp_height;
}
int AdapterPDCur::GetMaxWidthMainWin() const
{
    int tmp_height, tmp_width = 0;
    getmaxyx(stdscr, tmp_height, tmp_width);
    return tmp_width;
}

void AdapterPDCur::CreateColorPair(int id_pair, int text_color, int background_color) const
{
    init_pair(id_pair, text_color, background_color);
}

int AdapterPDCur::CrateWin(int y, int x, int height, int width)
{
    WINDOW* new_window = newwin(height, width, y, x);
    wrefresh(new_window);
    refresh();

    windows_.push_back(new_window);
    return windows_.size() - 1;
}

int AdapterPDCur::CratePad(int y, int x, int height, int width)
{
    WINDOW* new_window = newpad(height, width);
    prefresh(new_window,y,x,y,x, height, width);
    refresh();
    windows_.push_back(new_window);
    return windows_.size() - 1;
}

bool AdapterPDCur::Refresh()
{
    return refresh();
}

bool AdapterPDCur::RefreshWin(int id_win)
{
    return wrefresh(windows_[id_win]);
}

bool AdapterPDCur::RefreshPad(int id_pad, int py, int px, int s1y, int s1x, int s2y, int s2x)
{
    return prefresh(windows_[id_pad], py, px, s1y, s1x, s2y, s2x);
}

void AdapterPDCur::ClearLine(int id_screen, int y, int x, int width, int color_pair)
{
    BasicString spaces(width, ' ');
    Print(id_screen, y, x, spaces, color_pair);
}

void AdapterPDCur::ClearAll()
{
    erase();
    refresh();
}

int AdapterPDCur::GetchSymbol()
{
    return getch();
}



void AdapterPDCur::ViewCursor(int y, int x, char symbol, int color_pair, int id_screen)
{
    attron(COLOR_PAIR(color_pair));
    mvprintw(y, x, "%c", symbol);
    attroff(COLOR_PAIR(color_pair));
}

int AdapterPDCur::MvScan(int y, int x, int id_screen)
{
    return mvwgetch(windows_[id_screen], y, x);
}