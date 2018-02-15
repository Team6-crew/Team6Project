#pragma once

#include <string>

namespace nclgl
{
	namespace Maths
	{
		class Vector2;
		class Vector3;
		class Vector4;
		class Matrix4;
	}
}


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
	virtual void SetUniform(const std::string& name, const nclgl::Maths::Matrix4& mat) = 0;
	virtual void SetUniform(const std::string& name, const nclgl::Maths::Vector2& vec) = 0;
	virtual void SetUniform(const std::string& name, const nclgl::Maths::Vector3& vec) = 0;
	virtual void SetUniform(const std::string& name, const nclgl::Maths::Vector4& vec) = 0;
	virtual void SetUniform(const std::string& name, int numMats, nclgl::Maths::Matrix4* mats) = 0;
	virtual void SetUniform(const std::string& name, int numFloats, float* floats) = 0;
	virtual void SetUniform(const std::string& name, int numVecs, nclgl::Maths::Vector3* vecs) = 0;
protected:
	bool loadFailed = true;
};

