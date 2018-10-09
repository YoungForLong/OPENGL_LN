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

glm::mat4 getTransform(const float dither)
{
	glm::mat4 trans;
	trans = glm::rotate(trans, float(dither), glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	return trans;
}

int main()
{
	stbi_set_flip_vertically_on_load(true);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1080, 720, "GL_LN", NULL, NULL);

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
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	OPENGL_LN::TextureLN tln = OPENGL_LN::TextureLN();
	tln.clearTextureCache();
	tln.flushMixImgIntoBuffer({ "test.jpg", "test_head.png"});

	OPENGL_LN::Shader sr = OPENGL_LN::Shader("vertex.glsl", "fragment.glsl");

	// @param 1 :从头开始读取； 2：3个顶点； 3：顶点数据类型；4：是否归一化数据；5：数据大小； 6：第一个顶点的偏移量
	// 顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture采样点
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float deltaTime = 0;
	float curT = glfwGetTime();
	float lastT = curT;
	sr.use();
	sr.setVal("texture1", 0);
	sr.setVal("texture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// 渲染
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // 设置屏幕颜色
		glClear(GL_COLOR_BUFFER_BIT); // 清屏


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
		auto trans = getTransform(curT);
		sr.setTrans("transform", glm::value_ptr(trans));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		// dky?
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}