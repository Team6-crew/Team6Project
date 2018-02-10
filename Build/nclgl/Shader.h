/******************************************************************************
Class:Shader
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:VERY simple class to encapsulate GLSL shader loading, linking,
and binding. Useful additions to this class would be overloaded functions to
replace the glUniformxx functions in external code, and possibly a map to store
uniform names and their resulting bindings. 

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once


#include <nclgl\Graphics\Renderer\OpenGL\OGLRenderer.h>

#include <string> //getline

#include <GL/glew.h>
#include <GL/wglew.h>

#define SHADER_VERTEX   0
#define SHADER_FRAGMENT 1
#define SHADER_GEOMETRY 2

using namespace std;
class Shader	{
public:
	Shader(std::string vertex, std::string fragment , std::string geometry = "");
	~Shader(void);

	GLuint  GetProgram() { return program;}
	

	void	Reload(bool deleteOld = true);


	bool	LoadSuccess() {
		return !loadFailed;
	}
	bool	LinkProgram();
protected:
	bool	LoadShaderFile(std::string from, std::string &into);
	GLuint	GenerateShader(std::string from, GLenum type);
	void	SetDefaultAttributes();
	
	GLuint	objects[3];
	GLuint	program;

	bool	loadFailed;

	std::string	vertexName;
	std::string	fragName;
	std::string	geomName;
};

