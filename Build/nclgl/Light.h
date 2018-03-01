#pragma once
#include "Vector4.h"
#include "Vector3.h"

class Light {
public:
	Light() {}
	Light(nclgl::Maths::Vector3 position, nclgl::Maths::Vector4 colour, float radius) {
		this -> position = position;
		this -> colour = colour;
		this -> radius = radius;
	}
	
	~Light(void) {};
	
	nclgl::Maths::Vector3 GetPosition() const { return position; }
	void SetPosition(nclgl::Maths::Vector3 val) { position = val; }
	
	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }
	
	nclgl::Maths::Vector4 GetColour() const { return colour; }
	void SetColour(nclgl::Maths::Vector4 val) { colour = val; }

protected:
	nclgl::Maths::Vector3 position;
	nclgl::Maths::Vector4 colour;
	float radius;
};


