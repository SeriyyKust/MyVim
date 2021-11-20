#pragma once
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "model.h"
#include "Adapter.h"
#include "vector"

enum class TypeController
{
	cReadEdit = 0,
	cCommand = 1,
	cSearch = 2,
	cEdit = 3,
	cHex = 4
};

class Controller
{
public:
	Controller(IAdapterPDCur* adapter, std::vector<IModel*> models, TypeController type) : models_(models), adapter_(adapter), type_(type)
	{
		index_window_ = int(type);

	}
	void StartApplication(BasicString path);

	bool CheckSymbols(char symbol);
	void ChangeMode(TypeController type);
	~Controller() {};
private:
	std::vector<IModel*> models_;
	IAdapterPDCur* adapter_;
	TypeController type_;
	int index_window_;
};

#endif // _CONTROLLER_H_
