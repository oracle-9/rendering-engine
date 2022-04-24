#pragma once

#include "engine/render/camera.hpp"
#include "engine/render/keyboard.hpp"
#include "engine/render/layout/world/camera.hpp"
#include "engine/render/layout/world/group/model.hpp"
#include "engine/render/layout/world/world.hpp"

#include <GL/freeglut.h>
#include <brief_int.hpp>
#include <nonnull_ptr.hpp>
#include <vector>

namespace engine::render::state {

extern bool enable_axis;
extern bool enable_lookat_indicator;

extern GLenum polygon_mode;

extern float line_width;

extern Keyboard keyboard;

extern World default_world_mut;
extern ptr::nonnull_ptr<World> world_ptr;

extern Camera default_camera_mut;
extern ptr::nonnull_ptr<Camera> camera_ptr;
extern CameraMode camera_mode;

extern std::vector<ptr::nonnull_ptr<Model const>> model_refs;

extern brief_int::usize focused_model_idx;

auto build_model_refs(World const& world)
    -> std::vector<ptr::nonnull_ptr<Model const>>;

} // namespace engine::render::state
