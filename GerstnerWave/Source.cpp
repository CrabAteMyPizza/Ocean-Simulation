#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <chrono>
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "dataStoreAndUtils.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//Stored in heap due to compiler suggestion and size
//1 Tile has a resolution of 350 x 350.
static dStoreAndUtils::vecTexCoord waveField[3066001];
static unsigned int renderOrder[18375000];

int main() {

	//Window initialization and configs
	//Initialized opengl versions and profile type
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window, viewport size, capture and hide the cursor, and setting up the size of viewport when framebuffer is resize
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Ocean Wave Simulation", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	//Can only be done AFTER HAVING AN ACTUAL WINDOW CONTEXT
	//Load opengl function implementation
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Generating the waveField and wave Parameters using class dStoreAndUtils
	// ------------------------------- START -----------------------//
	
	dStoreAndUtils compartment = dStoreAndUtils();
	compartment.generateWaveField(waveField, renderOrder);

	glm::vec2 directions[64];
	glm::vec4 waveParams[64];
	compartment.generateDirectAndWaveParams(directions, waveParams, true);

	// -------------------------------  END ------------------------//

	//Load the skybox images, static normans, create the waveShader, and skyboxshader
	// -------------------------------------- START --------------------------------------------------------------- //
	const char* vertPath = "C:\\Users\\HP\\source\\repos\\GerstnerWave\\GerstnerWave\\VertexShaderGerstner.txt";
	const char* fragPath = "C:\\Users\\HP\\source\\repos\\GerstnerWave\\GerstnerWave\\FragmentShaders.txt";
	const char* skyboxVertPath = "C:\\Users\\HP\\source\\repos\\GerstnerWave\\GerstnerWave\\skyboxVert.txt";
	const char* skyboxFragPath = "C:\\Users\\HP\\source\\repos\\GerstnerWave\\GerstnerWave\\skyboxFrag.txt";

	Shader waveShader(vertPath, fragPath);
	Shader skyboxShader(skyboxVertPath, skyboxFragPath);

	unsigned int skyboxTexID = compartment.loadSkybox("Sunset");

	std::string nrmNames[] = { "Perturb_Nrm_Sm_1.png",  "Perturb_Nrm_Sm_2.png", "Perturb_Nrm_Lg_1.png", "Perturb_Nrm_Lg_2.png" };
	unsigned int waveNRMID[4];
	compartment.loadWaveNRM(nrmNames, waveNRMID, 4);

	for (int i = 0; i < 4; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, waveNRMID[i]);

		if (i == 0) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexID);
		}
	}

	// ----------------------------------------------------- END ----------------------------------------------------- //

	//Configuring the skybox 
	// -------------------------------------- START --------------------------------------------------------------- //

	skyboxShader.use();

	unsigned int VAO[2];
	glGenVertexArrays(2, VAO);
	glBindVertexArray(VAO[0]);
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	unsigned int VBO[2];
	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(compartment.skyboxVertices), compartment.skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Position attribute
	glEnableVertexAttribArray(0);

	skyboxShader.setInt("skyBox", 0);

	// ----------------------------------------------------- END ----------------------------------------------------- //

	//Configuring the wave shader
	// -------------------------------------- START --------------------------------------------------------------- //

	waveShader.use();

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(waveField), waveField, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3)); // texCoord attribute
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(renderOrder), renderOrder, GL_STATIC_DRAW);

	// ----------------------------------------------------- END ----------------------------------------------------- //

	//Setting up samplers
	for (int i = 0; i < 4; i++) {
		waveShader.setInt(("waveNRM_" + std::to_string(i)).c_str(), i);

		if (i == 0) {
			waveShader.setInt("reflectSampler", 0);
		}
	}
	
	//Pointing to the directions uniform
	glUniform2fv(glGetUniformLocation(waveShader.ID, "Directions"), 64, glm::value_ptr(directions[0]));
	glUniform4fv(glGetUniformLocation(waveShader.ID, "waveParams"), 64, glm::value_ptr(waveParams[0]));

	//instantiating camera class and defining viewport
	Camera mainCamera(&waveShader, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 960.0f, 540.0f, window);

	glViewport(0, 0, 1920, 1080);

	/*------------ SETTING UP CALLBACKS -----------------*/

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowUserPointer(window, &mainCamera);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	/*---------------------- END ------------------------*/

	glEnable(GL_DEPTH_TEST); // enable depth testing in the depth buffer
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Polygon Testing

	//Material Test
	glUniform3f(glGetUniformLocation(waveShader.ID, "material.diffuse"), 0.00f, 0.08f, 0.13f);
	glUniform3f(glGetUniformLocation(waveShader.ID, "material.specular"), 0.85, 0.85, 0.85);
	glUniform1f(glGetUniformLocation(waveShader.ID, "material.shininess"), 150.0f);

	//Light Test
	glUniform3f(glGetUniformLocation(waveShader.ID, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(waveShader.ID, "dirLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(waveShader.ID, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
	glDepthFunc(GL_LEQUAL);
	glm::vec3 curDir = glm::vec3(-1.08631f, -15.6359f, -25.7334f); //Sunset Alt 24.9743f, -4.14718f, 16.0384f Noon -1.08631f, -15.6359f, -25.7334f
	float startTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		//Performance Checking (in ms)
		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

		waveShader.use();
		glUniform1f(glGetUniformLocation(waveShader.ID, "time"), glfwGetTime() - startTime);

		//Check for aspect Ratio change and keyboard input
		mainCamera.cameraInput(window);
		mainCamera.setProjection(window);

		//Main Render Logic
		glUniform3f(glGetUniformLocation(waveShader.ID, "dirLight.direction"), curDir.x, curDir.y, curDir.z);
		glm::vec3 camPos = mainCamera.getCameraPos();
		glUniform3f(glGetUniformLocation(waveShader.ID, "dirLight.camPos"), camPos.x, camPos.y, camPos.z);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 18375000, GL_UNSIGNED_INT, 0);

		skyboxShader.use();
		skyboxShader.setMatrix4X4("projection", 1, mainCamera.getProjectionMat());
		skyboxShader.setMatrix4X4("view", 1, glm::mat4(glm::mat3(mainCamera.getViewMat())));

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();

		//Performance Checking (in ms)
		std::cout << (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() - ms)).count() << std::endl;
	}

	glfwTerminate();
	return 0;
}

//function to call when framebuffer is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//Function to relay the information
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	Camera* curCamPoint = (Camera*)glfwGetWindowUserPointer(window);
	curCamPoint->mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Camera* curCamPoint = (Camera*)glfwGetWindowUserPointer(window);
	curCamPoint->scroll_callback(window, xoffset, yoffset);
}