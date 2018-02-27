#include "OGLShader.h"
#include <nclgl\NCLDebug.h>


// Note: Phil only including this for enums
#include <nclgl\Graphics\Renderer\OpenGL\OGLMesh.h>

#include <fstream>

using namespace std;
using std::string;
using namespace nclgl::Maths;

#include <iostream>
#include <cassert>

DEFINE_HEAP(OGLShader, "Graphics");

OGLShader::OGLShader(std::string vFile, std::string fFile, std::string gFile) {
	NCLDebug::Log("Loading Shader:");

	program = glCreateProgram();
	objects[SHADER_VERTEX] = GenerateShader(vFile, GL_VERTEX_SHADER);
	objects[SHADER_FRAGMENT] = GenerateShader(fFile, GL_FRAGMENT_SHADER);
	objects[SHADER_GEOMETRY] = 0;

	if (!gFile.empty()) {
		objects[SHADER_GEOMETRY] = GenerateShader(gFile, GL_GEOMETRY_SHADER);
		glAttachShader(program, objects[SHADER_GEOMETRY]);
	}

	glAttachShader(program, objects[SHADER_VERTEX]);
	glAttachShader(program, objects[SHADER_FRAGMENT]);

	SetDefaultAttributes();

	LinkProgram();
}

OGLShader::~OGLShader(void) {
	for (int i = 0; i < 3; ++i) {
		glDetachShader(program, objects[i]);
		glDeleteShader(objects[i]);
	}
	glDeleteProgram(program);
}

bool	OGLShader::LoadShaderFile(string from, string &into) {
	ifstream	file;
	string		temp;
	
	file.open(from.c_str());
	if (!file.is_open()) {	
		return false;
	}

	while (!file.eof()) {
		getline(file, temp);
		into += temp + "\n";
	}

	file.close();	
	return true;
}

GLuint	OGLShader::GenerateShader(std::string from, GLenum type) {

	NCLDebug::Log("    -> Compiling Shader: %s", from.c_str());

	string load;
	if (!LoadShaderFile(from, load)) {
		NCLERROR("    ERROR: File does not exist!");
		loadFailed = true;
		return 0;
	}

	GLuint shader = glCreateShader(type);

	const char *chars = load.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		char error[512];
		glGetInfoLogARB(shader, sizeof(error), NULL, error);
		cout << error;
		loadFailed = true;
		cout << endl;

		//NCLERROR("    ERROR: Cannot compile shader");
		return 0;
	}
	loadFailed = false;
	return shader;
}

bool OGLShader::LinkProgram() {
	if (loadFailed) {
		return false;
	}
	glLinkProgram(program);

	GLint code;
	glGetProgramiv(program, GL_LINK_STATUS, &code);

	if (code == GL_FALSE) {		
		char error[512];
		glGetInfoLogARB(program, sizeof(error), NULL, error);
		cout << error;
		loadFailed = true;

		NCLERROR("    -> Linking Shader: Failed!");
		return false;
	}

	NCLDebug::Log("    -> Linking Shader: Success!");
	NCLDebug::Log("");
	return true;
}

void	OGLShader::SetDefaultAttributes() {
	glBindAttribLocation(program, VERTEX_BUFFER, "position");
	glBindAttribLocation(program, COLOUR_BUFFER, "colour"); //ARGHHH
	glBindAttribLocation(program, COLOUR_BUFFER, "color");
	glBindAttribLocation(program, NORMAL_BUFFER, "normal");
	glBindAttribLocation(program, TANGENT_BUFFER, "tangent");
	glBindAttribLocation(program, TEXTURE_BUFFER, "texCoord");

	glBindAttribLocation(program, MAX_BUFFER + 1, "transformIndex");
}


void OGLShader::Activate()
{
	glUseProgram(program);
}

void OGLShader::SetUniform(const std::string& name, int i)
{
	glUniform1i(GetUniformLocation(name), i);
}

void OGLShader::SetUniform(const std::string& name, float f)
{
	glUniform1f(GetUniformLocation(name), f);
}

void OGLShader::SetUniform(const std::string& name, const Matrix4& mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, false, (float*)&mat);
}

void OGLShader::SetUniform(const std::string& name, const Vector2& vec)
{
	glUniform2fv(GetUniformLocation(name), 1, (float*)& vec);
}

void OGLShader::SetUniform(const std::string& name, const Vector3& vec)
{
	glUniform3fv(GetUniformLocation(name), 1, (float*)& vec);
}

void OGLShader::SetUniform(const std::string& name, const Vector4& vec)
{
	glUniform4fv(GetUniformLocation(name), 1, (float*)& vec);
}

void OGLShader::SetUniform(const std::string& name, int numMats, Matrix4* mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), numMats, false, (float*)mat);
}

void OGLShader::SetUniform(const std::string& name, int numVecs, Vector3* vecs)
{
	glUniform3fv(GetUniformLocation(name), numVecs, (float*)vecs);
}

void OGLShader::SetUniform(const std::string& name, int numFloats, float* floats)
{
	glUniform1fv(GetUniformLocation(name), numFloats, floats);
}

GLint OGLShader::GetUniformLocation(string name)
{
	GLint location = glGetUniformLocation(program, name.c_str());
	if (location < 0)
	{
		cout << "Uniform " << name << " does not exist" << endl;
	}
	return location;
}

