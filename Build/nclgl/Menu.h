#pragma once

#include "NCLDebug.h"
#include "../ncltech/SceneManager.h"

class Menu : public NCLDebug
{
public:
	Menu();

	~Menu();

	void ShowMenu();
	void AddMenuItem(string item);
	void MoveUp();
	void MoveDown();
	
	void setSelection(int sel);
	bool visible = false;
	int getSelection();
	
protected:
	int selection;
	vector<string> Items;
	
};


