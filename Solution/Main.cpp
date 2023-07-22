#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <Eigen/Dense>
#include "Animation.h"

#include <Windows.h>
#include <ShlObj.h>
#include <Shlwapi.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma comment(lib, "Shlwapi.lib")


//camera global 
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 500.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;  // Yaw angle (initialized to face the negative Z-axis)
float pitch = 0.0f;  // Pitch angle
float sensitivity = 0.1f; // Mouse sensitivity
float movementSpeed = 5.0f; // Camera movement speed
float zoomSpeed = 0.1f; // Camera zoom speed



// Function to read data from CSV files and convert to Eigen matrices/vectors
void readCSVToMatrix(Eigen::MatrixXf* data, const std::string& filename, int rows, int cols) {
 
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        
    }

    else {
        std::string line;
        int row = 0;
        while (std::getline(file, line) && row < rows) {
            std::istringstream iss(line);
            float value;
            int col = 0;
            while (iss >> value && col < cols) {
                (*data)(row, col) = value;
                col++;
                if (iss.peek() == ',')
                    iss.ignore();
            }
            row++;
        }
    }

}

std::string getProjectPath() {
    wchar_t buffer[MAX_PATH];
    if (GetModuleFileNameW(NULL, buffer, MAX_PATH) > 0) {
        PathRemoveFileSpecW(buffer);
        PathRemoveFileSpecW(buffer); // Remove the "Debug" or "Release" folder
    }
    else {
        std::cerr << "Failed to get the project folder." << std::endl;
    }

    std::wstring wstr(buffer);
    std::string projectPath(wstr.begin(), wstr.end());
    std::cout << "Project Folder: " << projectPath << std::endl;

    return projectPath;
}


void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    static double lastX = xpos;
    static double lastY = ypos;

    // Calculate the change in mouse position
    double deltaX = xpos - lastX;
    double deltaY = lastY - ypos; // Inverted Y-axis for more intuitive camera movement

    // Update yaw and pitch angles based on mouse movement
    yaw += deltaX * sensitivity;
    pitch += deltaY * sensitivity;

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

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Update camera position (zoom in or out) based on yoffset
    cameraPos += cameraFront * static_cast<float>(yoffset) * zoomSpeed;
}


void fillMatrix(Eigen::MatrixXf* matrix) {
    int rows = matrix->rows();
    int cols = matrix->cols();
    int value = 1;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            (*matrix)(i, j) = value;
            value++;
        }
    }
}

// Function to print the first n rows and last n rows and all columns of the matrix
void printNRows(const Eigen::MatrixXf* matrix, int n) {
    int rowsToPrint = n;

    for (int i = 0; i < rowsToPrint; ++i) {
        for (int j = 0; j < (*matrix).cols(); ++j) {
            std::cout << (*matrix)(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "..........." << std::endl;

    for (int i = (*matrix).rows() - n; i < (*matrix).rows() ; ++i) {
        for (int j = 0; j < (*matrix).cols(); ++j) {
            std::cout << (*matrix)(i, j) << " ";
        }
        std::cout << std::endl;
    }    
    std::cout  << std::endl;
}


int main() {


    // Read data from CSV files and convert to Eigen matrices/vectors
    int numFrames = 1000;
    int numMarkers = 6;
    std::string projectPath = getProjectPath();

    Eigen::MatrixXf* matrix = new Eigen::MatrixXf(5, 5);
    fillMatrix(matrix);

    Eigen::MatrixXf* hexagonGlobalPositions = new Eigen::MatrixXf(numFrames * numMarkers, 3);
    Eigen::MatrixXf* modelHexagon = new Eigen::MatrixXf(numMarkers, 3);
    Eigen::MatrixXf* j1LocalHomogeneousMatrix = new Eigen::MatrixXf(numFrames, 4 * 4);
    Eigen::MatrixXf* j2LocalPositionVectors = new Eigen::MatrixXf(numFrames, 3);

    readCSVToMatrix(hexagonGlobalPositions, projectPath + "\\test_hexagon_global_positions.csv", numFrames * numMarkers, 3);
    readCSVToMatrix(modelHexagon,projectPath + "\\test_model_hexagon.csv", numMarkers, 3);
    readCSVToMatrix(j1LocalHomogeneousMatrix, projectPath + "\\j1_local_homogeneous_matrix.csv", numFrames, 4*4);
    readCSVToMatrix(j2LocalPositionVectors, projectPath + "\\j2_local_position_vectors.csv", numFrames, 3);

    std::cout << "hexagonGlobalPositions \n" << std::endl;
    printNRows(hexagonGlobalPositions, 5);
    std::cout << "modelHexagon,projectPath \n" << std::endl;
    printNRows(modelHexagon, 3);
    std::cout << "j1LocalHomogeneousMatrix \n," << std::endl;
    printNRows(j1LocalHomogeneousMatrix, 5);
    std::cout << "j2LocalPositionVectors \n" << std::endl;
    printNRows(j2LocalPositionVectors, 5);


    GLFWwindow* window;

    /* Initialize GLFW library */
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
        

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1600, 1000, "NPCxTest", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 1600, 1000);

    // Set up the projection matrix using glm
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));

    //  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Read data from CSV files and convert them to vectors

    // Create the animation object

    try {

        Animation animation(hexagonGlobalPositions, modelHexagon, j1LocalHomogeneousMatrix, j2LocalPositionVectors);
        std::cout << "Animation object created " << std::endl;
        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        //1 frame testing
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //while (!glfwWindowShouldClose(window))
        //{
        //    animation.drawFrame();
        //    glfwSwapBuffers(window);
        //    glfwPollEvents();
        //}
        // Main rendering loop
        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            animation.drawFrame();
            animation.nextFrame();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glfwTerminate();

        //deallocate memory
        delete hexagonGlobalPositions;
        delete modelHexagon;
        delete j1LocalHomogeneousMatrix;
        delete j2LocalPositionVectors;
    }
    catch (const std::invalid_argument& e) {
        // Handle the exception: Print an error message or take any other appropriate action
        std::cerr << "Error: " << e.what() << std::endl;
    }



    return 0;
}
