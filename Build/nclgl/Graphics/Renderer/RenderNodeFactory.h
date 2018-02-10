#pragma once

#include <nclgl\TSingleton.h>

class RenderNodeBase;
class MeshBase;

#include <nclgl\Vector4.h>

class RenderNodeFactory : public TSingleton<RenderNodeFactory>
{
	friend class RenderNodeFactory;
public:
	RenderNodeBase * MakeRenderNode(MeshBase* mesh = nullptr, Vector4 colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f));
public:
	RenderNodeFactory();
	~RenderNodeFactory();
};

