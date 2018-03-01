#pragma once

#include <nclgl\TSingleton.h>
class InputBase;

class ControllerFactory : public TSingleton<ControllerFactory>
{
	friend class TSingleton<ControllerFactory>;
public:
	InputBase* GetController();
private:
	ControllerFactory();
	~ControllerFactory();
};

