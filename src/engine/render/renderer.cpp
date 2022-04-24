#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "engine/render/renderer.hpp"

#include "engine/config.hpp"
#include "engine/render/io_events.hpp"
#include "engine/render/render.hpp"
#include "engine/render/state.hpp"

#include <spdlog/spdlog.h>

namespace engine::render {

auto static display_info() -> void;

auto get() -> Renderer& {
    auto static lazy_static = Renderer{};
    return lazy_static;
}

Renderer::Renderer() {
    // GLUT requires argc and argv to be passed to their init function,
    // which we don't want to forward.
    // Therefore, we create our own dummy values and forward those instead.
    static int dummy_argc = 1;
    static char dummy_arg[] = "";
    static char* dummy_argv[] = {dummy_arg};
    glutInit(&dummy_argc, dummy_argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(config::WIN_POS_X, config::WIN_POS_Y);
    glutInitWindowSize(config::WIN_WIDTH, config::WIN_HEIGHT);
    glutCreateWindow(config::WIN_TITLE);
    glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutTimerFunc(config::RENDER_TICK_MILLIS, [](int) { update_camera(); }, 0);
    glutKeyboardFunc(key_down);
    glutKeyboardUpFunc(key_up);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(
        config::DEFAULT_BG_COLOR.r,
        config::DEFAULT_BG_COLOR.g,
        config::DEFAULT_BG_COLOR.b,
        config::DEFAULT_BG_COLOR.a
    );
    glPolygonMode(GL_FRONT, config::DEFAULT_POLYGON_MODE);
    glLineWidth(config::DEFAULT_LINE_WIDTH);
    display_info();
}

auto Renderer::set_world(World& world) -> Renderer& {
    if (auto* const world_ptr = &world;
        world_ptr != state::world_ptr
    ) {
        state::world_ptr = world_ptr;
        state::model_refs = state::build_model_refs(world);
    }
    return *this;
}

auto Renderer::set_camera(Camera& camera) -> Renderer& {
    state::camera_ptr = &camera;
    return *this;
}

auto Renderer::run() noexcept -> void {
    glutMainLoop();
}

auto static display_info() -> void {
    // reinterpret_cast is needed to silence some fmt + unsigned char warnings.
    spdlog::info(
        "vendor: {}.",
        reinterpret_cast<char const*>(glGetString(GL_VENDOR))
    );
    spdlog::info(
        "renderer: {}.",
        reinterpret_cast<char const*>(glGetString(GL_RENDERER))
    );
    spdlog::info(
        "version: {}.",
        reinterpret_cast<char const*>(glGetString(GL_VERSION))
    );
}

} // namespace engine::render
