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

//���ô��ڴ�С
const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

int main() {
	glfwSetErrorCallback(glfw_error_callback);
	//����glfwInit��������ʼ��GLFW
	if (!glfwInit()) {
		return -1;
	}
	//glfwWindowHint����������GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//����ģʽ

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //���ʹ�õ���Mac OS Xϵͳ���ɽ�����Ĵ�����ע��
#endif
	
	//����һ�����ڶ���������ڶ����������кʹ�����ص����ݣ����һᱻGLFW����������Ƶ�����õ���
	GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//���ǻ���Ҫע���������������GLFW����ϣ��ÿ�����ڵ�����С��ʱ������������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ����ImGui��
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	//����һ����ɫ������
	Shader triangleShader("triangle.vs","triangle.fs");

	// ���ö���
	vec3 vertices[] = 
	{	//λ��				  //��ɫ
		{0.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f},  
		{-0.8f,0.0f,0.0f},{0.0f,1.0f,0.0f},  
		{-0.4f,0.4f,0.0f},{1.0f,0.0f,0.0f}, //��һ�������� 
		{0.0f,-0.2f,0.0f },{0.0f,0.0f,1.0f},
		{-0.8f,-0.2f,0.0f}, {0.0f,1.0f,0.0f},
		{-0.4f,-0.6f,0.0f },{1.0f,0.0f,0.0f},//�ڶ���������
		{0.2f,0.4f,0.0f},  {1.0f, 1.0f,0.0f}, //��һ����
		{0.4f,0.4f,0.0f },{ 0.0f, 1.0f,1.0f}, //�ڶ�����
		{0.2f,-0.4f,0.0f}, {1.0f, 0.0f,1.0f},
		{0.8f,-0.4f,0.0f },{ 0.0f, 1.0f,1.0f} //ֱ��
	};

	unsigned int indices[] = {  
		0, 1, 2,  // ��һ�������εĶ�������
		3, 4, 5	  // �ڶ��������εĶ�������
	};

	//����һ��VBO(���㻺�����)
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	// ��VAO
	glBindVertexArray(VAO);

	//���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ���ϣ���֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	//����λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//������ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//������ʼ������ɫ
	vec3 col[] = {
		{ 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.00f},
		{ 1.0f, 0.0f, 0.0f}
	};

	bool show_another_window = true;

	//��Ⱦѭ����������������GLFW�˳�ǰһֱ��������
	while (!glfwWindowShouldClose(window)) {
		
		//������glClear�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//ͨ������glClear�����������Ļ����ɫ����
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(triangleShader.ID);

		//����������
		renderTriangle();
		//���Ƶ�
		renderPoint();
		//��������
		renderLine();

		static vec3 tmp_color;
		static bool topFlag=false, leftFlag=false, rightFlag=false;

		//�����˵�������
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

		//�ı���ɫ�����»���
		changVertexColor(vertices, col,3);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*6, vertices);
		//��Ⱦ����ʾ����
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		// ��鲢�����¼�����������
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void renderTriangle() {
	//����������
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void renderPoint() {
	//���õ�Ĵ�С
	glPointSize(10);  //���õ�Ĵ�СΪ10������
	glDrawArrays(GL_POINTS, 6, 2);
}

void renderLine() {
	glLineWidth(5);
	glDrawArrays(GL_LINES, 8, 2);
}

//�����������룺���᷵����������Ƿ����ڱ����£�������Ӧ��Ӧ
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//ÿ�����ڸı��С��GLFW�������������������Ӧ�Ĳ������㴦��
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
