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
	void replaceMenuItem(int position, std::string item);
	Menu * onMenuSelect() { return subMenu[selection]; }
	void addToMenu(Menu * mn, int pos) { subMenu[pos] = mn; }
	void set_id(int i) { id = i; }
	int get_id() { return id; }
	int lastElement();


	void ShowPauseMenu();
protected:
	int selection;
	std::vector<std::string> Items;
	std::vector<Menu*> subMenu;
	int id;


};


