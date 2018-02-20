#pragma once

#include <nclgl\NCLDebug.h>
#include "../ncltech/SceneManager.h"

#include <string>
#include <vector>	

class Menu : public NCLDebug
{
public:
	Menu();

	~Menu();

	void ShowMenu();
	void AddMenuItem(std::string item);
	void MoveUp();
	void MoveDown();
	
	void setSelection(int sel);
	bool visible = false;
	int getSelection();
	
protected:
	int selection;
	std::vector<std::string> Items;
	
};


