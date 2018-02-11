#pragma once

class Vector2;
class Vector3;
class Vector4;
class Matrix4;

#include <string>

class ShaderBase
{
public:
	ShaderBase();
	~ShaderBase();

	bool	LoadSuccess() {
		return !loadFailed;
	}

	virtual void Activate() = 0;

	virtual void SetUniform(const std::string& name, int i) = 0;
	virtual void SetUniform(const std::string& name, float f) = 0;
	virtual void SetUniform(const std::string& name, const Matrix4& mat) = 0;
	virtual void SetUniform(const std::string& name, const Vector2& vec) = 0;
	virtual void SetUniform(const std::string& name, const Vector3& vec) = 0;
	virtual void SetUniform(const std::string& name, const Vector4& vec) = 0;
	virtual void SetUniform(const std::string& name, int numMats, Matrix4* mats) = 0;
	virtual void SetUniform(const std::string& name, int numFloats, float* floats) = 0;
	virtual void SetUniform(const std::string& name, int numVecs, Vector3* vecs) = 0;
protected:
	bool loadFailed = true;
};

