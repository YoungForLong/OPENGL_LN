#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "stb_image.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "EventRegisterMng.h"
#include "Light.h"
#include <string>
#include <time.h>
#include "ModelLoader.h"
#include "Model.h"
#include "IOUtils.h"
#include "TextureLoader.h"

#ifdef linux
#ifndef MAX_PATH
#define MAX_PATH 260
#endif // !MAX_PATH
#endif // linux

#define FPS 30
#define DEFAULT_DELTA 0.0333f
#define ORIGIN_VEC_3 glm::vec3(0.0f, 0.0f, 0.0f)
#define GET_KEY_VOID_VAL(__key) static_cast<void*>(&__key)


void framebuffer_size_callback(GLFWwindow* window, int w, int h) 
{
	glViewport(0, 0, w, h);
}

void mouseCallBack(GLFWwindow* winddow, double xpos, double ypos)
{
	double pos[] = { xpos, ypos };
	EventRegisterMng::instance()->dispatchEvent(EventTypes::EVENT_MOUSE, GET_KEY_VOID_VAL(pos));
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		int key = GLFW_KEY_W;
		EventRegisterMng::instance()->dispatchEvent(EventTypes::EVENT_KEYBOARD, GET_KEY_VOID_VAL(key));
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		int key = GLFW_KEY_S;
		EventRegisterMng::instance()->dispatchEvent(EventTypes::EVENT_KEYBOARD, GET_KEY_VOID_VAL(key));
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		int key = GLFW_KEY_A;
		EventRegisterMng::instance()->dispatchEvent(EventTypes::EVENT_KEYBOARD, GET_KEY_VOID_VAL(key));
	}
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		int key = GLFW_KEY_D;
		EventRegisterMng::instance()->dispatchEvent(EventTypes::EVENT_KEYBOARD, GET_KEY_VOID_VAL(key));
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

glm::mat4 localTransform(const glm::vec3& offset, const float dither)
{
	// local
	glm::mat4 model;
	model = glm::translate(model, offset);
	model = glm::rotate(model, dither * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	return model;
}

glm::mat4 projectionTransform()
{
	// projection
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	return projection;
}

glm::vec4 origin(0.0f, 0.0f, 10.0f, 0.0f);
glm::vec3 genCameraPos()
{
	auto t = glfwGetTime();
	return glm::vec3(sin(t) * 10, 0.0f, cos(t) * 10);
}

int main()
{
	srand(time(0));

	stbi_set_flip_vertically_on_load(true);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(720, 720, "GL_LN", NULL, NULL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallBack);

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

	/*float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f
	};*/
	
	/*unsigned int indices[] = {
		0, 1, 3,
		1, 0, 2,
		1, 2, 3,
		2, 0, 3
	};*/

	/*unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// normal vec
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//// texture coord attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	OPENGL_LN::Shader sr = OPENGL_LN::Shader("vertex.glsl", "fragment.glsl");

	float deltaTime = 0;
	float curT = glfwGetTime();
	float lastT = curT;

	sr.use();

	OPENGL_LN::Camera ca(glm::vec3(0, 0, 6));
	
	glEnable(GL_DEPTH_TEST);

	std::vector<glm::vec3> offsetArr;
	for (int i = 0; i < 1; ++i)
	{
		offsetArr.push_back(glm::vec3(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5) / 2.3f);
	}

	OPENGL_LN::Light sunLt("sunLight",
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(2, 2, 2)
	);
	sunLt.applyLight(sr);

	OPENGL_LN::Light pointLt("pointLight",
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(4.0f, 4.0f, 4.0f),
		0.0f,
		0.09f,
		0.0032f
	);
	pointLt.applyLight(sr);

	unsigned int modelId = MODELMNG->initOneObj("nanosuit.obj");
	
	auto beginT = glfwGetTime();
	//simplely wait loading finished
	while (true)
	{
		curT = glfwGetTime();
		if (curT - beginT > 15)
			break;

		if (MODELMNG->hasLoadFinished() && TEXTUREMNG->hasLoadFinished())
			break;
	}

	auto model = MODELMNG->getObjbyKey(modelId);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// 渲染
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // 设置屏幕颜色
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
		ca.tick(deltaTime);
		sr.use();
		sr.setVal("time", curT);

		auto view = ca.getCurTrans();
		auto projection = projectionTransform();

		// 光照
		/*OPENGL_LN::Light lt(
			"light",
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			ca.getPos(),
			ca.getCameraLookAt(),
			glm::cos(glm::radians(12.5f)),
			glm::cos(glm::radians(17.5f))
		);*/
		/*OPENGL_LN::Light lt("spotLight",
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(2, 2, 2)
		);
		lt.applyLight(sr);*/

		/*auto caPos = ca.getPos();
		sr.setVal("viewPos", caPos);*/

		auto modelTrans = localTransform(offsetArr[0], curT);
		sr.setTrans("model", glm::value_ptr(modelTrans));
		sr.setTrans("view", glm::value_ptr(view));
		sr.setTrans("projection", glm::value_ptr(projection));

		model->render(&sr);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}