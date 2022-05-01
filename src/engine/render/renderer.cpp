#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <GL/glew.h>
#include "engine/render/renderer.hpp"

#include "engine/config.hpp"
#include "engine/render/io_events.hpp"
#include "engine/render/render.hpp"
#include "engine/render/state.hpp"

#include <spdlog/spdlog.h>



#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

namespace engine::render {

auto static display_info() -> void;

auto get() -> Renderer& {
    auto static lazy_static = Renderer{};
    return lazy_static;
}




auto static bufferVBOs(Group const& root) noexcept -> void {
    for (auto const& model : root.models) {
        std::vector<float> BufferModel;
        for (auto const& vertex : model.vertices) {
            BufferModel.push_back(vertex[0]);
            BufferModel.push_back(vertex[1]);
            BufferModel.push_back(vertex[2]);
        }
        state::buffers.push_back(BufferModel);
    }
    for (auto const& child_node : root.children) {
        bufferVBOs(child_node);
    }
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

    glewInit();
    glEnableClientState(GL_VERTEX_ARRAY);

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
        bufferVBOs(state::world_ptr->root);

        int i = 0;

        glEnableClientState(GL_VERTEX_ARRAY);
        glGenBuffers(500,state::bind);
        int tamanho = static_cast<int> (state::buffers.size());

        for(int i = 0; i < tamanho;i++){
            glBindBuffer(GL_ARRAY_BUFFER, state::bind[i]);
            glBufferData(GL_ARRAY_BUFFER,sizeof(float) * state::buffers[i].size(),state::buffers[i].data(),GL_STATIC_DRAW);
        }

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
