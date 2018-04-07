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
glm::mat4 rotateCube(glm::mat4& Model, bool check, GLfloat speed, GLfloat radians);
glm::mat4 translateCube(glm::mat4 Model, bool check, GLfloat speed, GLfloat offset);
glm::mat4 zoomCube(glm::mat4 Model, bool check, GLfloat speed, GLfloat offset);

//设置窗口大小
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 680;
// 设置摄像机
Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// 设置计算用于帧渲染的计时器
float deltaTime = 0.0f;	
float lastFrame = 0.0f;


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
	Shader cubeShader("cubeShader.vs", "cubeShader.fs");

	//设置顶点
	GLfloat cubeVertices[] = //画6个四边形,8个顶点
	{	//位置（x,y,z）		 
		-0.2f, -0.2f, -0.2f,  1.0f, 0.0f, 0.0f,
		0.2f, -0.2f, -0.2f,  1.0f, 0.0f, 0.0f,
		0.2f,  0.2f, -0.2f,  1.0f, 0.0f, 0.0f,
		0.2f,  0.2f, -0.2f,  1.0f, 0.0f, 0.0f,
		-0.2f,  0.2f, -0.2f,  1.0f, 0.0f, 0.0f,
		-0.2f, -0.2f, -0.2f,  1.0f, 0.0f, 0.0f,

		-0.2f, -0.2f,  0.2f,  1.0f, 1.0f,0.0f, 
		0.2f, -0.2f,  0.2f,  1.0f, 1.0f,0.0f,
		0.2f,  0.2f,  0.2f,  1.0f, 1.0f,0.0f,
		0.2f,  0.2f,  0.2f,  1.0f, 1.0f,0.0f,
		-0.2f,  0.2f,  0.2f,  1.0f, 1.0f,0.0f,
		-0.2f, -0.2f,  0.2f,  1.0f, 1.0f,0.0f,

		-0.2f,  0.2f,  0.2f,  0.0f, 1.0f, 0.0f,
		-0.2f,  0.2f, -0.2f,  0.0f, 1.0f, 0.0f,
		-0.2f, -0.2f, -0.2f,  0.0f, 1.0f, 0.0f,
		-0.2f, -0.2f, -0.2f,  0.0f, 1.0f, 0.0f,
		-0.2f, -0.2f,  0.2f,  0.0f, 1.0f, 0.0f,
		-0.2f,  0.2f,  0.2f,  0.0f, 1.0f, 0.0f,

		0.2f,  0.2f,  0.2f,  0.0f, 1.0f, 1.0f,
		0.2f,  0.2f, -0.2f,  0.0f, 1.0f, 1.0f,
		0.2f, -0.2f, -0.2f,  0.0f, 1.0f, 1.0f,
		0.2f, -0.2f, -0.2f,  0.0f, 1.0f, 1.0f,
		0.2f, -0.2f,  0.2f,  0.0f, 1.0f, 1.0f,
		0.2f,  0.2f,  0.2f,  0.0f, 1.0f, 1.0f,

		-0.2f, -0.2f, -0.2f,  0.0f, 0.0f,1.0f,
		0.2f, -0.2f, -0.2f,  0.0f, 0.0f,1.0f,
		0.2f, -0.2f,  0.2f,  0.0f, 0.0f,1.0f,
		0.2f, -0.2f,  0.2f,  0.0f, 0.0f,1.0f,
		-0.2f, -0.2f,  0.2f,  0.0f, 0.0f,1.0f,
		-0.2f, -0.2f, -0.2f,  0.0f, 0.0f,1.0f,

		-0.2f,  0.2f, -0.2f,  1.0f, 0.0f, 1.0f,
		0.2f,  0.2f, -0.2f,  1.0f, 0.0f, 1.0f,
		0.2f,  0.2f,  0.2f,  1.0f, 0.0f, 1.0f,
		0.2f,  0.2f,  0.2f,  1.0f, 0.0f, 1.0f,
		-0.2f,  0.2f,  0.2f,  1.0f, 0.0f, 1.0f,
		-0.2f,  0.2f, -0.2f,  1.0f, 0.0f, 1.0f
	};


	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);//创建VAO
	glGenBuffers(1, &VBO); //创建VBO	

	//--------设置立方体----------------------------------------------
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定新创建的VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // 解除当前绑定的VBO
	//顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	bool show_another_window = true;

	cubeShader.use();
	//渲染循环：它能在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window)) {

		static GLfloat proj_radians = 45.0f;
		static GLfloat scale_size = 1.0f;
		static GLfloat rotate_radians = 0.0f;
		static GLfloat translate_x = 0.0f;
		static GLfloat translate_y = 0.0f;
		static GLfloat translate_z = 0.0f;
		static glm::vec3 lookat_x = { 0,0,2 };
		static glm::vec3 lookat_y = { 0,0,0 };
		static glm::vec3 lookat_z = { 0,1,0 };
		static GLfloat ort_left = -1.4f;
		static GLfloat ort_right = 1.4f;
		static GLfloat ort_bottom = -1.0f;
		static GLfloat ort_top = 1.0f;
		static int select_view = 4;
		static GLfloat per_radians = 45.0f;
		static GLfloat per_z_near = 0.1f;
		static GLfloat per_z_far = 100.0f;
		static GLfloat ort_z_near = -100.0f;
		static GLfloat ort_z_far = 100.0f;
		static GLfloat per_ratio = (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT;

		// 每帧渲染时间
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//处理输入
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


		//----设置视角、模型、透射----------------------------
		glm::mat4 View = glm::mat4(1.0f), Projection = glm::mat4(1.0f), cubeModel = glm::mat4(1.0f);
		
		//----orthographic------------------------------------
		if (select_view == 0) {
			cubeModel = glm::mat4(1.0f);
			Projection = glm::orthoLH(ort_left, ort_right, ort_bottom, ort_top, ort_z_near, ort_z_far);
			View = glm::lookAt(glm::vec3(-1.5, 0.5, -1.5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			cubeModel = glm::translate(cubeModel, glm::vec3(1.5, -0.5f, 1.5f));
		}
		//----perspective-------------------------------------
		if (select_view == 1) {
			cubeModel = glm::mat4(1.0f);
			Projection = glm::perspective(glm::radians(per_radians), per_ratio, per_z_near, per_z_far);
			View = glm::lookAt(glm::vec3(-1.5, 0.5, -1.5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			cubeModel = glm::translate(cubeModel, glm::vec3(1.5, -0.5f, 1.5f));
		}
		//----view changing-----------------------------------
		if (select_view == 2) {
			cubeModel = glm::mat4(1.0f);
			Projection = glm::perspective(glm::radians(45.0f), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
			View = glm::lookAt(glm::vec3(sin(glfwGetTime())*2.0f, 0, cos(glfwGetTime())*2.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		}
		//----camera view-------------------------------------
		if (select_view == 3) {
			cubeModel = glm::mat4(1.0f);
			Projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			View = camera.GetViewMatrix();

		}
		cubeModel = glm::translate(cubeModel, glm::vec3(translate_x, translate_y, translate_z));
		cubeModel = glm::scale(cubeModel, glm::vec3(scale_size, scale_size, scale_size));
		cubeModel = glm::rotate(cubeModel, glm::radians(rotate_radians), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 cubeTransform = Projection * View * cubeModel;

		//绘制图元1
		glBindVertexArray(VAO);
		GLint transformID = glGetUniformLocation(cubeShader.ID, "transform");
		glUniformMatrix4fv(transformID, 1, GL_FALSE, glm::value_ptr(cubeTransform));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//创建菜单栏窗口
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Begin("Transformation", &show_another_window);
		if (ImGui::TreeNode("Select your view way")) {
			ImGui::RadioButton("Orthographic Projection", &select_view, 0); ImGui::SameLine();
			ImGui::RadioButton("Perspective Projection", &select_view, 1);
			ImGui::RadioButton("View Changing", &select_view, 2); ImGui::SameLine();
			ImGui::RadioButton("Camera View", &select_view, 3);
			ImGui::Spacing(); ImGui::Spacing();
			if (select_view == 0)
			{
				if (ImGui::TreeNode("Orthographic")) {
					ImGui::SliderFloat("Left", &ort_left, -8.0f, 8.0f);
					ImGui::SliderFloat("Right", &ort_right, -8.0f, 8.0f);
					ImGui::SliderFloat("Bottom", &ort_bottom, -8.0f, 8.0f);
					ImGui::SliderFloat("Top", &ort_top, -8.0f, 8.0f);
					ImGui::DragFloat("z-Near0", &ort_z_near, 0.5f, -100.0f, 100.0f);
					ImGui::DragFloat("z-Far0", &ort_z_far, 0.5f, -100.0f, 100.0f);
					ImGui::TreePop();
				}
			}
			if (select_view == 1)
			{
				if (ImGui::TreeNode("Perspective")) {
					ImGui::SliderFloat("Projection Angle", &per_radians, 0.0f, 360.0f);
					ImGui::SliderFloat("Perspective Aspect", &per_ratio, -5.0f, 5.0f);
					ImGui::DragFloat("z-Near1", &per_z_near, 0.5f, -100.0f, 100.0f);
					ImGui::DragFloat("z-Far1", &per_z_far, 0.5f, -100.0f, 100.0f);
					ImGui::TreePop();
				}
			}
			if (select_view == 2)
			{
				if (ImGui::TreeNode("View_Changing")) {
					ImGui::DragFloat3("Look At x", (GLfloat*)&lookat_x, 0.5f, -30.0f, 30.0f);
					ImGui::DragFloat3("Look At y", (GLfloat*)&lookat_y, 0.5f, -30.0f, 30.0f);
					ImGui::DragFloat3("Look At z", (GLfloat*)&lookat_z, 0.5f, -30.0f, 30.0f);
					ImGui::TreePop();
				}
			}
			if (select_view == 3)
			{
				if (ImGui::TreeNode("Camera_View")) {
					ImGui::SliderFloat("Projection Angle", &proj_radians, 0.0f, 360.0f);
					ImGui::DragFloat3("Look At x", (GLfloat*)&lookat_x, 0.5f, -30.0f, 30.0f);
					ImGui::DragFloat3("Look At y", (GLfloat*)&lookat_y, 0.5f, -30.0f, 30.0f);
					ImGui::DragFloat3("Look At z", (GLfloat*)&lookat_z, 0.5f, -30.0f, 30.0f);
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::Spacing();
		if (ImGui::TreeNode("Transform")) {
			ImGui::SliderFloat("Translation X", &translate_x, -3.0f, 3.0f);
			ImGui::SliderFloat("Translation Y", &translate_y, -3.0f, 3.0f);
			ImGui::SliderFloat("Translation Z", &translate_z, -3.0f, 3.0f);
			ImGui::SliderFloat("Rotation Angle", &rotate_radians, 0.0f, 360.0f);
			ImGui::SliderFloat("Scale Factor", &scale_size, 0.0f, 5.0f);
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
	glDeleteVertexArrays(1, &VAO);
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

glm::mat4 rotateCube(glm::mat4& Model, bool check, GLfloat speed, GLfloat radians) {
	if (check) {
		Model = glm::rotate(Model, glm::radians((GLfloat)glfwGetTime() * speed), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		Model = glm::rotate(Model, glm::radians(radians), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	return Model;
}

glm::mat4 zoomCube(glm::mat4 Model, bool check, GLfloat speed, GLfloat offset) {
	if (check) {
		static float s = 1.0f;
		static float f = speed;
		s = s + f;
		if (s < 0) f = -f;
		if (s > 5) f = -f; //持续缩放
		Model = glm::scale(Model, glm::vec3(s, s, s));//缩放
	}
	else {
		Model = glm::scale(Model, glm::vec3(offset, offset, offset));//缩放
	}
	return Model;
}

glm::mat4 translateCube(glm::mat4 Model, bool check, GLfloat speed, GLfloat offset) {
	if (check) {
		static float x = 0;
		static float h = speed;
		x = x + h;
		if (x < -2) h = -h;
		if (x > 2) h = -h; //左右平移
		Model = glm::translate(Model, glm::vec3(x, 0.0f, 0.0f));
	}
	else {
		Model = glm::translate(Model, glm::vec3(offset, 0.0f, 0.0f));
	}
	return Model;
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