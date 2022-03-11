#include "engine/render/renderer.hpp"

#include "engine/config.hpp"
#include "engine/render/world/world.hpp"
#include "util/pretty_print.hpp"

#include <GL/freeglut.h>
#include <fmt/core.h>

namespace engine::render {

auto render() noexcept -> void;
auto resize(int const width, int height) -> void;
auto display_info() -> void;

namespace state {
    // Will eventually allow "reverting" to default world to undo changes.
    auto default_world_mut = config::DEFAULT_WORLD;
    world* world_ptr = &default_world_mut;

    double aspect_ratio
        = static_cast<double>(config::DEFAULT_WIN_WIDTH)
        / static_cast<double>(config::DEFAULT_WIN_WIDTH);
} // namespace state

auto launch() noexcept -> renderer& {
    auto static lazy_static = renderer{};
    return lazy_static;
}

renderer::renderer() noexcept {
    // GLUT requires argc and argv to be passed to their init function,
    // which we don't to forward.
    // Therefore, we create our own dummy values and forward those instead.
    static int dummy_argc = 1;
    static char dummy_arg[] = "";
    static char* dummy_argv[] = {dummy_arg};
    glutInit(&dummy_argc, dummy_argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(
        config::DEFAULT_WIN_POS_X,
        config::DEFAULT_WIN_POS_Y
    );
    glutInitWindowSize(config::DEFAULT_WIN_WIDTH, config::DEFAULT_WIN_HEIGHT);
    glutCreateWindow(config::WIN_TITLE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glClearColor(
        config::DEFAULT_BG_COLOR.r,
        config::DEFAULT_BG_COLOR.g,
        config::DEFAULT_BG_COLOR.b,
        config::DEFAULT_BG_COLOR.a
    );

    glutDisplayFunc(render);
    glutReshapeFunc(resize);

    display_info();
}

auto renderer::set_world(struct world& world) noexcept -> renderer& {
    state::world_ptr = &world;
    return *this;
}

[[noreturn]]
auto renderer::run() noexcept -> void {
    glutMainLoop();
    __builtin_unreachable(); // glutMainLoop is noreturn,
                             // but isn't marked as such.
}

auto render() noexcept -> void {
    auto const& camera_pos    = state::world_ptr->camera.pos;
    auto const& camera_lookat = state::world_ptr->camera.lookat;
    auto const& camera_up     = state::world_ptr->camera.up;
    auto const& camera_proj   = state::world_ptr->camera.projection;

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
        camera_pos.x,    camera_pos.y,    camera_pos.z,
        camera_lookat.x, camera_lookat.y, camera_lookat.z,
        camera_up.x,     camera_up.y,     camera_up.z
    );
    gluPerspective(
        camera_proj[0], state::aspect_ratio, camera_proj[1], camera_proj[2]
    );

    glColor3f(1.f, 0.f, 0.f);

    glBegin(GL_TRIANGLES);
        // Test.
        glVertex3f(0.f, 0.f, 0.f);
        glVertex3f(10.f, 0.f, 10.f);
        glVertex3f(10.f, 0.f, 0.f);
    glEnd();

    glutSwapBuffers();
}

auto resize(int const width, int height) -> void {
    // Prevent a divide by zero when window is too short.
    if (height == 0) {
        height = 1;
    }

    double const aspect_ratio
        = static_cast<double>(width) / static_cast<double>(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect_ratio, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
}

auto display_info() -> void {
    using engine::config::prog_name;

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

} // namespace engine::render
