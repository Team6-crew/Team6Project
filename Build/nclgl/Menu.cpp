#include "Menu.h"

using namespace nclgl::Maths;
using namespace std;

Menu::Menu() :selection(0)
{

}

Menu::~Menu()
{
}

void Menu::AddMenuItem(string item) {
	Items.push_back(item);
}

void Menu::MoveUp() {
	selection--;
	if (selection < 0) {
		selection = Items.size() - 1;
	}
}

void Menu::MoveDown() {
	selection++;
	if (selection > Items.size() - 1) {
		selection = 0;
	}
}

void Menu::ShowMenu()
{
	const Vector2 ss = Window::GetWindow().GetScreenSize();
	float cs_size_x = 0;
	float cs_size_y = 60.f / ss.y * 2.0f;

	const Vector4 log_background_highlight(0.0f, 0.0f, 1.0f, 1.0f);

	for (std::vector<string>::iterator it = Items.begin(); it != Items.end(); ++it)
	{
		Vector4 color = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

		if (selection == g_NumStatusEntries) 
		{
			color = log_background_highlight;
		}

		DrawTextCs(Vector4(cs_size_x, 0.4f-(g_NumStatusEntries * cs_size_y), -1.0f, 1.0f), 60.f, *it, TEXTALIGN_CENTRE, color);
		g_NumStatusEntries++;
	}
}

int Menu::getSelection() {
	return selection;
}
void Menu::setSelection(int sel) {
	selection = sel;
}