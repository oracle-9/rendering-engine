#include "generator/util/coord_conv.hpp"

#include <glm/exponential.hpp>
#include <glm/trigonometric.hpp>

auto to_cartesian(glm::vec3 const& cylindrical) -> glm::vec3 {
    float const r = cylindrical[0];
    float const y = cylindrical[1];
    float const theta = cylindrical[2];

    return glm::vec3 {
        r * glm::cos(theta),
        y,
        r * glm::sin(theta),
    };
}

auto to_cylindrial(glm::vec3 const& cartesian) -> glm::vec3 {
    float const x = cartesian.x;
    float const y = cartesian.y;
    float const z = cartesian.z;

    return glm::vec3 {
        glm::sqrt(x * x + z * z),
        y,
        glm::atan(z, x),
    };
}

auto to_cartesian_inplace(glm::vec3& cylindrical) -> void {
    float const r = cylindrical[0];
    float const y = cylindrical[1];
    float const theta = cylindrical[2];
    cylindrical[0] = r * glm::cos(theta);
    cylindrical[1] = y;
    cylindrical[2] = r * glm::sin(theta);
}

auto to_cylindrical_inplace(glm::vec3& cartesian) -> void {
    float const x = cartesian.x;
    float const y = cartesian.y;
    float const z = cartesian.z;
    cartesian[0] = glm::sqrt(x * x + z * z);
    cartesian[1] = y;
    cartesian[2] = glm::atan(z, x);
}
