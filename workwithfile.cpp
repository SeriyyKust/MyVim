#include "workwithfile.h"

WorkWithFile::WorkWithFile(BasicString path_to_file)
{
    NewPath(path_to_file);
}

BasicString WorkWithFile::GetName()
{
    return name_file_;
}

BasicString WorkWithFile::GetPath()
{
    return path_to_file_;
}

int WorkWithFile::GetAmountLine(int width)
{
    std::ifstream file_input(path_to_file_.CStr());
    if (!file_input) 
    {
        return 0;
    }
    char buffer;
    int amount_symbol = 0;
    int amount_line = 0;
    while (!file_input.eof())
    {
        file_input.read(&buffer, 1);
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
    if (amount_symbol > 0)
        amount_line++;
    file_input.close();
    return amount_line;
}

BasicString WorkWithFile::GetLine(int number_line, int width)
{
    std::ifstream file_input(path_to_file_.CStr());
    BasicString line;
    if (!file_input)
    {
        return line;
    }
    char buffer;
    int amount_symbol = 0;
    int amount_line = 0;
    int amount = 0;
    while (!file_input.eof() && amount_line < number_line)
    {
        file_input.read(&buffer, 1);
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
    while (!file_input.eof())
    {
        file_input.read(&buffer, 1);
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
    file_input.close();
    return line;
}

void WorkWithFile::WriteFromBuffer(const char* str, int length)
{
    std::ofstream fin(path_to_file_.CStr());
    fin.write(str, length);
    fin.close();
}


void WorkWithFile::DeleteFile(BasicString path_to_file)
{
    BasicString com;
    com.Insert(0, "del ");
    com.Insert(4, path_to_file.CStr());
    system(com.CStr());
    //system("cls");
}

void WorkWithFile::NewPath(BasicString path_to_file)
{
    name_file_.Clear();
    path_to_file_.Clear();

    if (path_to_file.Empty())
    {
        path_to_file_ = "Logo.txt";
    }
    else
    {
        path_to_file_ = path_to_file;
    }

    int i = 0;
    auto it = path_to_file_.RBegin();
    while ((i < path_to_file_.Size()) && (*it != '\\'))
    {
        ++i, ++it;
    }

    if (i == path_to_file_.Size())
    {
        name_file_ = path_to_file_;
        return;
    }

    i = path_to_file_.Size() - i;
    name_file_ = path_to_file_.Substr(i);
}

bool WorkWithFile::Open()
{
    std::ifstream file_input(path_to_file_.CStr());
    if (!file_input)
    {
        return false;
    }
    else true;
}

