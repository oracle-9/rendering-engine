#pragma once

#include <glm/vec3.hpp>

namespace util {

[[nodiscard]]
auto to_cartesian(glm::vec3 const& cylindrical) -> glm::vec3;

[[nodiscard]]
auto to_cylindrical(glm::vec3 const& cartesian) -> glm::vec3;

auto to_cartesian_inplace(glm::vec3& cylindrical) -> void;

auto to_cylindrical_inplace(glm::vec3& cartesian) -> void;

} // namespace util
