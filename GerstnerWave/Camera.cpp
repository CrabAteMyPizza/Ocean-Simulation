#include "Camera.h"

//for better usage the camera should be assumed to be attached to one and one window context only
//Constructor Definition
Camera::Camera(Shader *curShader, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float lastX, float lastY, GLFWwindow* window) {
    this->cameraFront = cameraFront;
    this->cameraPos = cameraPos;
    this->cameraUp = cameraUp;
    this->lastX = lastX;
    this->lastY = lastY;
    this->assocShader = curShader;

    //Setting up default matrix view and projection
    setProjection(window);
    setViewMat(window);

    //BIG NOTE:: The setting of callbacks can only be done by using pointer to an OBJ of camera class which we must point
    //SUCKS IK ill do it on the main source.
}

//Set the projection matrix of the selected shader
void Camera::setProjection(GLFWwindow *window) {
    glfwGetWindowSize(window, &(this->width), &(this->height));
    this->curProj = glm::mat4(1.0f);
    this->curProj = glm::perspective(glm::radians(this->fov), (float)(this->width) / (float)(this->height), this->nearClipDist, this->farClipDist);
    glUniformMatrix4fv(glGetUniformLocation(assocShader->ID, "project"), 1, GL_FALSE, glm::value_ptr(this->curProj));
}

void Camera::setViewMat(GLFWwindow* window) {
    this->curView = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
    glUniformMatrix4fv(glGetUniformLocation(assocShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(this->curView));
}

//Processing all Camera Inputs using GLFW
void Camera::cameraInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    setViewMat(window);
}

//Function to call when mouse is calledback
void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->cameraFront = glm::normalize(direction);

    setViewMat(window);
}

//function to call when scroll is used
void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
    setProjection(window);
}

void Camera::setCameraFront(glm::vec3 cameraFront){
    this->cameraFront = cameraFront;
}

void Camera::setCameraPos(glm::vec3 cameraPos) {
    this->cameraPos = cameraPos;
}

glm::vec3 Camera::getCameraFront() {
    return this->cameraFront;
}

glm::vec3 Camera::getCameraPos() {
    return this->cameraPos;
}

void Camera::setAssocShader(Shader *curShader) {
    this-> assocShader = curShader;
}

int Camera::getAssocShaderNum() {
    return this-> assocShader->ID;
}

glm::mat4 Camera::getViewMat() {
    return this->curView;
}

glm::mat4 Camera::getProjectionMat() {
    return this->curProj;
}