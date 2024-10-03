#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Camera {
private:
    //Camera properties
    const float cameraSpeed = 0.7f;
    float yaw = -90.0f, pitch = 0.0f, fov = 45.0;
    float lastX, lastY;
    bool firstMouse = true;
    int width, height;
    float nearClipDist = 0.1f;
    float farClipDist = 300.0f;
    Shader *assocShader;

    //Camera vectors
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);

    //pipeline Matrices
    glm::mat4 curView;
    glm::mat4 curProj;

public:
    // constructor reads and builds the camera
    Camera(Shader *curShader, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float lastX, float lastY, GLFWwindow* window);

    // utility functions
    void setProjection(GLFWwindow* window);
    void cameraInput(GLFWwindow* window);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    //Getter and Setter
    glm::vec3 getCameraPos();
    glm::vec3 getCameraFront();
    void setCameraPos(glm::vec3 cameraPos);
    void setCameraFront(glm::vec3 cameraFront);
    void setAssocShader(Shader *curShader);
    void setViewMat(GLFWwindow* window);
    glm::mat4 getViewMat();
    int getAssocShaderNum();
    glm::mat4 getProjectionMat();
};

#endif