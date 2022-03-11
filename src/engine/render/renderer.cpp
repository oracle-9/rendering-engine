#include "engine/render/renderer.hpp"

#include "engine/config.hpp"
#include "engine/render/world/world.hpp"
#include "util/pretty_print.hpp"

#include <GL/freeglut.h>
#include <fmt/core.h>

namespace engine::render {

auto render() noexcept -> void;
auto render_group(group const& root) -> void;
auto resize(int width, int height) -> void;
auto display_info() -> void;

namespace state {
    // Will eventually allow "reverting" to default world to undo changes.
    auto aspect_ratio = config::DEFAULT_ASPECT_RATIO;
    auto default_world_mut = config::DEFAULT_WORLD;
    auto* world_ptr = &default_world_mut;
} // namespace state

auto launch() noexcept -> renderer& {
    auto static lazy_static = renderer{};
    return lazy_static;
}

renderer::renderer() noexcept {
    // GLUT requires argc and argv to be passed to their init function,
    // which we don't want to forward.
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

    render_group(state::world_ptr->root);

    glutSwapBuffers();
}

// TODO: Implement non-recursively.
auto render_group(group const& root) -> void {
    // TODO: Finish implementing transforms.
    // for (auto const& transform : root->transforms) {
    //     switch (transform.kind) {
    //         using enum decltype(transform::kind);
    //         case translate:
    //             glTranslatef(
    //                 transform.translate.x,
    //                 transform.translate.y,
    //                 transform.translate.z
    //             );
    //             break;
    //         case rotate:
    //             glRotatef(
    //                 transform.rotate[0],
    //                 transform.rotate[1],
    //                 transform.rotate[2],
    //                 transform.rotate[3]
    //             );
    //             break;
    //         case scale:
    //             glScalef(
    //                 transform.scale.x,
    //                 transform.scale.y,
    //                 transform.scale.z
    //             );
    //             break;
    //     }
    // }
    for (auto const& model : root.models) {
        float const* i = model.coords.data();
        float const* const end = i + model.coords.size();
        glBegin(GL_TRIANGLES);
        for ( ; i != end; i += 3) {
            glVertex3fv(i);
        }
        glEnd();
    }
    for (auto const& child_node : root.children) {
        render_group(child_node);
    }
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
        "Vendor: {}\n",
        reinterpret_cast<char const*>(glGetString(GL_VENDOR))
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
