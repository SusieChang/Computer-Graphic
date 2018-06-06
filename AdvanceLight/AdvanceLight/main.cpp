#include <glad/glad.h>
#include <glew/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void renderCube(Shader&);
void renderQuad(Shader&);

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 680;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
bool shadows = true;
bool shadowsKeyPressed = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int planeVAO, cubeVAO, planeVBO, cubeVBO;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 建立ImGui绑定
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	Shader floorShader("orth_shadow.vs", "orth_shadow.fs");
	Shader cubeShader("orth_cube_shadow.vs", "orth_cube_shadow.fs");
	Shader simpleDepthShader("orth_shadow_depth.vs", "orth_shadow_depth.fs");
	Shader point_floorShader("point_shadow.vs", "point_shadow.fs");
	Shader point_cubeShader("point_cube_shadow.vs", "point_cube_shadow.fs");
	Shader point_simpleDepthShader("point_shadow_depth.vs", "point_shadow_depth.fs","point_shadow_depth.gs");
	renderCube(cubeShader);
	renderQuad(floorShader);

	// configure depth map FBO
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, 
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int depthCubeMapFBO;
	glGenFramebuffers(1, &depthCubeMapFBO);
	// create depth cubemap texture
	unsigned int depthCubeMap;
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	vector<std::string> faces
	{
		"resources/moon.jpg",
		"resources/moon.jpg",
		"resources/moon.jpg",
		"resources/moon.jpg",
		"resources/moon.jpg",
		"resources/moon.jpg",
	};
	unsigned int cubemapTexture = loadCubemap(faces);
	cubeShader.use();
	cubeShader.setInt("skybox", 0);
	cubeShader.setInt("shadowMap", 1);
	point_cubeShader.use();
	point_cubeShader.setInt("skybox", 0);
	point_cubeShader.setInt("depthCubeMap", 1);

	// load textures
	unsigned int floorTexture = loadTexture("resources/brick.jpg");
	// shader configuration
	floorShader.use();
	floorShader.setInt("floorTexture", 0);
	floorShader.setInt("shadowMap", 1);
	point_floorShader.use();
	point_floorShader.setInt("floorTexture", 0);
	point_floorShader.setInt("depthCubeMap", 1);

	// lighting info
	glm::vec3 lightPos(5.0f, 1.5f, 4.0f);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		static int lightType = 0;
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);
		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float near_plane = 1.0f, far_plane = 10.0f;
		if (lightType == 1) {
			//lightPos = {};
			// 1. render depth of scene to texture (from light's perspective)x;
			glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
			std::vector<glm::mat4> shadowTransforms;
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFBO);
			point_simpleDepthShader.use();
			for (unsigned int i = 0; i < 6; ++i)
				point_simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
			point_simpleDepthShader.setFloat("far_plane", far_plane);
			point_simpleDepthShader.setVec3("lightPos", lightPos);

			glClear(GL_DEPTH_BUFFER_BIT);
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			// reset viewport
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// draw objects
			point_floorShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			point_floorShader.setMat4("projection", projection);
			point_floorShader.setMat4("view", view);
			// set light uniforms
			point_floorShader.setVec3("viewPos", camera.Position);
			point_floorShader.setVec3("lightPos", lightPos);
			point_floorShader.setVec3("lightColor", 0.6, 0.6, 0.6);
			point_floorShader.setFloat("far_plane", far_plane);
			// floor
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, floorTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// draw cube
			point_cubeShader.use();
			point_cubeShader.setMat4("projection", projection);
			point_cubeShader.setMat4("view", view);
			// set light uniforms
			point_cubeShader.setVec3("viewPos", camera.Position);
			point_cubeShader.setVec3("lightPos", lightPos);
			point_cubeShader.setVec3("lightColor", 0.6, 0.6, 0.6);
			point_cubeShader.setFloat("far_plane", far_plane);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
		else {
			// 1. render depth of scene to texture (from light's perspective)
			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;

			simpleDepthShader.use();
			simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			// reset viewport
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// draw objects
			floorShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			floorShader.setMat4("projection", projection);
			floorShader.setMat4("view", view);
			// set light uniforms
			floorShader.setVec3("viewPos", camera.Position);
			floorShader.setVec3("lightPos", lightPos);
			floorShader.setVec3("lightColor", 0.6, 0.6, 0.6);
			floorShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
			// floor
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, floorTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// draw cube
			cubeShader.use();
			cubeShader.setMat4("projection", projection);
			cubeShader.setMat4("view", view);
			// set light uniforms
			cubeShader.setVec3("viewPos", camera.Position);
			cubeShader.setVec3("lightPos", lightPos);
			cubeShader.setVec3("lightColor", 0.6, 0.6, 0.6);
			cubeShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
			glBindVertexArray(cubeVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Begin("Transformation");
		if (ImGui::TreeNode("choose light matrix")) {
			ImGui::RadioButton("orthogonal m", &lightType, 0);
			ImGui::RadioButton("perspective m", &lightType, 1);
			ImGui::TreePop();
		}
		ImGui::End();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}


void renderCube(Shader &cubeShader) {
	//设置顶点
	GLfloat cubeVertices[] = //画6个四边形,8个顶点
	{	//位置（x,y,z）		 
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,

		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,

		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f
	};

	glGenVertexArrays(1, &cubeVAO);//创建VAO
	glGenBuffers(1, &cubeVBO); //创建VBO	

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO); // 绑定新创建的VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);
	//顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void renderQuad(Shader& floorShader) {
	// set up vertex data (and buffer(s)) and configure vertex attributes
	float planeVertices[] = {
		// positions            // normals         // texcoords
		10.0f, -1.3f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -1.3f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -1.3f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		10.0f, -1.3f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -1.3f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		10.0f, -1.3f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}