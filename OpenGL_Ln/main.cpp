#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

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

void drawTriangleDemo()
{
	const GLchar* vertexShaderSource = getFileData("vertics.glsl");
	const GLchar* fragmentShaderSource = getFileData("fragment.glsl");

	float vertics[] = {
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO); // ����һ������ID

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ��Ϊ�����������,֮��ͨ���������ƻ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertics), vertics, GL_STATIC_DRAW);

	// @param 1 :��ͷ��ʼ��ȡ�� 2��3�����㣻 3�������������ͣ�4���Ƿ��һ�����ݣ�5�����ݴ�С�� 6����һ�������ƫ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLint succ;
	
	// ����������ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{
		char info[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		std::cout << "Err: SHADER::VERTEX::COMPILATION_FAILED\n" << info << std::endl;
	}

	// ����ƬԪ��ɫ��
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{	
		char info[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		std::cout << "Err: SHADER::FRAGMENT::COMPILATION_FAILED\n" << info << std::endl;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &succ);
	if (!succ)
	{
		char info[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, info);
		std::cout << "Err: PROGRAM::SHADER::LINK_FAILED\n" << info << std::endl;
	}

	glUseProgram(shaderProgram);
	
	/*float timeValue = glfwGetTime();
	float greenVal = sin(timeValue) / 2.0f + 0.5f;
	int fragmentColor = glGetUniformLocation(shaderProgram, "pn");
	glUniform4f(fragmentColor, 0.0f, greenVal, 0.0f, 1.0f);*/

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1080, 720, "GL_LN", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Err: Failed to create window" << std::endl;
		glfwTerminate(); // ��ֹ����
		return -1;
	}

	// �����ڵ��������л�Ϊ��ǰ���߳�������
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Err: Failed to init glad" << std::endl;
		return -1;
	}

	// ע�ᴰ�ڻص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float deltaTime = 0;
	float curT = glfwGetTime();
	float lastT = curT;
	while (!glfwWindowShouldClose(window))
	{
		curT = glfwGetTime();
		float deltaTime = curT - lastT;
		lastT = curT;
		if (deltaTime < DEFAULT_DELTA)
		{
			Sleep((DEFAULT_DELTA - deltaTime) * 1000);
		}
		// std::cout << deltaTime << std::endl;

		processInput(window);

		//��Ⱦ
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // ������Ļ��ɫ
		glClear(GL_COLOR_BUFFER_BIT); //����

		drawTriangleDemo();

		// dky?
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}