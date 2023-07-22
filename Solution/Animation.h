#ifndef ANIMATION_H
#define ANIMATION_H

#include <Eigen/Dense>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Animation {
public:
    Animation(
        Eigen::MatrixXf*& hexagonGlobalPositions,
        Eigen::MatrixXf*& modelHexagon,
        Eigen::MatrixXf*& j1LocalHomogeneousMatrix,
        Eigen::MatrixXf*& j2LocalPositionVectors);

    void drawFrame();
    void nextFrame();

    // Mouse and scroll event handling functions
    void processMouseMovement(double xpos, double ypos);
    void processScroll(double yoffset);

private:
    Eigen::MatrixXf* hexagonGlobalPositions;
    Eigen::MatrixXf* modelHexagon;
    Eigen::MatrixXf* j1LocalHomogeneousMatrix;
    Eigen::MatrixXf* j2LocalPositionVectors;
    int currentFrame;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float yaw;
    float pitch;
    float sensitivity;
    float movementSpeed;
    float zoomSpeed;
    Eigen::Matrix4d projection;
    Eigen::Matrix4d viewMatrix;
    Eigen::Vector4f j0GlobalPosition;
    Eigen::Vector4f j1GlobalPosition;
    Eigen::Vector4f j2GlobalPosition;

    float linkDistanceJ0ToJ1;
    float linkDistanceJ1ToJ2;

    std::vector<float> linkDistancesH1;
    std::vector<float> linkDistancesH2;

    Eigen::Matrix4f multiplyMatrices(const Eigen::Matrix4f& mat1, const Eigen::Matrix4f& mat2);
    Eigen::Vector3f transformPoint(const Eigen::Matrix4f& transformMatrix, const Eigen::Vector3f& point);
    void calculateGlobalPositions();
};

#endif
