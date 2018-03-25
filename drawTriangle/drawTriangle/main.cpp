#include <iostream>
#include <windows.h>
#include <glad/glad.h>
#include <glew/glfw3.h>
#include <glm/glm.hpp>
#include "imgui/imgui.h"
#include "imgui_glfw3.h"
#include "Shader.h"

using glm::vec3;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
static void glfw_error_callback(int error, const char* description);
void changVertexColor(vec3*, vec3*,int);
void renderTriangle();
void renderPoint();
void renderLine();

unsigned int VBO, VAO, EBO;

//设置窗口大小
const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

int main() {
	glfwSetErrorCallback(glfw_error_callback);
	//调用glfwInit函数来初始化GLFW
	if (!glfwInit()) {
		return -1;
	}
	//glfwWindowHint函数来配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//核心模式

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //如果使用的是Mac OS X系统，可将上面的代码解除注释
#endif
	
	//创建一个窗口对象，这个窗口对象存放了所有和窗口相关的数据，而且会被GLFW的其他函数频繁地用到。
	GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//我们还需要注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 建立ImGui绑定
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	//创建一个着色器对象
	Shader triangleShader("triangle.vs","triangle.fs");

	// 设置顶点
	vec3 vertices[] = 
	{	//位置				  //颜色
		{0.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f},  
		{-0.8f,0.0f,0.0f},{0.0f,1.0f,0.0f},  
		{-0.4f,0.4f,0.0f},{1.0f,0.0f,0.0f}, //第一个三角形 
		{0.0f,-0.2f,0.0f },{0.0f,0.0f,1.0f},
		{-0.8f,-0.2f,0.0f}, {0.0f,1.0f,0.0f},
		{-0.4f,-0.6f,0.0f },{1.0f,0.0f,0.0f},//第二个三角形
		{0.2f,0.4f,0.0f},  {1.0f, 1.0f,0.0f}, //第一个点
		{0.4f,0.4f,0.0f },{ 0.0f, 1.0f,1.0f}, //第二个点
		{0.2f,-0.4f,0.0f}, {1.0f, 0.0f,1.0f},
		{0.8f,-0.4f,0.0f },{ 0.0f, 1.0f,1.0f} //直线
	};

	unsigned int indices[] = {  
		0, 1, 2,  // 第一个三角形的顶点索引
		3, 4, 5	  // 第二个三角形的顶点索引
	};

	//生成一个VBO(顶点缓冲对象)
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	// 绑定VAO
	glBindVertexArray(VAO);

	//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上，把之前定义的顶点数据复制到缓冲的内存中
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	//顶点位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//顶点颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//设置起始顶点颜色
	vec3 col[] = {
		{ 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.00f},
		{ 1.0f, 0.0f, 0.0f}
	};

	bool show_another_window = true;

	//渲染循环：它能在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window)) {
		
		//当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//通过调用glClear函数来清空屏幕的颜色缓冲
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(triangleShader.ID);

		//绘制三角形
		renderTriangle();
		//绘制点
		renderPoint();
		//绘制线条
		renderLine();

		static vec3 tmp_color;
		static bool topFlag=false, leftFlag=false, rightFlag=false;

		//创建菜单栏窗口
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Begin("Triangle Vertex Color Editer", &show_another_window);
		if (ImGui::BeginMenu("choose color")) {
			ImGui::MenuItem("Top", NULL, &topFlag);
			ImGui::MenuItem("Left", NULL, &rightFlag);
			ImGui::MenuItem("Right", NULL, &leftFlag);
			ImGui::EndMenu();
		}
		ImGui::ColorEdit3("pick color", (float*)&tmp_color);
		if (topFlag == true) { col[2] = tmp_color; }
		if (leftFlag == true) { col[1] = tmp_color; }
		if (rightFlag == true) { col[0] = tmp_color; }
		ImGui::End();

		//改变颜色，更新缓冲
		changVertexColor(vertices, col,3);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*6, vertices);
		//渲染，显示窗口
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		// 检查并调用事件，交换缓冲
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void renderTriangle() {
	//绘制三角形
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void renderPoint() {
	//设置点的大小
	glPointSize(10);  //设置点的大小为10个像素
	glDrawArrays(GL_POINTS, 6, 2);
}

void renderLine() {
	glLineWidth(5);
	glDrawArrays(GL_LINES, 8, 2);
}

//处理所有输入：将会返回这个按键是否正在被按下，并作相应反应
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void changVertexColor(vec3* vertices, vec3* colors, int num) {
	for (int i = 0; i < num; i++) {
		vertices[i * 2 + 1] = colors[i];
	}
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}
