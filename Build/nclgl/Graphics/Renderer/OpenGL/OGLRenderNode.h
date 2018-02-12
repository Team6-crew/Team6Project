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
	OGLRenderNode(MeshBase* m = NULL, Vector4 colour = Vector4(1,1,1,1));
	virtual ~OGLRenderNode(void);


	virtual void	Update(float msec) override;
	void Draw() override;

	void			SetTransform(const Matrix4 &matrix) override;
	const Matrix4&	GetTransform() const override;
	const Matrix4&	GetWorldTransform() const override;
	void			SetWorldTransform(const Matrix4 &matrix) override;

	Vector4&		GetColour() override;
	void			SetColour(const Vector4 &c) override;

	const Vector3&	GetModelScale()		const			override; 
	void			SetModelScale(const Vector3 &s)		override;

protected:
	Matrix4		worldTransform;
	Matrix4		transform;

	Vector4		colour;
	Vector3		modelScale;
};

