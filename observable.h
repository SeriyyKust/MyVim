#pragma once
#include "observer.h"
#include "vector"

class Observable
{
protected:
    std::vector<Observer*> observers;

public:
    inline void AddObserver(Observer* observer) 
    {
        observers.push_back(observer);
    }

    inline void DeleteObserver()
    {
        observers.pop_back();
    }

    void ChangeLine(const BasicString& string, int line, int hex_cursor, int position_first, int position_last)
    {
        int index = 0;
        for (auto& observer : observers) 
        {
            if (index >= position_first && index < position_last)
                observer->UpdateLine(string,line, hex_cursor);
            ++index;
        }
    }

    void ChangeCursor(const int y, const int x, BasicString name, BasicString type, int position_first, int position_last)
    {
        int index = 0;
        for (auto& observer : observers)
        {
            if (index >= position_first && index < position_last)
            observer->UpdateCursor(y, x, name, type);
            ++index;
        }
    }

    void ChangeCross(bool step, int position_first, int position_last)
    {
        int index = 0;
        for (auto& observer : observers)
        {
            if (index >= position_first && index < position_last)
            observer->UpdateCross(step);
            ++index;
        }
    }

    void ChangeType(TypeUpdate type)
    {
        {
            for (auto& observer : observers)
            {
                observer->UpdateType(type);
            }
        }
    }
};

