#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <Eigen/Dense>
#include "Animation.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define M_PI 3.14159265358979323846

Animation::Animation(
    Eigen::MatrixXf*& hexagonGlobalPositions,
    Eigen::MatrixXf*& modelHexagon,
    Eigen::MatrixXf*& j1LocalHomogeneousMatrix,
    Eigen::MatrixXf*& j2LocalPositionVectors)

    : hexagonGlobalPositions(hexagonGlobalPositions),
    modelHexagon(modelHexagon),
    j1LocalHomogeneousMatrix(j1LocalHomogeneousMatrix),
    j2LocalPositionVectors(j2LocalPositionVectors),
    currentFrame(0),
    //cameraPos(glm::vec3(0.0f, 0.0f, 0.0f)),
    cameraPos(glm::vec3(0.0f, 0.0, 200.0f)),
    cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
    cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    yaw(0.0f),
    pitch(0.0f),
    sensitivity(0.1f),
    movementSpeed(5.0f),
    zoomSpeed(0.1f) {

    cameraPos.x+= 200.0f;


    // Check if any of the pointers are null
    if (!hexagonGlobalPositions || !modelHexagon || !j1LocalHomogeneousMatrix || !j2LocalPositionVectors) {
        throw std::invalid_argument("Null pointer passed to Animation constructor.");
    }
    // Set up initial joint positions
    int numFrames = hexagonGlobalPositions->rows() / 6;
    linkDistancesH1.resize(numFrames);
    linkDistancesH2.resize(numFrames);

       // Initialize projection matrix
    projection.setZero();
    double nearPlane = 0.1;
    double farPlane = 1000.0;
    double fov = 45.0;
    double aspectRatio = 1.0; // Set the aspect ratio 
    double fovRad = 1.0 / tan((fov * 0.5) * (M_PI / 180.0));

    projection(0, 0) = fovRad / aspectRatio;
    projection(1, 1) = fovRad;
    projection(2, 2) = (farPlane + nearPlane) / (nearPlane - farPlane);
    projection(2, 3) = -1.0;
    projection(3, 2) = (2.0 * farPlane * nearPlane) / (nearPlane - farPlane);


    calculateGlobalPositions();
}


void Animation::drawFrame() {
    //Calculate Positions
    calculateGlobalPositions();


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


        // Assuming you have a camera with position, front, and up vectors

    //uncomment those lines to have a zoom out effect in the animation 

    //float zoomFactor = 1.2f; // Adjust this value to control the zoom level
    //glm::vec3 cameraDirection = glm::normalize(cameraFront); // Assuming cameraFront is the direction the camera is facing
    //cameraPos -= zoomFactor * cameraDirection;

    

    // Set up the camera position and view using glm::lookAt
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glLoadMatrixf(glm::value_ptr(view));

    //Draw origin reference 

    glVertex3f(0, 0, 0);

    // Draw hexagonGlobalPoints
    glBegin(GL_POINTS);
    glColor3f(1, 1, 1);
    for (int i = 0; i < 6; i++) {
        float x = hexagonGlobalPositions->operator()(currentFrame * 6 + i, 0);
        float y = hexagonGlobalPositions->operator()(currentFrame * 6 + i, 1);
        float z = hexagonGlobalPositions->operator()(currentFrame * 6 + i, 2);
        glVertex3f(x, y, z);
    }
    glEnd();


    // Set the color and size of the joints and links
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glPointSize(5.0f); // Joint size
    glLineWidth(5.0f); // Link width

    // Draw the joints (assumed to be stored in joint0Position, joint1Position, joint2Position)
    glBegin(GL_POINTS);
    glVertex3f(j0GlobalPosition.x(), j0GlobalPosition.y(), j0GlobalPosition.z());
    glVertex3f(j1GlobalPosition.x(), j1GlobalPosition.y(), j1GlobalPosition.z());
    glVertex3f(j2GlobalPosition.x(), j2GlobalPosition.y(), j2GlobalPosition.z());
    glEnd();


    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    // Draw the links between joints
    glBegin(GL_LINES);
    glVertex3f(j0GlobalPosition.x(), j0GlobalPosition.y(), j0GlobalPosition.z());
    glVertex3f(j1GlobalPosition.x(), j1GlobalPosition.y(), j1GlobalPosition.z());

    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glVertex3f(j1GlobalPosition.x(), j1GlobalPosition.y(), j1GlobalPosition.z());
    glVertex3f(j2GlobalPosition.x(), j2GlobalPosition.y(), j2GlobalPosition.z());
    glEnd();

    // Flush the OpenGL pipeline and swap buffers
    glFlush();
   // glutSwapBuffers();

}



void Animation::nextFrame() {
    // Move to the next frame
    currentFrame++;
    if (currentFrame >= hexagonGlobalPositions->rows() / 6) {
        currentFrame = 0;
    }

    // Update joint positions for the next frame
    calculateGlobalPositions();
}

void Animation::processMouseMovement(double xpos, double ypos) {
    // Calculate the change in mouse position
    static double lastX = xpos;
    static double lastY = ypos;
    double deltaX = xpos - lastX;
    double deltaY = lastY - ypos; // reversed since y-coordinates range from bottom to top

    // Update yaw and pitch angles based on mouse movement
    yaw += (float)deltaX * sensitivity;
    pitch += (float)deltaY * sensitivity;

    // Clamp pitch to avoid flipping the camera
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    // Calculate the new camera front vector based on yaw and pitch angles
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    lastX = xpos;
    lastY = ypos;
}

void Animation::processScroll(double yoffset) {
     //Update camera position (zoom in or out) based on yoffset
    cameraPos += cameraFront * static_cast<float>(yoffset) * zoomSpeed;
}

Eigen::Matrix4f Animation::multiplyMatrices(const Eigen::Matrix4f& mat1, const Eigen::Matrix4f& mat2) {
    return mat1 * mat2;
}

Eigen::Vector3f Animation::transformPoint(const Eigen::Matrix4f& transformMatrix, const Eigen::Vector3f& point) {
    return (transformMatrix * Eigen::Vector4f(point[0], point[1], point[2], 1.0f)).head<3>();
}


// Function to calculate the variance of a vector of values
float calculateVariance(const std::vector<float>& values) {
    float sum = 0.0f;
    float sumSquared = 0.0f;
    for (float value : values) {
        sum += value;
        sumSquared += value * value;
    }
    float mean = sum / values.size();
    float meanSquared = mean * mean;
    float variance = sumSquared / values.size() - meanSquared;
    return variance;
}

void Animation::calculateGlobalPositions() {
    // 1. Get the transformation matrix for joint j0
    Eigen::Matrix4f j0Transform = Eigen::Matrix4f::Identity();
    j0Transform.block<3, 1>(0, 3) = Eigen::Vector3f(
        (*hexagonGlobalPositions)(currentFrame * 6, 0),
        (*hexagonGlobalPositions)(currentFrame * 6, 1),
        (*hexagonGlobalPositions)(currentFrame * 6, 2));

    // Get the model hexagon's center
    Eigen::Vector3f modelHexagonCenter = Eigen::Vector3f::Zero();
    // Create an Eigen::Map to handle the data from the pointer

    Eigen::Map<Eigen::MatrixXf> hexagon_map(modelHexagon->data(), modelHexagon->rows(), modelHexagon->cols());

    // Calculate the centroid of the hexagon markers
    Eigen::Vector3f centroid = hexagon_map.colwise().mean();


    // 2. Get the position and rotation of joint j1 in the global reference frame
    Eigen::Matrix4f j1Transform;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            j1Transform(i, j) = (*j1LocalHomogeneousMatrix)(currentFrame, i * 4 + j);
        }
    }

    // Calculate the global position of joint j0
   // j0GlobalPosition = Eigen::Vector4f::Zero(); // Assuming joint 0 is at the origin
    //j0GlobalPosition = j0Transform.col(3); // J0 is the position vector of the Homogeneous matrix
    j0GlobalPosition.head<3>() =  centroid;
    j0GlobalPosition[3] = 1.0f;
    j0GlobalPosition = j0Transform * j0GlobalPosition; // Assuming joint 0 is in the centroid of the hexagon markers


    // 3. Calculate the global position of joint j1
    j1GlobalPosition = j0Transform * j1Transform.col(3); // Assuming j1 position is stored in the last column of the transformation matrix

    // Calculate the global position of joint j2 using the local position vector for j2
    Eigen::Vector3f j2LocalPosition = Eigen::Vector3f(
        (*j2LocalPositionVectors)(currentFrame, 0),
        (*j2LocalPositionVectors)(currentFrame, 1),
        (*j2LocalPositionVectors)(currentFrame, 2));
    Eigen::Vector4f j2LocalPositionHomogeneous(j2LocalPosition(0), j2LocalPosition(1), j2LocalPosition(2), 1.0f);
   // j2GlobalPosition = j1GlobalPosition + j1Transform * j2LocalPositionHomogeneous;
    //j2GlobalPosition = j1Transform * j2LocalPositionHomogeneous;


    // Apply rotation to the local position vector using the 3x3 part of j1Transform
    Eigen::Vector3f rotatedJ2LocalPosition = j1Transform.block<3, 3>(0, 0) * j2LocalPosition;

    // Calculate the global position of joint j2 by adding the transformed j2LocalPosition to j1GlobalPosition
    Eigen::Vector4f homegeneousRotatedJ2(rotatedJ2LocalPosition(0), rotatedJ2LocalPosition(1), rotatedJ2LocalPosition(2), 1.0f);

    j2GlobalPosition = j1GlobalPosition + homegeneousRotatedJ2;

    // Calculate the link distance from joint j0 to j1
    linkDistanceJ0ToJ1 = (j1GlobalPosition - j0GlobalPosition).norm();
    linkDistancesH1[currentFrame] = linkDistanceJ0ToJ1;

    // Calculate the link distance from joint j1 to j2
    linkDistanceJ1ToJ2 = (j2GlobalPosition - j1GlobalPosition).norm();
    linkDistancesH2[currentFrame] = linkDistanceJ1ToJ2;

    //print  H1 and H2 frame by frame
    //std::cout << "H1 = " << linkDistancesH1[currentFrame] << ", H2 = " << linkDistancesH2[currentFrame] << std::endl;


    if (currentFrame == 999) {
        std::cout << "H1 = " << linkDistancesH1[currentFrame] << ", H2 = " << linkDistancesH2[currentFrame] << std::endl;
        std::cout << "Stan Desv H1 = " << std::sqrt(calculateVariance(linkDistancesH1)) << " , Stan Desv H2 = " << std::sqrt(calculateVariance(linkDistancesH2)) << std::endl;
    }
  

}
