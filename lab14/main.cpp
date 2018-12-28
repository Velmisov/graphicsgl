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
GLuint shaderProgram1, shaderProgram2, shaderProgram3, shaderProgram4;
GLuint currentShaderProgram;
GLuint vertArrObj;
GLint uniformFor1, uniformFor2;

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
		res += "\n " + line;
	return res;
}

void initShader1()
{
	string readed = readShader("vertex1.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("fragment1.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vShader);
	glAttachShader(shaderProgram1, fShader);
	glLinkProgram(shaderProgram1);

	int link_ok;
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(1)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgram1, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	uniformFor1 = glGetUniformLocation(shaderProgram1, "objColor");
	if (uniformFor1 == -1)
	{
		cout << "could not bind uniform objColor(1)" << endl;
		return;
	}
}

void initShader2()
{
	string readed = readShader("vertex2.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("fragment2.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vShader);
	glAttachShader(shaderProgram2, fShader);
	glLinkProgram(shaderProgram2);

	int link_ok;
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(2)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgram2, 512, &size, infoLog);
		cout << infoLog;
		return;
	}
	uniformFor2 = glGetUniformLocation(shaderProgram2, "objColor");
	if (uniformFor2 == -1)
	{
		cout << "could not bind uniform objColor(2)" << endl;
		return;
	}
}

void initShader3()
{
	string readed = readShader("vertex3.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("fragment3.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgram3 = glCreateProgram();
	glAttachShader(shaderProgram3, vShader);
	glAttachShader(shaderProgram3, fShader);
	glLinkProgram(shaderProgram3);

	int link_ok;
	glGetProgramiv(shaderProgram3, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(3)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgram3, 512, &size, infoLog);
		cout << infoLog;
		return;
	}
}

void initShader4()
{
	string readed = readShader("vertex4.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("fragment4.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgram4 = glCreateProgram();
	glAttachShader(shaderProgram4, vShader);
	glAttachShader(shaderProgram4, fShader);
	glLinkProgram(shaderProgram4);

	int link_ok;
	glGetProgramiv(shaderProgram4, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(4)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgram4, 512, &size, infoLog);
		cout << infoLog;
		return;
	}
}

void initShaders() {
	initShader1();
	initShader2();
	initShader3();
	initShader4();
	currentShaderProgram = shaderProgram1;
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
	
	float blue[4] = { 0, 0, 1, 1 };
	if (mode == 1)
	{
		glUseProgram(shaderProgram1);
		glUniform4fv(uniformFor1, 1, blue);
	}
	else if (mode == 2)
	{
		glUseProgram(shaderProgram2);
		glUniform4fv(uniformFor2, 1, blue);
	}
	else if (mode == 3)
	{
		GLint Unif_tex = glGetUniformLocation(shaderProgram3, "myTexture");
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUseProgram(currentShaderProgram);
	}
	else
	{
		GLint Unif_tex = glGetUniformLocation(shaderProgram4, "myTexture");
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUseProgram(currentShaderProgram);
	}

	setTransform();
	setPointLight();
	float m_ambient[]{ 1.0f, 1.0f, 1.0f, 1.0f};
	float m_diffuse[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	float m_specular[]{ 0.2f, 0.2f, 0.2f, 1.0f };
	float m_emission[]{ 0.0f, 0.0f, 0.0f, 1.0f };
	float m_shiness = 0;
	setMaterial(m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(vertArrObj);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	if (glIsEnabled(GL_TEXTURE_2D))
		glDisable(GL_TEXTURE_2D);
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
			currentShaderProgram = shaderProgram1;
			break;
		case '2':
			mode = 2;
			currentShaderProgram = shaderProgram2;
			break;
		case '3':
			mode = 3;
			currentShaderProgram = shaderProgram3;
			break;
		case '4':
			mode = 4;
			currentShaderProgram = shaderProgram4;
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
	glutCreateWindow("lab 14");

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
	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);
	glDeleteProgram(shaderProgram3);
	glDeleteProgram(shaderProgram4);

	glDeleteBuffers(1, &vertArrObj);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}