#include <string>
#include <glad/glad.h>
#include <glew/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "Shader.h"
#include "Camera.h"
#include <iostream>
#include <stb_image.h>

using glm::vec3;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void glfw_error_callback(int error, const char* description);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
//GLuint loadTexture(GLchar* path);

//���ô��ڴ�С
const unsigned int SCR_WIDTH = 980;
const unsigned int SCR_HEIGHT = 680;
// ���������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// ���ü�������֡��Ⱦ�ļ�ʱ��
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.5f, 2.0f);

int main() {
	glfwSetErrorCallback(glfw_error_callback);
	//����glfwInit��������ʼ��GLFW
	if (!glfwInit()) {
		return -1;
	}
	//glfwWindowHint����������GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //���ʹ�õ���Mac OS Xϵͳ���ɽ�����Ĵ�����ע��
#endif

														 //����һ�����ڶ���������ڶ����������кʹ�����ص����ݣ����һᱻGLFW����������Ƶ�����õ���
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ShaderMapping", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	//	std::cout << "Failed to initialize GLAD" << std::endl;
	//	return -1;
	//}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// ����ImGui��
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	glEnable(GL_DEPTH_TEST);
	// ����ImGui��

	//������ɫ������
	Shader pongShader("lightShader.vs", "lightShader.fs");
	Shader lampShader("lampShader.vs", "lampShader.fs");
	Shader planeShader("planeShader.vs", "planeShader.fs");

	//���ö���
	GLfloat cubeVertices[] = //��6���ı���,8������
	{	//λ�ã�x,y,z��		 
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO, objectVAO, lightVAO;
	glGenVertexArrays(1, &objectVAO);//����VAO
	glGenVertexArrays(1, &lightVAO);//����VAO
	glGenBuffers(1, &VBO); //����VBO	

	glBindVertexArray(objectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ���´�����VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);
	//��������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ���´�����VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);
	//��������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	float planeVertices[] = {
		// positions          // colors           // texture coords
		0.5f, -0.5f, 0.5f,   0.467f, 0.686f, 0.612f,  // 1
		-0.5f, -0.5f, 0.5f,   0.467f, 0.686f, 0.612f,  // 2
		-0.5f,  -0.5f, -0.5f,  0.467f, 0.686f, 0.612f,  // 3
		0.5f,  -0.5f, -0.5f,   0.467f, 0.686f, 0.612f,    // 0
		0.5f, -0.5f, 0.5f,   0.467f, 0.686f, 0.612f,  // 1
		-0.5f,  -0.5f, -0.5f,   0.467f, 0.686f, 0.612f,  // 3

	};
	unsigned int planeVBO, planeVAO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//��Ⱦѭ����������������GLFW�˳�ǰһֱ��������
	while (!glfwWindowShouldClose(window)) {
		static bool is_trans = false;

		// ÿ֡��Ⱦʱ��
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		if (is_trans) {
			static float x = 0;
			static float h = 0.002;
			x = x + h;
			if (x < -1.5) h = -h;
			if (x > 1.5) h = -h; //����ƽ��
			lightPos.x = x;
		}


		//��������
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 View = glm::mat4(1.0f), Projection = glm::mat4(1.0f), cubeModel = glm::mat4(1.0f);
		GLint transformID;
		glm::mat4 cubeTransform, lightTransform;

		//-----����-------------------------------------------
		glBindVertexArray(objectVAO);
		//----�����ӽǡ�ģ�͡�͸��----------------------------
		cubeModel = glm::mat4(1.0f);
		//Projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//View = camera.GetViewMatrix();
		cubeModel = glm::mat4(1.0f);
		Projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		View = glm::lookAt(glm::vec3(0, 2, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		cubeModel = glm::scale(cubeModel, glm::vec3(0.5, 0.5, 0.5));
		cubeModel = glm::rotate(cubeModel, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		pongShader.use();
		pongShader.setMat4("projection", Projection);
		pongShader.setMat4("view", View);
		pongShader.setMat4("model", cubeModel);
		pongShader.setVec3("objectColor", 1.0f, 0.3f, 0.5f);
		pongShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		pongShader.setVec3("lightPos", lightPos);
		pongShader.setVec3("viewPos", glm::vec3(0, 2, 4));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		//----��Դ-------------------------------------------
		glBindVertexArray(lightVAO);
		lampShader.use();
		//----�����ӽǡ�ģ�͡�͸��----------------------------
		cubeModel = glm::mat4(1.0f);
		cubeModel = glm::translate(cubeModel, lightPos);
		cubeModel = glm::scale(cubeModel, glm::vec3(0.05, 0.05, 0.05));
		/*cubeModel = glm::rotate(cubeModel, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/
		lightTransform = Projection * View * cubeModel;
		//----��ɫ--------------------------------------------
		glm::vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
		int lightColorLocation = glGetUniformLocation(lampShader.ID, "ourColor");
		glUniform4f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2], 1.0f);
		//----�任--------------------------------------------
		lampShader.setMat4("projection", Projection);
		lampShader.setMat4("view", View);
		lampShader.setMat4("model", cubeModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		static float trans_x[] = { 0,0,0 };
		//ƽ��
		planeShader.use();
		glm::mat4 planeModel = glm::mat4(1.0),planeProj = glm::mat4(1.0), planeView = glm::mat4(1.0);
		planeModel = glm::scale(planeModel, glm::vec3(5, 1, 5));
		planeModel = glm::translate(planeModel, glm::vec3(trans_x[0], trans_x[1], trans_x[2]));
		planeModel = glm::rotate(planeModel, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glBindVertexArray(planeVAO);
		planeShader.setMat4("projection", Projection);
		planeShader.setMat4("view", View);
		planeShader.setMat4("model", planeModel);
		glDrawArrays(GL_TRIANGLES,0,6);

		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Begin("test");
		//ImGui::DragFloat3("translate(x,y,z)", trans_x, 0.05f, -10, 10);
		ImGui::Checkbox("is_trans", &is_trans);
		ImGui::End();

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
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &objectVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &planeVBO);
	glfwTerminate();
	return 0;
}


//ÿ�����ڸı��С��GLFW�������������������Ӧ�Ĳ������㴦��
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
