#include <iostream>
#include <fstream>
#include "controller.h"
#include "View.h"
#include "Adapter.h"

#include "workwithfile.h"


const std::string path_o = "output.txt";

int ParserArgs(int argc, char* argv[], std::string& path)
{
    if (argc > 2 || argc <= 0)
    {
        std::cout << "Incorrect arguments, see help\n";
        return -1;
    }
    if (argc == 1)
    {
        path = path_o;
        return 1;
    }
    if (argc == 2)
    {
        std::string check = argv[1];
        int i = check.size() - 1;
        for (; i >= 0 && check[i] != '.'; i--);
        if (i < 0)
        {
            std::cout << "Incorrect arguments, see help\n";
            return -1;
        }
        path = argv[1];
        return 1;
    }


}

int main(int argc, char* argv[])
{
    
    std::string path;

    if (ParserArgs(argc, argv, path) < 0)
        return 0;

    //path = "tim.txt";

    int length_hex_text = 16;
    IAdapterPDCur* adapter = new AdapterPDCur();
    View screen(adapter);
    int terminal_width = adapter->GetMaxWidthMainWin();
    int terminal_height = adapter->GetMaxHeightMainWin();
    BasicString buffer;
    NavigationEdit* readmodel = new NavigationEdit(terminal_height - 3, terminal_width - 3,  &buffer,& screen);
    CommandModel* commandmodel = new CommandModel(terminal_height - 3, terminal_width - 3, &buffer, &screen);
    SearchModel* searchmodel = new SearchModel (terminal_height - 3, terminal_width - 3, &buffer, &screen);
    EditModel* editmodel = new EditModel (terminal_height - 3, terminal_width - 3, &buffer, &screen);
    HexModel* hexmodel = new HexModel(terminal_height - 6, length_hex_text, &buffer, &screen);
    std::vector<IModel*> models;
    models.push_back(readmodel);
    models.push_back(commandmodel);
    models.push_back(searchmodel);
    models.push_back(editmodel);
    models.push_back(hexmodel);
    

    Controller controll(adapter, models,TypeController::cReadEdit);

    controll.StartApplication(path);
    
    
    
    return 0;
}