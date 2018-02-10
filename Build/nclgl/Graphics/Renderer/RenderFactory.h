#pragma once

#include <nclgl\TSingleton.h>


class RenderBase;

class RenderFactory : public TSingleton<RenderFactory> 
{
	friend class TSingleton<RenderFactory>;
public:
	RenderBase* MakeRenderer();
private:
	RenderFactory();
	~RenderFactory();
};

