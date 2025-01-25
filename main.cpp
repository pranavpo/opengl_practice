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

int main()
{
    glm::vec3 A(4.0f, 0.0f, 0.0f);
    glm::vec3 B(0.0f, 4.0f, 0.0f);
    glm::vec3 C = glm::cross(A,B);
    // glm::vec3 result = A + B;
    // // glm::mat4 C(1.0f);
    // std::cout << "A " << glm::length(A) << std::endl;
    // // std::cout << "B " << glm::length(B) << std::endl;
    // std::cout << "A-normalized " << glm::to_string(glm::normalize(A)) << std::endl;
    // std::cout << "Length of A after normalization: " << glm::length(glm::normalize(A)) << std::endl;
    // std::cout << "Sum of A and B: " << glm::to_string(result) << std::endl;
    // std::cout << "Dot product of A and B: " << glm::dot(normalize(A),normalize(B)) << std::endl;
    // float dot_product_normalized = glm::dot(normalize(A),normalize(B));
    // std::cout << "angle is: " << std::acos(dot_product_normalized) * 180.0/M_PI << std::endl;

    std::cout << "Cross product AXB " << glm::to_string(C) << std::endl;

    return 0;
}