#include <Windows.h>
#include <stdio.h>
#include "GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <conio.h>
#include <math.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

const double PI = acos(-1);

GLuint texture;
int width = 0, height = 0;
int mode = 1;
GLuint shaderProgramLambert, shaderProgramBlinn, shaderProgramMinnaert, shaderProgramToon, shaderProgramAmi, shaderProgramRim;
GLuint currentShaderProgram;
GLuint vertArrObj;

mat4 modelPos, viewProjection;
mat3 normaltr;

float viewPosition[]{ 0, 10, -20 };
float light_angle = 0, light_pos = 0, light_rad = 50;
float light[]{ 0, 5, 0 };

vector<unsigned short> indices;
GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;
GLuint elementbuffer;

string objectName = "apple.obj";
string objectTextureName = "apple.jpg";
double scaleObj = 0.1;
float rotateX = 0, rotateY = 0, rotateZ = 0;

void loadTexture()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	texture = SOIL_load_OGL_texture
	(
		objectTextureName.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}

string readShader(const char* path)
{
	string res = "";
	ifstream file(path);
	string line;
	getline(file, res, '\0');
	while (getline(file, line))
	{
		res += "\n " + line;
	}
	return res;
}

void initLambertShader()
{
	string readed = readShader("lamberv.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("lamberf.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgramLambert = glCreateProgram();
	glAttachShader(shaderProgramLambert, vShader);
	glAttachShader(shaderProgramLambert, fShader);
	glLinkProgram(shaderProgramLambert);

	int link_ok;
	glGetProgramiv(shaderProgramLambert, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(1)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgramLambert, 512, &size, infoLog);
		cout << infoLog;
		return;
	}
}

void initBlinnShader()
{
	string readed = readShader("blinnv.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("blinnf.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgramBlinn = glCreateProgram();
	glAttachShader(shaderProgramBlinn, vShader);
	glAttachShader(shaderProgramBlinn, fShader);
	glLinkProgram(shaderProgramBlinn);

	int link_ok;
	glGetProgramiv(shaderProgramBlinn, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(2)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgramBlinn, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	
}

void initMinnaertShader()
{
	string readed = readShader("minnaertv.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("minnaertf.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgramMinnaert = glCreateProgram();
	glAttachShader(shaderProgramMinnaert, vShader);
	glAttachShader(shaderProgramMinnaert, fShader);
	glLinkProgram(shaderProgramMinnaert);

	int link_ok;
	glGetProgramiv(shaderProgramMinnaert, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(3)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgramMinnaert, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	
}

void initToonShader()
{
	string readed = readShader("toonv.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("toonf.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgramToon = glCreateProgram();
	glAttachShader(shaderProgramToon, vShader);
	glAttachShader(shaderProgramToon, fShader);
	glLinkProgram(shaderProgramToon);

	int link_ok;
	glGetProgramiv(shaderProgramToon, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(4)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgramToon, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	
}

void initAmiShader()
{
	string readed = readShader("amiv.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("amif.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgramAmi = glCreateProgram();
	glAttachShader(shaderProgramAmi, vShader);
	glAttachShader(shaderProgramAmi, fShader);
	glLinkProgram(shaderProgramAmi);

	int link_ok;
	glGetProgramiv(shaderProgramAmi, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(5)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgramAmi, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	
}

void initRimShader()
{
	string readed = readShader("rimv.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("rimf.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgramRim = glCreateProgram();
	glAttachShader(shaderProgramRim, vShader);
	glAttachShader(shaderProgramRim, fShader);
	glLinkProgram(shaderProgramRim);

	int link_ok;
	glGetProgramiv(shaderProgramRim, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(6)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgramRim, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	
}

void initShaders() {
	initLambertShader();
	initBlinnShader();
	initMinnaertShader();
	initToonShader();
	initAmiShader();
	initRimShader();
	currentShaderProgram = shaderProgramLambert;
}

void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
}

void loadOBJ(const string & path, vector<vec3> & out_vertices, vector<vec2> & out_uvs, vector<vec3> & out_normals)
{
	vector<unsigned int> vertex_indices, uv_indices, normal_indices;
	vector<vec3> temp_vertices;
	vector<vec2> temp_uvs;
	vector<vec3> temp_normals;

	ifstream infile(path);
	string line;
	while (getline(infile, line))
	{
		stringstream ss(line);
		string lineHeader;
		getline(ss, lineHeader, ' ');
		if (lineHeader == "v")
		{
			vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;

			vertex.x *= scaleObj;
			vertex.y *= scaleObj;
			vertex.z *= scaleObj;
			temp_vertices.push_back(vertex);
		}
		else if (lineHeader == "vt")
		{
			vec2 uv;
			ss >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (lineHeader == "vn")
		{
			vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (lineHeader == "f")
		{
			unsigned int vertex_index, uv_index, normal_index;
			char slash;
			while (ss >> vertex_index >> slash >> uv_index >> slash >> normal_index)
			{
				vertex_indices.push_back(vertex_index);
				uv_indices.push_back(uv_index);
				normal_indices.push_back(normal_index);
			}
		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertex_indices.size(); i++)
	{
		unsigned int vertexIndex = vertex_indices[i];
		vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);

		unsigned int uvIndex = uv_indices[i];
		vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);

		unsigned int normalIndex = normal_indices[i];
		vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
}

// https://github.com/opengl-tutorials/ogl/blob/master/common/vboindexer.cpp
struct PackedVertex
{
	vec3 position;
	vec2 uv;
	vec3 normal;
	bool operator<(const PackedVertex that) const
	{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
	};
};

bool getSimilarVertexIndex_fast(PackedVertex & packed, map<PackedVertex, unsigned short> & VertexToOutIndex, unsigned short & result)
{
	map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end())
	{
		return false;
	}
	else
	{
		result = it->second;
		return true;
	}
}

void indexVBO(vector<vec3> & in_vertices, vector<vec2> & in_uvs, vector<vec3> & in_normals,
	vector<unsigned short> & out_indices, vector<vec3> & out_vertices, vector<vec2> & out_uvs, vector<vec3> & out_normals)
{
	map<PackedVertex, unsigned short> VertexToOutIndex;

	// For each input vertex
	for (unsigned int i = 0; i < in_vertices.size(); i++)
	{
		PackedVertex packed = { in_vertices[i], in_uvs[i], in_normals[i] };

		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found)
		{ // A similar vertex is already in the VBO, use it instead!
			out_indices.push_back(index);
		}
		else
		{ // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

void initBuffers()
{
	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<vec3> normals;
	vector<vec3> indexed_vertices;
	vector<vec2> indexed_uvs;
	vector<vec3> indexed_normals;
	loadOBJ(objectName.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &vertArrObj);

	glBindVertexArray(vertArrObj);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 3rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 2nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void updateLight()
{
	light[0] = light_rad * cos(light_angle / 180 * PI);
	light[1] = light_pos;
	light[2] = light_rad * sin(light_angle / 180 * PI);
}

void setTransform()
{
	GLint s_model = glGetUniformLocation(currentShaderProgram, "transform.model");
	GLint s_proj = glGetUniformLocation(currentShaderProgram, "transform.viewProjection");
	GLint s_normal = glGetUniformLocation(currentShaderProgram, "transform.normal");
	GLint s_view = glGetUniformLocation(currentShaderProgram, "transform.viewPosition");

	glUniformMatrix4fv(s_model, 1, GL_FALSE, &modelPos[0][0]);
	glUniformMatrix4fv(s_proj, 1, GL_FALSE, &viewProjection[0][0]);
	glUniformMatrix3fv(s_normal, 1, GL_FALSE, &normaltr[0][0]);
	glUniform3fv(s_view, 1, viewPosition);

	
}

void setPointLight()
{
	GLint s_position = glGetUniformLocation(currentShaderProgram, "light.position");
	GLint s_ambient = glGetUniformLocation(currentShaderProgram, "light.ambient");
	GLint s_diffuse = glGetUniformLocation(currentShaderProgram, "light.diffuse");
	GLint s_specular = glGetUniformLocation(currentShaderProgram, "light.specular");
	GLint s_attenuation = glGetUniformLocation(currentShaderProgram, "light.attenuation");

	float ambient[]{ 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuse[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	float specular[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	float attenuation[]{ 1.0f, 0.0f, 0.0f };

	glUniform4fv(s_position, 1, light);
	glUniform4fv(s_ambient, 1, ambient);
	glUniform4fv(s_diffuse, 1, diffuse);
	glUniform4fv(s_specular, 1, specular);
	glUniform3fv(s_attenuation, 1, attenuation);
}

void setMaterial(float* m_ambient, float* m_diffuse, float* m_specular, float* m_emission, float m_shiness)
{
	GLint s_ambient = glGetUniformLocation(currentShaderProgram, "material.ambient");
	GLint s_diffuse = glGetUniformLocation(currentShaderProgram, "material.diffuse");
	GLint s_specular = glGetUniformLocation(currentShaderProgram, "material.specular");
	GLint s_emission = glGetUniformLocation(currentShaderProgram, "material.emission");
	GLint s_shiness = glGetUniformLocation(currentShaderProgram, "material.shiness");

	glUniform4fv(s_ambient, 1, m_ambient);
	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform4fv(s_emission, 1, m_emission);
	glUniform1f(s_shiness, m_shiness);
}

void Lambert()
{
	glUseProgram(currentShaderProgram);
	float fColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLint color = glGetUniformLocation(currentShaderProgram, "diffColor");
	glUniform4fv(color, 1, fColor);
	
	setTransform();

	GLint s_position = glGetUniformLocation(currentShaderProgram, "light.position");
	glUniform4fv(s_position, 1, light);
}

void Blinn()
{
	glUseProgram(currentShaderProgram);
	float fColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLint color = glGetUniformLocation(currentShaderProgram, "diffColor");
	glUniform4fv(color, 1, fColor);

	setTransform();
	GLint s_position = glGetUniformLocation(currentShaderProgram, "light.position");
	glUniform4fv(s_position, 1, light);
	
	GLint s_diffuse = glGetUniformLocation(currentShaderProgram, "material.diffuse");
	GLint s_specular = glGetUniformLocation(currentShaderProgram, "material.specular");
	GLint s_shiness = glGetUniformLocation(currentShaderProgram, "material.shiness");

	float m_diffuse[]{ 0.5f,0.0f,0.0f,1.0f };
	float m_specular[]{ 0.7f,0.7f,0.0f,1.0f };
	float m_shiness = 30;

	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform1f(s_shiness, m_shiness);
}

void Minnaert()
{
	glUseProgram(currentShaderProgram);
	float fColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLint color = glGetUniformLocation(currentShaderProgram, "diffColor");
	glUniform4fv(color, 1, fColor);
	
	setTransform();
	GLint s_position = glGetUniformLocation(currentShaderProgram, "light.position");
	glUniform4fv(s_position, 1, light);
	
	float koef = 0.8;
	GLint s_k = glGetUniformLocation(currentShaderProgram, "k");
	glUniform1f(s_k, koef);
}

void Toon()
{
	glUseProgram(currentShaderProgram);
	float fColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLint color = glGetUniformLocation(currentShaderProgram, "diffColor");
	glUniform4fv(color, 1, fColor);
	
	setTransform();
	GLint s_position = glGetUniformLocation(currentShaderProgram, "light.position");
	glUniform4fv(s_position, 1, light);
}

void Ami()
{
	glUseProgram(currentShaderProgram);
	float fColor[4] = { 0.75f, 0.75f, 0.75f, 1.0f };
	GLint color = glGetUniformLocation(currentShaderProgram, "diffColor");
	glUniform4fv(color, 1, fColor);
	
	setTransform();
	GLint s_position = glGetUniformLocation(currentShaderProgram, "light.position");
	glUniform4fv(s_position, 1, light);
	
	float oWarm[4] = { 0.8f, 0.0f, 0.0f, 1.0f };
	GLint warm = glGetUniformLocation(currentShaderProgram, "warm");
	glUniform4fv(warm, 1, oWarm);
	
	float oCold[4] = { 0.0f, 0.0f, 0.6f, 1.0f};
	GLint cold = glGetUniformLocation(currentShaderProgram, "cold");
	glUniform4fv(cold, 1, oCold);
	
	float oDiffWarm = 0.45;
	GLint dwarm = glGetUniformLocation(currentShaderProgram, "diffwarm");
	glUniform1f(dwarm, oDiffWarm);
	
	float oDiffCold = 0.45;
	GLint dcold = glGetUniformLocation(currentShaderProgram, "diffcold");
	glUniform1f(dcold, oDiffCold);
}

void Rim()
{
	glUseProgram(currentShaderProgram);

	setTransform();
	GLint s_position = glGetUniformLocation(currentShaderProgram, "light.position");
	glUniform4fv(s_position, 1, light);
	
	float diffc[4] = { 0.5f, 0.0f, 0.0f, 1.0f };
	GLint diff = glGetUniformLocation(currentShaderProgram, "diff");
	glUniform4fv(diff, 1, diffc);
	
	float specc[4] = { 0.7f, 0.7f, 0.0f, 1.0f };
	GLint spec = glGetUniformLocation(currentShaderProgram, "spec");
	glUniform4fv(spec, 1, specc);
	
	float ospecPow = 30.0f;
	GLint specpow = glGetUniformLocation(currentShaderProgram, "specPow");
	glUniform1f(specpow, ospecPow);
	
	float orimPow = 8.0f;
	GLint rimpow = glGetUniformLocation(currentShaderProgram, "rimPow");
	glUniform1f(rimpow, orimPow);
	
	float obias = 0.3f;
	GLint bias = glGetUniformLocation(currentShaderProgram, "bias");
	glUniform1f(bias, obias);
}

void show(void)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	modelPos = mat4(1.0f);
	modelPos = rotate(modelPos, radians(rotateX), vec3(1, 0, 0));
	modelPos = rotate(modelPos, radians(rotateY), vec3(0, 1, 0));
	modelPos = rotate(modelPos, radians(rotateZ), vec3(0, 0, 1));

	viewProjection = perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	viewProjection *= lookAt(
		vec3(viewPosition[0], viewPosition[1], viewPosition[2]),
		vec3(0, 0, 0),
		vec3(0, 1, 0)
	);
	normaltr = transpose(inverse(modelPos));
	
	switch (mode)
	{
	case 1:
		Lambert();
		break;
	case 2:
		Blinn();
		break;
	case 3:
		Minnaert();
		break;
	case 4:
		Toon();
		break;
	case 5:
		Ami();
		break;
	case 6:
		Rim();
		break;
	}

	glBindVertexArray(vertArrObj);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	
	glutSwapBuffers();
}

void lightManager(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		light_pos += 0.5;
		break;
	case GLUT_KEY_DOWN:
		light_pos -= 0.5;
		break;
	case GLUT_KEY_RIGHT:
		light_angle -= 3;
		break;
	case GLUT_KEY_LEFT:
		light_angle += 3;
		break;
	case GLUT_KEY_PAGE_UP:
		light_rad -= 0.5;
		break;
	case GLUT_KEY_PAGE_DOWN:
		light_rad += 0.5;
		break;
	default:
		break;
	}
	updateLight();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		mode = 1;
		currentShaderProgram = shaderProgramLambert;
		break;
	case '2':
		mode = 2;
		currentShaderProgram = shaderProgramBlinn;
		break;
	case '3':
		mode = 3;
		currentShaderProgram = shaderProgramMinnaert;
		break;
	case '4':
		mode = 4;
		currentShaderProgram = shaderProgramToon;
		break;
	case '5':
		mode = 5;
		currentShaderProgram = shaderProgramAmi;
		break;
	case '6':
		mode = 6;
		currentShaderProgram = shaderProgramRim;
		break;
	case 'w':
		rotateX -= 1;
		break;
	case 's':
		rotateX += 1;
		break;
	case 'q':
		rotateY -= 1;
		break;
	case 'e':
		rotateY += 1;
		break;
	case 'a':
		rotateZ -= 1;
		break;
	case 'd':
		rotateZ += 1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(250, 5);
	glutCreateWindow("Lab 15");

	glewInit();
	initBuffers();
	loadTexture();
	initShaders();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	updateLight();

	glutDisplayFunc(show);
	glutReshapeFunc(reshape);
	glutSpecialFunc(lightManager);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	glUseProgram(0);
	glDeleteProgram(shaderProgramLambert);
	glDeleteProgram(shaderProgramBlinn);
	glDeleteProgram(shaderProgramMinnaert);
	glDeleteProgram(shaderProgramToon);
	glDeleteProgram(shaderProgramAmi);
	glDeleteProgram(shaderProgramRim);

	glDeleteBuffers(1, &vertArrObj);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}