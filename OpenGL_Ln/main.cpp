#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "TextureLN.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#ifdef linux
#ifndef MAX_PATH
#define MAX_PATH 260
#endif // !MAX_PATH
#endif // linux

#define FPS 30
#define DEFAULT_DELTA 0.0333f

void framebuffer_size_callback(GLFWwindow* window, int w, int h) 
{
	glViewport(0, 0, w, h);
}
void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

static const GLchar* getFileData(const char * path)
{
	char real_path[MAX_PATH] = {0};
	strcpy_s(real_path, path);
#ifdef WIN32
	memset(real_path, 0, MAX_PATH);
	const char* prefix = "../Resources/";
	strcpy_s(real_path, prefix);
	strcat_s(real_path, path);
#endif
	FILE* infile;
	fopen_s(&infile, real_path, "rb");
	if (!infile)
	{
		return NULL;
	}
	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar *source = new GLchar[len + 1];
	fread(source, 1, len, infile);
	fclose(infile);
	source[len] = 0;
	return const_cast<const GLchar *>(source);
}

glm::mat4 localTransform(const float dither)
{
	// local
	glm::mat4 model;
	model = glm::rotate(model, dither * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	return model;
}

glm::mat4 cameraTransform(const glm::vec3& pos)
{
	// camera
	glm::mat4 camera;
	camera = glm::translate(camera, (glm::vec3(0.0f, 0.0f, 0.0f) - pos));
	return camera;
}

glm::mat4 projectionTransform()
{
	// projection
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);
	return projection;
}

glm::vec4 origin(0.0f, 0.0f, 6.0f, 1.0f);
glm::vec3 genCameraPos()
{
	auto t = glfwGetTime();
	glm::mat4 trans;
	trans = glm::rotate(trans, (float)(glm::radians(t * 10)), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 ret = trans * origin;

	auto v3ret = glm::vec3(ret.x, ret.y, ret.z);
	std::cout << v3ret.length() << std::endl;
	return v3ret;
}

int main()
{
	stbi_set_flip_vertically_on_load(true);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(720, 720, "GL_LN", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Err: Failed to create window" << std::endl;
		glfwTerminate(); // 终止进程
		return -1;
	}

	// 将窗口的上下文切换为当前主线程上下文
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Err: Failed to init glad" << std::endl;
		return -1;
	}

	// 注册窗口回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	const GLchar* vertexShaderSource = getFileData("vertics.glsl");
	const GLchar* fragmentShaderSource = getFileData("fragment.glsl");

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	
	/*unsigned int indices[] = {
		0, 1, 3,
		1, 0, 2,
		1, 2, 3,
		2, 0, 3
	};*/

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	OPENGL_LN::TextureLN tln = OPENGL_LN::TextureLN();
	tln.clearTextureCache();
	tln.flushSingleImgIntoBuffer("test.jpg");

	OPENGL_LN::Shader sr = OPENGL_LN::Shader("vertex.glsl", "fragment.glsl");

	float deltaTime = 0;
	float curT = glfwGetTime();
	float lastT = curT;
	sr.use();
	sr.setVal("texture1", 0);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// 渲染
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // 设置屏幕颜色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清屏


		curT = glfwGetTime();
		float deltaTime = curT - lastT;
		lastT = curT;
		if (deltaTime < DEFAULT_DELTA)
		{
			Sleep((DEFAULT_DELTA - deltaTime) * 1000);
		}
		// std::cout << floor(1 / deltaTime) << std::endl;

		// render
		tln.tick();
		sr.use();
		auto model = localTransform(0);
		auto view = cameraTransform(genCameraPos());
		auto projection = projectionTransform();
		sr.setTrans("model", glm::value_ptr(model));
		sr.setTrans("view", glm::value_ptr(view));
		sr.setTrans("projection", glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, floor(sizeof(vertices)/sizeof(float)));
		

		// dky?
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}