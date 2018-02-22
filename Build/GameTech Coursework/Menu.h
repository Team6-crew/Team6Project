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
	bool Submenu() { return Submenu_active; };
	void replaceMenuItem(int position, std::string item);

protected:
	int selection;
	std::vector<std::string> Items;
	bool Submenu_active;
};


