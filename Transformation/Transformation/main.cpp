#include <iostream>
#include <windows.h>
#include <glad/glad.h>
#include <glew/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "Shader.h"
using glm::vec3;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void glfw_error_callback(int error, const char* description);
//设置窗口大小
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 640;
unsigned int cubeVBO, cubeVAO, cubeIBO;

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
	Shader cubeShader("shader.vs", "shader.fs");

	// 设置顶点
	GLfloat cubeVertices[] = //画6个四边形,8个顶点
	{	//位置（x,y,z）		 
		-0.4f,0.4f, 0.4f, 1.0f,1.0f,0.0f,//0
		0.4f, 0.4f, 0.4f, 1.0f,1.0f,0.0f,//1
		0.4f,-0.4f, 0.4f, 0.0f,1.0f,1.0f,//2
		-0.4f,-0.4f,0.4f, 0.0f,1.0f,1.0f,//3
		-0.4f,0.4f,-0.4f, 1.0f,0.0f,1.0f,//4
		0.4f, 0.4f, -0.4f, 1.0f,0.0f,1.0f,//5
		0.4f,-0.4f,-0.4f, 0.5f,0.0f,1.0f,//6
		-0.4f,-0.4f,-0.4f, 0.5f,0.0f,1.0f//7
	};

	GLuint cubeIndices[] = 
	{
		0,1,2,
		2,3,0, // QUAD1 : front
		1,5,6,
		6,2,1, // QUAD2 : left
		5,6,7,
		7,4,5, // QUAD3 : back
		4,7,3,
		3,0,4, // QUAD4 : right
		0,1,5,
		5,4,0, // QUAD5 : top
		3,2,6,
		6,7,3  // QUAD6 : bottom
	};

	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1, &cubeVBO); //创建VBO
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO); // 绑定新创建的VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // 解除当前绑定的VBO


	glGenBuffers(1, &cubeIBO); // 创建EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO); // 绑定新创建的EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_DYNAMIC_DRAW); 
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	bool show_another_window = true;

	//渲染循环：它能在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window)) {
		static int radio_flag = 2;
		static GLfloat proj_radians = 45.0f;
		static GLfloat scale_size = 1.0f;
		static GLfloat rotate_radians = 0.0f;
		static int is_filled = 2;
		static GLfloat translate_x = 0.0f;
		static glm::vec3 lookat_x = { 0,1,2 };
		static glm::vec3 lookat_y = { 0,0,0 };
		static glm::vec3 lookat_z = { 0,1,0 };

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//创建菜单栏窗口
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Begin("Transformation", &show_another_window);
		if (ImGui::TreeNode("Polygon Mode")) {
			ImGui::RadioButton("Unfilled", &is_filled, 0); ImGui::SameLine();
			ImGui::RadioButton("Filled", &is_filled, 1);
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Depth Test")){
			ImGui::RadioButton("Enable", &radio_flag, 0); ImGui::SameLine();
			ImGui::RadioButton("Disable", &radio_flag,1);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Perspective")) {
			ImGui::SliderFloat("Projection Angle", &proj_radians,0.0f, 360.0f);
			ImGui::DragFloat3("Look At x", (GLfloat*)&lookat_x, 0.5f, -30.f,30.f);
			ImGui::DragFloat3("Look At y", (GLfloat*)&lookat_y, 0.5f,-30.f, 30.f);
			ImGui::DragFloat3("Look At z", (GLfloat*)&lookat_z, 0.5f, -30.f, 30.f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Transform : Manual")) {
			ImGui::SliderFloat("Translation Offset", &translate_x, -10.0f, 10.0f);
			ImGui::SliderFloat("Rotation Angle", &rotate_radians, 0.0f, 360.0f);
			ImGui::SliderFloat("Scale Factor", &scale_size, 0.0f, 10.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Transform : Automatic")) {
			//ImGui::RadioButton("(Translation",);
			//ImGui::RadioButton("Rotation",);			
			//ImGui::RadioButton("Scaling",);
			ImGui::TreePop();
		}
		ImGui::End();
		//渲染，显示窗口
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


		if (radio_flag == 0) {
			glEnable(GL_DEPTH_TEST);//启动深度测试
		}
		else if (radio_flag == 1) {
			glDisable(GL_DEPTH_TEST);//关闭深度测试
		}
		
		cubeShader.use();//激活着色器程序	
		glm::mat4 View = glm::mat4(1.0f), 
				  Projection = glm::mat4(1.0f),
				  Model = glm::mat4(1.0f);
		View = glm::lookAt(lookat_x, lookat_y, lookat_z);//视角
		Projection = glm::perspective(glm::radians(proj_radians), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);//透视
		Model = glm::translate(Model, glm::vec3(translate_x, 0.0f, 0.0f));
		Model = glm::rotate(Model, glm::radians(rotate_radians), glm::vec3(0.0f, 1.0f, 0.0f));//旋转
		Model = glm::scale(Model, glm::vec3(scale_size, scale_size, scale_size));//缩放
		glm::mat4 transform = Projection * View * Model;
		GLint transformID = glGetUniformLocation(cubeShader.ID, "transform");
		glUniformMatrix4fv(transformID, 1, GL_FALSE, glm::value_ptr(transform));

		//绘制图元
		glBindVertexArray(cubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		is_filled ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL): glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		// 检查并调用事件，交换缓冲
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &cubeIBO);
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
