#include "engine/render/renderer.hpp"

#include "engine/config.hpp"
#include "engine/render/world/world.hpp"
#include "util/pretty_print.hpp"

#include <GL/freeglut.h>
#include <fmt/core.h>

namespace engine::render {
using config::prog_name;

struct render_payload {
    struct world* world;
    double aspect_ratio;
};

auto display_info() {
    // reinterpret_cast is needed to silence some fmt + unsigned char warnings.
    pretty_print(
        "Vendor: {}\n", reinterpret_cast<char const*>(glGetString(GL_VENDOR))
    );
    pretty_print(
        "Renderer: {}\n",
        reinterpret_cast<char const*>(glGetString(GL_RENDERER))
    );
    pretty_print(
        "Version: {}\n",
        reinterpret_cast<char const*>(glGetString(GL_VERSION))
    );
}

auto render(void* const payload) -> void {
    auto [world_ptr, aspect_ratio] = *static_cast<render_payload*>(payload);
    auto const& camera_pos = world_ptr->camera.pos;
    auto const& camera_lookat = world_ptr->camera.lookat;
    auto const& camera_up = world_ptr->camera.up;
    auto const& camera_proj = world_ptr->camera.projection;

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(
        camera_pos.x,    camera_pos.y,    camera_pos.z,
        camera_lookat.x, camera_lookat.y, camera_lookat.z,
        camera_up.x,     camera_up.y,     camera_up.z
    );
    gluPerspective(
        camera_proj[0], aspect_ratio, camera_proj[1], camera_proj[2]
    );

    glBegin(GL_TRIANGLES);
        glVertex3f(0.f, 0.f, 0.f);
        glVertex3f(1.f, 1.f, 2.f);
        glVertex3f(3.f, 3.f, 3.f);
    glEnd();

    glutSwapBuffers();
}

renderer::renderer() noexcept : world_ref{&renderer::default_world} {
    {
        // GLUT requires argc and argv to be passed to their init function,
        // which we don't to forward.
        // Therefore, we create our own dummy values and forward those instead.
        // This is done in an anonymous scope as not to polute the function
        // "global" namespace.
        static int dummy_argc = 1;
        static char dummy_arg[] = "";
        static char* dummy_argv[] = {dummy_arg};
        glutInit(&dummy_argc, dummy_argv);
    }

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(renderer::WIN_POS_X, renderer::WIN_POS_Y);
    glutInitWindowSize(renderer::WIN_WIDTH, renderer::WIN_HEIGHT);
    glutCreateWindow(renderer::WIN_TITLE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glClearColor(
        renderer::BG_COLOR.r,
        renderer::BG_COLOR.g,
        renderer::BG_COLOR.b,
        renderer::BG_COLOR.a
    );

    display_info();
}

auto renderer::set_world(struct world& world) noexcept -> renderer& {
    render_payload static payload; // payload must be static to ensure
                                   // GLUT doesn't outlive it.
    this->world_ref = &world;
    payload = render_payload {
        .world = this->world_ref,
        .aspect_ratio
            = static_cast<double>(renderer::WIN_WIDTH)
            / static_cast<double>(renderer::WIN_HEIGHT),
    };
    glutDisplayFuncUcall(render, &payload);
    return *this;
}

auto renderer::get() noexcept -> renderer& {
    auto static lazy_static = renderer{};
    return lazy_static;
}

[[noreturn]]
auto renderer::run() noexcept -> void {
    glutMainLoop();
    __builtin_unreachable(); // glutMainLoop is noreturn,
                             // but isn't marked as such.
}

} // namespace engine::render
