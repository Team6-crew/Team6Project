#pragma once

#include <nclgl\TSingleton.h>


class RenderBase;

class RenderFactory : public TSingleton<RenderFactory> 
{
	friend class TSingleton<RenderFactory>;
public:
	RenderBase* MakeRenderer();
	RenderBase* GetRenderer();
private:
	RenderFactory();
	~RenderFactory();

	RenderBase* renderer = nullptr;
};

