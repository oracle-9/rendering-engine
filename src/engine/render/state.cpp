#include "engine/render/state.hpp"

#include "engine/config.hpp"
#include "engine/render/layout/world/group/group.hpp"

namespace engine::render::state {

using namespace brief_int;
using namespace brief_int::literals;

bool enable_axis = config::ENABLE_AXIS;
bool enable_lookat_indicator = config::ENABLE_LOOKAT_INDICATOR;

GLenum polygon_mode = config::DEFAULT_POLYGON_MODE;
float line_width = config::DEFAULT_LINE_WIDTH;

Keyboard keyboard = {};

World default_world_mut = config::DEFAULT_WORLD;
ptr::nonnull_ptr<World> world_ptr = ptr::nonnull_ptr_to(default_world_mut);

Camera default_camera_mut = config::DEFAULT_CAMERA;
ptr::nonnull_ptr<Camera> camera_ptr = ptr::nonnull_ptr_to(default_camera_mut);
enum CameraMode camera_mode;

std::vector<ptr::nonnull_ptr<Model const>> model_refs
    = build_model_refs(*world_ptr);

usize focused_model_idx = 0_uz;

auto build_model_refs(World const& world)
    -> std::vector<ptr::nonnull_ptr<Model const>>
{
    auto model_refs = std::vector<ptr::nonnull_ptr<Model const>>{};
    auto const init_model_refs
        = [&model_refs](auto&& self, Group const& root) -> void
    {
        for (auto const& model : root.models) {
            model_refs.emplace_back(&model);
        }
        for (auto const& child_node : root.children) {
            // shenanigans to allow recursive lambda.
            self(self, child_node);
        }
    };
    init_model_refs(init_model_refs, world.root);
    return model_refs;
}

std::vector<std::vector<float>> buffers ;
GLuint bind[500];

} // namespace engine::render::state
