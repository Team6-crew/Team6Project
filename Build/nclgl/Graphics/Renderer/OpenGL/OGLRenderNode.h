/******************************************************************************
Class:RenderNode
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:A basic, but functional RenderNode class. SceneNodes have a
transformation matrix, and any number of children. They also have a parent, 
forming a tree structure, with their parent above them, and children below.

They also have a seperate scale for their Mesh, for no other reason than
it made the CubeRobot class easier ;) You might find it useful, though...

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////


#pragma once
#include "../../../Matrix4.h"
#include "../../../Vector3.h"
#include "../../../Vector4.h"

#include "../RenderNodeBase.h"

class MeshBase;

class OGLRenderNode : public RenderNodeBase	
{
public:
	OGLRenderNode(MeshBase* m = NULL, nclgl::Maths::Vector4 colour = nclgl::Maths::Vector4(1, 1, 1, 1));
	virtual ~OGLRenderNode(void);	  


	virtual void	Update(float msec) override;
	void Draw() override;

	void							SetTransform(const  nclgl::Maths::Matrix4 &matrix) override;
	const  nclgl::Maths::Matrix4 	GetTransform() const override;
	const  nclgl::Maths::Matrix4 	GetWorldTransform() const override;
	void							SetWorldTransform(const  nclgl::Maths::Matrix4 &matrix) override;

	nclgl::Maths::Vector4		GetColour() override;
	void						SetColour(const  nclgl::Maths::Vector4 &c) override;

	const  nclgl::Maths::Vector3	GetModelScale()		const			override;
	void							SetModelScale(const  nclgl::Maths::Vector3 &s)		override;

protected:
	 nclgl::Maths::Matrix4		worldTransform;
	 nclgl::Maths::Matrix4		transform;

	 nclgl::Maths::Vector4		colour;
	 nclgl::Maths::Vector3		modelScale;
};

