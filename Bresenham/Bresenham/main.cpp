#include <iostream>
#include <vector>
#include <windows.h>
#include <glad/glad.h>
#include <glew/glfw3.h>
#include <math.h>
#include <algorithm>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "Shader.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void glfw_error_callback(int error, const char* description);

void renderTriangle(GLint* v1, GLint* v2, GLint* v3, bool is_fill);
void renderCircle(GLint* center, GLfloat R, bool is_fill);
void Bresenham_circle(GLint* center, GLfloat R, bool is_fill);
void Bresenham_line(GLint* v1, GLint* v2, bool is_fill);
void setPixel(GLint x, GLint y);

GLfloat normalized_x(GLint);
GLfloat normalized_y(GLint);
void swap_data(int *a, int *b);

struct vi {
	int x;
	int y;
};

bool mycomp(const vi &a, const vi &b)
{
	//if (a.y <= b.y) return a.x < b.x;
	return a.y > b.y;
};

unsigned int VBO, VAO;
vector<GLfloat> outline;
vector<vi> tmp_outline;
const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;
unsigned int MAX_BUFFER = SRC_WIDTH*SRC_HEIGHT;

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
	// 初始化GLAD
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
	Shader triangleShader("shader.vs", "shader.fs");

	//生成一个VBO(顶点缓冲对象)
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	// 绑定VAO
	glBindVertexArray(VAO);

	//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上，把之前定义的顶点数据复制到缓冲的内存中
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*MAX_BUFFER, NULL, GL_DYNAMIC_DRAW);

	//顶点位置属性
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	bool show_another_window = true;

	//渲染循环：它能在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window)) {
		static float tmp_color[] = { 0.0,1.0,1.0,1.0 };
		static int radio_flag = 2;
		static bool check_flag = false;
		static int vertex1[2] = { 0,0 };
		static int vertex2[2] = { 0,0 };
		static int vertex3[2] = { 0,0 };
		static int center[2] = { 0,0 };
		static float radius = 0.0f;
		//当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//通过调用glClear函数来清空屏幕的颜色缓冲
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(triangleShader.ID);
		//改变颜色
		int colorLocation = glGetUniformLocation(triangleShader.ID, "ourColor");
		glUniform4f(colorLocation, tmp_color[0], tmp_color[1], tmp_color[2], 1.0f);

		//更新数据
		int n = outline.size();
		//cout << n << endl;
		GLfloat* tmp = new GLfloat[n];
		for (int i = 0; i < n; i++)
			tmp[i] = outline.at(i);
		vector<GLfloat>(outline).swap(outline);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * n, tmp);
		glPointSize(1.0f);
		glDrawArrays(GL_POINTS, 0, n / 2);
		//GLfloat v[2] = { 0,0 };
		//int n = outline.size();
		//for (int i = 0; i < n; i+=2) {
		//	v[0] = outline.at(i);
		//	v[1] = outline.at(i + 1);
		//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GL_FLOAT)*n, v);
		//	glPointSize(1.0f);
		//	glDrawArrays(GL_POINT, 0, 1);
		//}

		//创建菜单栏窗口
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Begin("Triangle Vertex Color Editer", &show_another_window);
		if (ImGui::TreeNode("Select a figure to draw")) {
			ImGui::RadioButton("Triangle", &radio_flag, 0); ImGui::SameLine();
			ImGui::RadioButton("Circle", &radio_flag, 1);
			ImGui::Spacing();
			ImGui::Checkbox("fill", &check_flag); ImGui::SameLine();
			ImGui::ColorEdit3("fill color", (float*)&tmp_color);
			ImGui::Spacing();
			ImGui::TextWrapped("User Tips: To prevent overflowing, the position range is from (-400,-300) to (400, 300)");
			if (radio_flag == 0)
			{
				if (ImGui::TreeNode("Input three vertices of triangle")) {
					static float begin = -1.0, end = 1.0;
					ImGui::DragInt2("Vertexb", vertex1, 1, -300, 300);
					ImGui::DragInt2("Vertex2", vertex2, 1, -300, 300);
					ImGui::DragInt2("Vertex3", vertex3, 1, -300, 300);
					if (ImGui::Button("Submit")) {
						renderTriangle(vertex1, vertex2, vertex3, check_flag);
					}
					ImGui::TreePop();
				}
			}
			if (radio_flag == 1)
			{
				if (ImGui::TreeNode("Input center and radius of circle")) {
					ImGui::DragInt2("Center", center, 1, -400, 400);
					ImGui::DragFloat("Radius", &radius, 0.1f, 0, 250);
					if (ImGui::Button("Submit")) {
						renderCircle(center, radius, check_flag);
					}
					ImGui::TreePop();
				}
			}
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


void renderTriangle(GLint* v1, GLint* v2, GLint* v3, bool is_fill) {
	outline.clear();
	tmp_outline.clear();

	Bresenham_line(v1, v2,is_fill);
	Bresenham_line(v1, v3,is_fill);
	Bresenham_line(v2, v3,is_fill);	
	if (is_fill) {
		sort(tmp_outline.begin(), tmp_outline.end(), mycomp);
		int n = tmp_outline.size();
		for (int i = 0; i < n - 1; i++) {
			int y1 = tmp_outline.at(i).y;
			int y2 = tmp_outline.at(i + 1).y;
			int x1 = tmp_outline.at(i).x;
			int x2 = tmp_outline.at(i + 1).x;
			if (y1 == y2) {
				if (x1 > x2) swap_data(&x1, &x2);
				for (int j = x1; j <= x2; j++) {
					setPixel(j, y1);
				}
			}
		}
		vector<vi>(tmp_outline).swap(tmp_outline);
	}
}

void renderCircle(GLint* center, GLfloat radius,bool is_fill) {
	outline.clear();
	Bresenham_circle(center, radius,is_fill);
}

void Bresenham_line(GLint* v1, GLint* v2, bool is_fill) {
	int x1 = v1[0], x2 = v2[0], y1 = v1[1], y2 = v2[1];
	int dx = abs(x2 - x1), dy = abs(y2 - y1);
	bool flag = false;
	vi v;
	if (dx < dy) {			
		flag = true;
		swap_data(&x1, &y1);
		swap_data(&x2, &y2);
		swap_data(&dx, &dy);
	}
	int cx = (x2 - x1) > 0 ? 1 : -1; // 决定x轴上的增量
	int cy = (y2 - y1) > 0 ? 1 : -1; // 决定y轴上的增量
	int x = x1, y = y1, d = dy * 2 - dx;
	while (x != x2) {
		if (d < 0) {
			d += dy * 2;
		}else {
			y += cy;
			d += (dy - dx) * 2;
		}
		if (flag) {// 之前是否交换了x、y	
			setPixel(y, x);
			v.x = y; v.y = x;
		} else {
			setPixel(x, y);
			v.x = x; v.y = y;
		}
		x += cx;
		if (is_fill) tmp_outline.push_back(v);
	}
}

void Bresenham_circle(GLint* center, GLfloat radius, bool is_fill) { //根据对称性
	int xc = center[0], yc = center[1];
	int x = 0, y = radius, iy, d = 3 - 2 * radius; //为了优化运算，将浮点数算法转化为整数算法
	while (x <= y) {
		if (is_fill) {
			for (iy = x; iy <= y; iy++) {
				setPixel(xc + x, yc + iy);
				setPixel(xc - x, yc + iy);
				setPixel(xc + x, yc - iy);
				setPixel(xc - x, yc - iy);
				setPixel(xc + iy, yc + x);
				setPixel(xc - iy, yc + x);
				setPixel(xc + iy, yc - x);
				setPixel(xc - iy, yc - x);
			}
		}
		setPixel(xc + x, yc + y);
		setPixel(xc - x, yc + y);
		setPixel(xc + x, yc - y);
		setPixel(xc - x, yc - y);
		setPixel(xc + y, yc + x);
		setPixel(xc - y, yc + x);
		setPixel(xc + y, yc - x);
		setPixel(xc - y, yc - x);
		if (d < 0) {	
			d += 4 * x + 6;
		} else {
			d += 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
}

void swap_data(int *a, int *b) {
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

GLfloat normalized_x(GLint x) {
	GLfloat y = ((GLfloat)x / (GLfloat)SRC_WIDTH) * 2.0f;
	return y;
}

GLfloat normalized_y(GLint x) {
	GLfloat y = ((GLfloat)x / (GLfloat)SRC_HEIGHT) * 2.0f;
	return y;
}

void setPixel(GLint x, GLint y) {
	outline.push_back(normalized_x(x));
	outline.push_back(normalized_y(y));
}

//每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}