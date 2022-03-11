#include "engine/render/renderer.hpp"

#include "engine/config.hpp"
#include "engine/render/world/world.hpp"
#include "util/pretty_print.hpp"

#include <GL/freeglut.h>
#include <fmt/core.h>

namespace engine::render {
using config::prog_name;

auto print_info() {
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

renderer::renderer() noexcept {
    {
        int dummy_argc = 1;
        char dummy_arg[] = "";
        char* dummy_argv[] = {dummy_arg};
        glutInit(&dummy_argc, dummy_argv);
    }

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(WIN_POS_X, WIN_POS_Y);
    glutInitWindowSize(WIN_SIZ_X, WIN_SIZ_Y);
    glutCreateWindow(WIN_TITLE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glClearColor(BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
    glutDisplayFunc([] {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glutSwapBuffers();
    });

    print_info();
}

[[noreturn]]
auto renderer::run(world const&) noexcept -> void {
    glutMainLoop();
    __builtin_unreachable();
}

} // namespace engine::render
