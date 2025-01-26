#include <glm/vec3.hpp>                  // glm::vec3
#include <glm/vec4.hpp>                  // glm::vec4
#include <glm/mat4x4.hpp>                // glm::mat4
#include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp>  // glm::pi
#include <iostream>
#include <glm/gtx/string_cast.hpp>
// using namespace std;

// glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
// {

// }
void about_vectors();
void scaling_matrix();
void rotating();
void translation();
void multiple_transforms();
int main()
{

    // about_vectors();
    // scaling_matrix();
    // rotating();
    // translation();
    multiple_transforms();
    return 0;
}

void about_vectors(){
    // THIS IS ABOUT VECTORS
    glm::vec3 A(4.0f, 0.0f, 0.0f);
    glm::vec3 B(0.0f, 4.0f, 0.0f);
    glm::vec3 C = glm::cross(A,B);
    glm::vec3 result = A + B;
    // glm::mat4 C(1.0f);
    std::cout << "A " << glm::length(A) << std::endl;
    // std::cout << "B " << glm::length(B) << std::endl;
    std::cout << "A-normalized " << glm::to_string(glm::normalize(A)) << std::endl;
    std::cout << "Length of A after normalization: " << glm::length(glm::normalize(A)) << std::endl;
    std::cout << "Sum of A and B: " << glm::to_string(result) << std::endl;
    std::cout << "Dot product of A and B: " << glm::dot(normalize(A),normalize(B)) << std::endl;
    float dot_product_normalized = glm::dot(normalize(A),normalize(B));
    std::cout << "angle is: " << std::acos(dot_product_normalized) * 180.0/M_PI << std::endl;

    std::cout << "Cross product AXB " << glm::to_string(C) << std::endl;
    // ABOUT VECTORS ENDS HERE
}

void scaling_matrix(){
    // if w is 0 it is a direction vector eg: glm:: vec4 vertex(0.0f,0.0f,-4.0f,0.0f); if w is 1 it is a point vector eg glm:: vec4 vertex(1.0f,5.0f,1.0f,1.0f);
    glm:: vec4 vertex(1.0f,5.0f,1.0f,1.0f);

    // point-point => direction vector
    // point + point => nonsensical
    // vector - vector => vector
    glm::mat4 model(1.0f); //diagonal 1 other are 0s. identity matrix. 4x4
    // while multiplying matrix and vector always matrix * vector not vector * matrix for some reason

    model = glm::scale(glm::mat4(1.0),glm::vec3(2.0f,2.0f,2.0f));
    std::cout << "scale matrix is " << glm::to_string(model) << std::endl;

    glm::vec4 worldspace_vertex = model * vertex;
    std::cout << "original vertex is " << glm::to_string(vertex) << std::endl;
    std::cout << "scaled vertex is " << glm::to_string(worldspace_vertex) << std::endl;
}

void rotating(){
    glm:: vec4 vertex(1.0f,5.0f,1.0f,1.0f);
    glm::mat4 model(1.0f);
    model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0,1,0));
    std::cout << "Rotation matrix is:\n" << glm::to_string(model) << std::endl;
    std::cout << "Original vertex is: " << glm::to_string(vertex) << std::endl;
    glm::vec4 rotated_vertex = model * vertex;
    std::cout << "Rotated vertex is: " << glm::to_string(rotated_vertex) << std::endl;
    // std::cout << "scaled vertex is " << glm::to_string(worldspace_vertex) << std::endl;
}

void translation(){
    glm:: vec4 vertex(1.0f,5.0f,1.0f,1.0f);
    glm::mat4 model(1.0f);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f,0.0f,-2.0f));
    glm::vec4 worldspace_vector = model * vertex;
    std::cout << "Translation matrix is:\n" << glm::to_string(model) << std::endl;
    std::cout << "Original vertex is: " << glm::to_string(vertex) << std::endl;
    std::cout << "Translated vertex is: " << glm::to_string(worldspace_vector) << std::endl;
}

void multiple_transforms(){
    glm:: vec4 vertex(1.0f,5.0f,1.0f,1.0f);
    glm::mat4 model(1.0f);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f,2.0f,2.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0,1,0));
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f,0.0f,-2.0f));
    // rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0,1,0));
    model = scale*rotate*translate; 
    // first applies translate then rotates then scales ===> last one first one to execute
    glm::vec4 worldspace_vector = model * vertex;
    std::cout << "Multiple transforms vertex is: " << glm::to_string(worldspace_vector) << std::endl;
}