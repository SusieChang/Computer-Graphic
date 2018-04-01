#include <iostream>
#include <glad/glad.h>
#include <glew/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "Shader.h"
#include "BallVertices.h"

using glm::vec3;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void glfw_error_callback(int error, const char* description);
//设置窗口大小
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 700;
glm::mat4 rotateCube(glm::mat4& Model, bool check, GLfloat speed, GLfloat radians);
glm::mat4 translateCube(glm::mat4 Model, bool check, GLfloat speed, GLfloat offset);
glm::mat4 zoomCube(glm::mat4 Model, bool check, GLfloat speed, GLfloat offset);

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

	//创建着色器对象
	Shader cubeShader("cubeShader.vs", "cubeShader.fs");
	Shader ballShader("ballShader.vs", "ballShader.fs");

	 //设置顶点
	GLfloat cubeVertices[] = //画6个四边形,8个顶点
	{	//位置（x,y,z）		 
		-0.2f,0.2f, 0.2f, 1.0f,1.0f,0.0f,//0
		0.2f, 0.2f, 0.2f, 1.0f,1.0f,0.0f,//1
		0.2f,-0.2f, 0.2f, 0.0f,1.0f,1.0f,//2
		-0.2f,-0.2f,0.2f, 0.0f,1.0f,1.0f,//3
		-0.2f,0.2f,-0.2f, 1.0f,0.0f,1.0f,//4
		0.2f, 0.2f, -0.2f, 1.0f,0.0f,1.0f,//5
		0.2f,-0.2f,-0.2f, 0.5f,0.0f,1.0f,//6
		-0.2f,-0.2f,-0.2f, 0.5f,0.0f,1.0f//7
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

	GLfloat* ballVertices = new GLfloat[13000];
	int ballSize = genBallVertices(ballVertices, 0.2, 20, 50);
	cout << ballSize << endl;
	unsigned int VBO[2], VAO[2], IBO;
	glGenVertexArrays(2, VAO);//创建VAO
	glGenBuffers(2, VBO); //创建VBO	
	glGenBuffers(1, &IBO); // 创建EBO

	//--------设置立方体----------------------------------------------
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // 绑定新创建的VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // 解除当前绑定的VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // 绑定新创建的EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_DYNAMIC_DRAW); 
	//顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//---------设置球体-------------------------------------------------
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); // 绑定新创建的VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*ballSize, ballVertices, GL_DYNAMIC_DRAW);
	//顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	bool show_another_window = true;

	//渲染循环：它能在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window)) {
		static int radio_flag = 1.0f;
		static GLfloat proj_radians = 45.0f;
		static GLfloat scale_size = 1.0f;
		static GLfloat rotate_radians = 0.0f;
		static int is_filled = 2;
		static GLfloat translate_x = 0.0f;
		static glm::vec3 lookat_x = { 0,0,2 };
		static glm::vec3 lookat_y = { 0,0,0 };
		static glm::vec3 lookat_z = { 0,1,0 };
		static bool t_check = false;
		static bool r_check = false;
		static bool s_check = false;
		static int is_draw_ball = 2;

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
			ImGui::RadioButton("Disable", &radio_flag,0);ImGui::SameLine();
			ImGui::RadioButton("Enable", &radio_flag, 1);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Transform : Manual")) {
			ImGui::SliderFloat("Translation Offset", &translate_x, -3.0f, 3.0f);
			ImGui::SliderFloat("Rotation Angle", &rotate_radians, 0.0f, 360.0f);
			ImGui::SliderFloat("Scale Factor", &scale_size, 0.0f, 5.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Transform : Automatic")) {
			ImGui::Checkbox("Translation", &t_check);
			ImGui::Checkbox("Rotation", &r_check);			
			ImGui::Checkbox("Scaling", &s_check);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Amazing Animation")) {
			if (ImGui::RadioButton("Display",&is_draw_ball,1)) {
				radio_flag = 1.0f;
				proj_radians = 45.0f;
				scale_size = 1.0f;
				rotate_radians = 0.0f;
				is_filled = 0;
				translate_x = 0.0f;
				lookat_x = { 0,2,3 };
				lookat_y = { 0,-0.2,0 };
				lookat_z = { 0,1,0 };
				t_check = false;
				r_check = true;
				s_check = false;
			}
			ImGui::RadioButton("No Display", &is_draw_ball, 0);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Perspective")) {
			ImGui::SliderFloat("Projection Angle", &proj_radians, 0.0f, 360.0f);
			ImGui::DragFloat3("Look At x", (GLfloat*)&lookat_x, 0.5f, -30.f, 30.f);
			ImGui::DragFloat3("Look At y", (GLfloat*)&lookat_y, 0.5f, -30.f, 30.f);
			ImGui::DragFloat3("Look At z", (GLfloat*)&lookat_z, 0.5f, -30.f, 30.f);
			ImGui::TreePop();
		}
		ImGui::End();
		//渲染，显示窗口
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		
		//----启动或关闭深度测试-----------------------------
		radio_flag ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);		
		//----设置视角、模型、透射----------------------------
		glm::mat4 View = glm::mat4(1.0f), Projection = glm::mat4(1.0f), cubeModel = glm::mat4(1.0f), ballModel = glm::mat4(1.0f);
		View = glm::lookAt(lookat_x, lookat_y, lookat_z);//视角	
		Projection = glm::perspective(glm::radians(proj_radians), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);//透视		
		cubeModel = translateCube(cubeModel, t_check, 0.004, translate_x);//1. 平移
		cubeModel = rotateCube(cubeModel, r_check,60.0f, rotate_radians); //2. 旋转
		cubeModel = zoomCube(cubeModel, s_check, 0.0025, scale_size);	  //3. 缩放
		glm::mat4 cubeTransform = Projection * View * cubeModel;
		
		//绘制图元1
		cubeShader.use();
		glBindVertexArray(VAO[0]);
		GLint transformID = glGetUniformLocation(cubeShader.ID, "transform");
		glUniformMatrix4fv(transformID, 1, GL_FALSE, glm::value_ptr(cubeTransform));
		is_filled ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL): glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//绘制图元2
		if (is_draw_ball==1) {
			glm::mat4 ballTransform = glm::mat4(1.0);
			ballModel = rotateCube(ballModel, true, 50.0f, 0); //2. 旋转
			ballModel = translateCube(ballModel, false, 0.004, 1.5f);//1. 平移	
			ballTransform = Projection * View * ballModel;
			ballShader.use();
			glBindVertexArray(VAO[1]);
			int colorLocation = glGetUniformLocation(ballShader.ID, "ourColor");
			glUniform4f(colorLocation,0.98f, 0.5f, 0.447f, 1.0f);
			GLint transformID2 = glGetUniformLocation(ballShader.ID, "transform");
			glUniformMatrix4fv(transformID2, 1, GL_FALSE, glm::value_ptr(ballTransform));
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, ballSize/3);
		}

		// 检查并调用事件，交换缓冲
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(1, &IBO);
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

glm::mat4 rotateCube(glm::mat4& Model, bool check, GLfloat speed,GLfloat radians) {
	if (check) {
		Model = glm::rotate(Model, glm::radians((GLfloat)glfwGetTime() * speed), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		Model = glm::rotate(Model, glm::radians(radians), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	return Model;
}

glm::mat4 zoomCube(glm::mat4 Model,bool check, GLfloat speed, GLfloat offset) {
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