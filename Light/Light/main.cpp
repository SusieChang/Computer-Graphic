#include <iostream>
#include <glad/glad.h>
#include <glew/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "Shader.h"
#include "Camera.h"

using glm::vec3;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void glfw_error_callback(int error, const char* description);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

//设置窗口大小
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 680;
// 设置摄像机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// 设置计算用于帧渲染的计时器
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.5f, 2.0f);

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Transformation", NULL, NULL);
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

	// GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	// 建立ImGui绑定
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	//创建着色器对象
	Shader pongShader("pongShader.vs", "pongShader.fs");
	Shader gouraudShader("gouraudShader.vs", "gouraudShader.fs");
	Shader lightShader("lampShader.vs", "lampShader.fs");

	//设置顶点
	GLfloat cubeVertices[] = //画6个四边形,8个顶点
	{	//位置（x,y,z）		 
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
	glGenVertexArrays(1, &objectVAO);//创建VAO
	glGenVertexArrays(1, &lightVAO);//创建VAO
	glGenBuffers(1, &VBO); //创建VBO	

	glBindVertexArray(objectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定新创建的VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);
	//顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定新创建的VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);
	//顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	bool show_another_window = true;

	//渲染循环：它能在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window)) {
		static int shader_flag = 0;
		static bool is_trans = true;

		static float ambientStrength=0.1;
		static float specularStrength=0.2;
		static int shininess=32;
		static float diffuse=1.0;

		static bool is_rotate = false;
		static float scale_factor = 1.0;
		static float trans_factor[] = { 0,0,0 };

		// 每帧渲染时间
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//处理输入
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 View = glm::mat4(1.0f), Projection = glm::mat4(1.0f), cubeModel = glm::mat4(1.0f);
		GLint transformID;
		glm::mat4 cubeTransform, lightTransform;
		
		if (is_trans) {
			static float x = 0;
			static float h = 0.002;
			x = x + h;
			if (x < -1.5) h = -h;
			if (x > 1.5) h = -h; //左右平移
			lightPos.x = x;
		}
		//-----物体-------------------------------------------
		glBindVertexArray(objectVAO);
		//----设置视角、模型、透射----------------------------
		cubeModel = glm::mat4(1.0f);
		//Projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//View = camera.GetViewMatrix();
		cubeModel = glm::mat4(1.0f);
		Projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		View = glm::lookAt(glm::vec3(0, 2, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		cubeModel = glm::translate(cubeModel,glm::vec3(trans_factor[0],trans_factor[1],trans_factor[2]));
		cubeModel = glm::scale(cubeModel, glm::vec3(scale_factor, scale_factor, scale_factor));
		if (is_rotate) {
			cubeModel = glm::rotate(cubeModel, glm::radians((GLfloat)glfwGetTime() * 60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else {
			cubeModel = glm::rotate(cubeModel, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (shader_flag == 0) {
			pongShader.use();
			pongShader.setMat4("projection", Projection);
			pongShader.setMat4("view", View);
			pongShader.setMat4("model", cubeModel);
			pongShader.setVec3("objectColor", 1.0f,0.3f,0.5f);
			pongShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			pongShader.setVec3("lightPos", lightPos);
			pongShader.setVec3("viewPos", glm::vec3(0, 2, 3));
			pongShader.setFloat("ambientStrength", ambientStrength);
			pongShader.setFloat("specularStrength", specularStrength);
			pongShader.setFloat("diff_factor", diffuse);
			pongShader.setInt("shininess", shininess);
		}
		if (shader_flag == 1) {
			gouraudShader.use();
			gouraudShader.setMat4("projection", Projection);
			gouraudShader.setMat4("view", View);
			gouraudShader.setMat4("model", cubeModel);
			gouraudShader.setVec3("objectColor", 1.0f, 0.3f, 0.5f);
			gouraudShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			gouraudShader.setVec3("lightPos", lightPos);
			gouraudShader.setVec3("viewPos", glm::vec3(0, 2, 3));
			gouraudShader.setFloat("ambientStrength", ambientStrength);
			gouraudShader.setFloat("specularStrength", specularStrength);
			gouraudShader.setFloat("diff_factor", diffuse);
			gouraudShader.setInt("shininess", shininess);
		}

		glDrawArrays(GL_TRIANGLES, 0, 36);


		//----光源-------------------------------------------
		glBindVertexArray(lightVAO);
		lightShader.use();
		//----设置视角、模型、透射----------------------------
		cubeModel = glm::mat4(1.0f);
		cubeModel = glm::translate(cubeModel, lightPos);
		cubeModel = glm::scale(cubeModel, glm::vec3(0.05, 0.05, 0.05));
		/*cubeModel = glm::rotate(cubeModel, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/
		lightTransform = Projection * View * cubeModel;
		//----颜色--------------------------------------------
		glm::vec3 lightColor = vec3(1.0f,1.0f,1.0f);
		int lightColorLocation = glGetUniformLocation(lightShader.ID, "ourColor");
		glUniform4f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2], 1.0f);
		//----变换--------------------------------------------
		lightShader.setMat4("projection", Projection);
		lightShader.setMat4("view", View);
		lightShader.setMat4("model", cubeModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//创建菜单栏窗口
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Begin("edit", &show_another_window);
		if(ImGui::TreeNode("change lamp")) {
			ImGui::Checkbox("translate?", &is_trans);
			ImGui::RadioButton("Pong", &shader_flag, 0);
			ImGui::RadioButton("Gouraud", &shader_flag, 1);
			ImGui::SliderFloat("ambient factor", &ambientStrength,0, 1);
			ImGui::SliderFloat("specular factor", &specularStrength,0, 1);
			ImGui::SliderFloat("diffuse factor", &diffuse,0, 1);
			ImGui::SliderInt("shininess factor", &shininess,0, 256);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("change object")) {
			ImGui::Checkbox("rotation", &is_rotate);
			ImGui::SliderFloat("scale factor", &scale_factor, 0, 5);
			ImGui::DragFloat3("translate(x,y,z)", trans_factor, 0.05f,  -2, 2);
			ImGui::TreePop();
		}
		ImGui::End();

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
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &objectVAO);
	glDeleteBuffers(1, &VBO);
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}


//每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理
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