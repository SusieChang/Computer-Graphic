//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <stb_image.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw_gl3.h"
//
//#include "Shader.h"
//
//#include <stdio.h>  
//#include <stdlib.h>  
//#include <string.h>  
//#include <iostream>
//
//using namespace std;
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//void cursor_position_callback(GLFWwindow* window, double x, double y);
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//static void glfw_error_callback(int error, const char* description);
//void renderCube(Shader&);
//void renderQuad(Shader&);
//void keyboard(unsigned char key, int i, int j);
//
//// settings
//const unsigned int SCR_WIDTH = 960;
//const unsigned int SCR_HEIGHT = 680;
//
//int num = 1;
//int flag = 0;
//static char msg[128] = { 0 };
//GLfloat xpos, ypos;
//unsigned int planeVAO, cubeVAO, planeVBO, cubeVBO;
//
//GLfloat C[4][2] =
//{
//	{ -1.0f, 0.1f },{ -0.33f, 0.8f },{ 0.6f, -0.3f },{ 0.9f,0.7f }
//};
//
//GLfloat control[4][2] =
//{
//	{ -0.9f, 0.1f },{ -0.8f, 0.6f },{ 0.2f, 0.8f },{ 1.0f, 0.2f }
//};
//
//
//int main()
//{
//	// glfw: initialize and configure
//	// ------------------------------
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
//#endif
//	glfwSetErrorCallback(glfw_error_callback);
//														 // glfw window creation
//														 // --------------------
//	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//	if (window == NULL)
//	{
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//	glfwSetKeyCallback(window, key_callback);
//	glfwSetMouseButtonCallback(window, mouse_button_callback);
//	glfwSetCursorPosCallback(window, cursor_position_callback);
//
//	// tell GLFW to capture our mouse
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//	// glad: load all OpenGL function pointers
//	// ---------------------------------------
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//
//	// 建立ImGui绑定
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	ImGui_ImplGlfwGL3_Init(window, true);
//	ImGui::StyleColorsDark();
//
//
//
//	while (!glfwWindowShouldClose(window))
//	{
//		// per-frame time logic
//		float currentFrame = glfwGetTime();
//
//		// render
//		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//		// Get mouse position
//		int xpos, ypos;
//
//
//		glfwPollEvents();
//		ImGui_ImplGlfwGL3_NewFrame();
//		ImGui::Begin("color");
//		ImGui::End();
//		//渲染，显示窗口
//		int display_w, display_h;
//		glfwGetFramebufferSize(window, &display_w, &display_h);
//		glViewport(0, 0, display_w, display_h);
//		ImGui::Render();
//		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//	ImGui_ImplGlfwGL3_Shutdown();
//	ImGui::DestroyContext();
//
//	glfwTerminate();
//	return 0;
//}
//
////处理所有输入：将会返回这个按键是否正在被按下，并作相应反应
//void processInput(GLFWwindow *window) {
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//}
//
////每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//	glViewport(0, 0, width, height);
//}
//
//static void glfw_error_callback(int error, const char* description)
//{
//	fprintf(stderr, "Error %d: %s\n", error, description);
//}
//
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (action != GLFW_PRESS)
//		return;
//	switch (key)
//	{
//	case GLFW_KEY_ESCAPE:
//		glfwSetWindowShouldClose(window, GL_TRUE);
//		break;
//	default:
//		break;
//	}
//}
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
//{
//	if (GLFW_PRESS == action) switch (button)
//	{
//	case GLFW_MOUSE_BUTTON_LEFT:
//		cout << "Mosue left button clicked!" << endl;;
//		break;
//	case GLFW_MOUSE_BUTTON_MIDDLE:
//		cout<<"Mosue middle button clicked!" << endl;;
//		break;
//	case GLFW_MOUSE_BUTTON_RIGHT:
//		cout << "Mosue right button clicked!" << endl;;
//		break;
//	default:
//		cout << "Mosue button clicked!" << endl;;
//		return;
//	}
//	return;
//}
//void cursor_position_callback(GLFWwindow* window, double x, double y)
//{
//	//cout << "Mouse position move to [" << int(x) << ","<< int(y) <<"]"<< endl;;
//	xpos = float((x - SCR_WIDTH / 2) / SCR_WIDTH) ;
//	ypos = float(0 - (y - SCR_HEIGHT / 2) / SCR_HEIGHT);
//	return;
//}
//
//
////键盘事件响应
//void keyboard(unsigned char key, int i, int j)
//{
//	if (key == '0')
//	{
//		flag = 0;
//		cout << "flag: " << flag << endl;
//	}
//	else if (key == '1')
//	{
//		flag = 1;
//		cout << "flag: " << flag << endl;
//	}
//	else if (key == '2')
//	{
//		flag = 2;
//		cout << "flag: " << flag << endl;
//	}
//	else if (key == '3')
//	{
//		flag = 3;
//		cout << "flag: " << flag << endl;
//	}
//}
//
////检测鼠标移动消息
//void motion(int x, int y)
//{
//	num = 0;
//	//将屏幕坐标转化为画图时的坐标
//	float xx = x / (SCR_WIDTH/2) - 1.0f;
//	float yy = -(y / (SCR_HEIGHT/2) - 1.0f);
//	//控制台输出位置信息
//	cout << "X: " << xx << "Y: " << yy << endl;
//	if (flag == 0)
//	{
//		control[0][0] = xx;
//		control[0][1] = yy;
//
//		C[0][0] = xx;
//		C[0][1] = yy;
//	}
//	else if (flag == 1)
//	{
//		control[1][0] = xx;
//		control[1][1] = yy;
//
//		C[1][0] = xx;
//		C[1][1] = yy;
//	}
//	else if (flag == 2)
//	{
//		control[2][0] = xx;
//		control[2][1] = yy;
//
//
//		C[2][0] = xx;
//		C[2][1] = yy;
//	}
//	else if (flag == 3)
//	{
//		control[3][0] = xx;
//		control[3][1] = yy;
//
//
//		C[3][0] = xx;
//		C[3][1] = yy;
//	}
//}
//
////void motion_three_bezier()
////{
////
////	glClear(GL_COLOR_BUFFER_BIT);
////	glLineWidth(3);
////	//绘制出控制点
////	glPointSize(6);//控制点大小设为4
////	for (int i = 0; i < 4; i++) {
////		glBegin(GL_POINTS);
////		glVertex2fv(&C[i][0]);
////		glEnd();
////	}
////
////	//画出控制点之间的连线
////	glColor3f(0.0, 1.0, 1.0);
////	glBegin(GL_LINE_STRIP);
////	for (int i = 0; i < 4; i++) {
////		glVertex2fv(&C[i][0]);
////	}
////	glEnd();
////
////
////	//计算三阶
////	GLfloat three_order[3][10001][2];
////	int u = 0;
////	for (double t = 0.0; t <= 1.0; t += 0.0001)
////	{
////		for (int i = 0; i < 3; i++)
////		{
////			three_order[i][u][0] = C[i][0] * (1 - t) + C[i + 1][0] * t;
////			three_order[i][u][1] = C[i][1] * (1 - t) + C[i + 1][1] * t;
////
////		}
////		u++;
////	}
////	u = 0;
////	GLfloat two_order[2][10001][2];
////	for (double t = 0.0; t <= 1.0; t += 0.0001)
////	{
////		for (int i = 0; i < 2; i++)
////		{
////			two_order[i][u][0] = three_order[i][u][0] * (1 - t) + three_order[i + 1][u][0] * t;
////			two_order[i][u][1] = three_order[i][u][1] * (1 - t) + three_order[i + 1][u][1] * t;
////		}
////		u++;
////	}
////
////	u = 0;
////	GLfloat result[10001][2];
////	for (double t = 0.0; t <= 1.0; t += 0.0001)
////	{
////		result[u][0] = two_order[0][u][0] * (1 - t) + two_order[1][u][0] * t;
////		result[u][1] = two_order[0][u][1] * (1 - t) + two_order[1][u][1] * t;
////		u++;
////	}
////
////	glColor3f(0.0, 1.0, 1.0);
////	for (int i = 0; i < 2; i++)
////	{
////		glColor3f(1.0, 1.0, 0.0);//颜色设为黄色
////		glBegin(GL_LINE_STRIP);
////		glVertex2fv(three_order[i][num]);
////		glVertex2fv(three_order[i + 1][num]);
////		glEnd();
////
////		glColor3f(1.0, 0.0, 0.0);//颜色设为红色
////		glBegin(GL_POINTS);
////		for (int i = 0; i < 3; i++)
////			glVertex2fv(three_order[i][num]);
////		glEnd();
////	}
////
////	glColor3f(0.0, 0.0, 1.0);//颜色设为蓝色
////	glBegin(GL_LINE_STRIP);
////	glVertex2fv(two_order[0][num]);
////	glVertex2fv(two_order[1][num]);
////	glEnd();
////	glColor3f(1.0, 0.0, 0.0);//颜色设为红色
////	glBegin(GL_POINTS);
////	for (int i = 0; i < 2; i++)
////		glVertex2fv(two_order[i][num]);
////	glEnd();
////
////	glColor3f(99.0, 18.0, 200.0);
////	glBegin(GL_LINE_STRIP);
////	for (int i = 0; i<num; i++)
////		glVertex2fv(result[i]);
////	glEnd();
////
////	glFlush();
////	glutSwapBuffers();
////
////}
