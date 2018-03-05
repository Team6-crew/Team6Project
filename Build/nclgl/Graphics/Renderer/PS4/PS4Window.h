#pragma once

#include <nclgl\WindowBase.h>

class PS4Window : public WindowBase
{
public:
	PS4Window();
	~PS4Window();

	bool UpdateWindow() override;
};

