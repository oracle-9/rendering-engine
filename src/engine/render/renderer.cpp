#include "engine/render/renderer.hpp"

#include "engine/config.hpp"
#include "engine/render/keyboard.hpp"
#include "engine/render/world/world.hpp"
#include "util/pretty_print.hpp"

#include <GL/freeglut.h>
#include <fmt/core.h>
#include <glm/gtc/type_ptr.hpp>

namespace engine::render {

auto render() noexcept -> void;
auto update_camera(int) noexcept -> void;
auto render_axis() noexcept -> void;
auto render_group(group const& root) noexcept -> void;
auto resize(int width, int height) noexcept -> void;
auto display_info() -> void;

namespace state {
    auto bg_color = config::DEFAULT_BG_COLOR;
    auto fg_color = config::DEFAULT_FG_COLOR;

    auto polygon_mode = static_cast<GLenum>(config::DEFAULT_POLYGON_MODE);

    auto kb = keyboard{};



    auto default_world_mut = config::DEFAULT_WORLD;
    auto* world_ptr = &default_world_mut;


} // namespace state

auto get() -> renderer& {
    auto static lazy_static = renderer{};
    return lazy_static;
}

renderer::renderer() {
    // GLUT requires argc and argv to be passed to their init function,
    // which we don't want to forward.
    // Therefore, we create our own dummy values and forward those instead.
    static int dummy_argc = 1;
    static char dummy_arg[] = "";
    static char* dummy_argv[] = {dummy_arg};
    glutInit(&dummy_argc, dummy_argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(
        config::DEFAULT_WIN_POS_X,
        config::DEFAULT_WIN_POS_Y
    );
    glutInitWindowSize(config::DEFAULT_WIN_WIDTH, config::DEFAULT_WIN_HEIGHT);
    glutCreateWindow(config::WIN_TITLE);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutTimerFunc(config::RENDER_TICK_MILLIS, update_camera, 0);
    glutKeyboardFunc([](unsigned char const key, int, int) {
        switch (key) {
            using enum config::kb_keys;
            case KEY_MOVE_UP:
            case KEY_MOVE_LEFT:
            case KEY_MOVE_DOWN:
            case KEY_MOVE_RIGHT:
                state::kb.press(key);
                glutPostRedisplay();
                break;
            case KEY_NEXT_POLYGON_MODE:
                using state::polygon_mode;
                switch (polygon_mode) {
                    case GL_POINT:
                        polygon_mode = GL_LINE;
                        break;
                    case GL_LINE:
                        polygon_mode = GL_FILL;
                        break;
                    case GL_FILL:
                        polygon_mode = GL_POINT;
                        break;
                    default:
                        __builtin_unreachable();
                }
                break;
            default:
                break;
        }
    });
    glutKeyboardUpFunc([](unsigned char const key, int, int) {
        switch (key) {
            using enum config::kb_keys;
            case KEY_MOVE_UP:
            case KEY_MOVE_LEFT:
            case KEY_MOVE_DOWN:
            case KEY_MOVE_RIGHT:
                state::kb.release(key);
                glutPostRedisplay();
                break;
            default:
                break;
        }
    });

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(
        state::bg_color.r,
        state::bg_color.g,
        state::bg_color.b,
        state::bg_color.a
    );

    display_info();
}

auto renderer::set_world(world& world) noexcept -> renderer& {
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
    auto const& camera = state::world_ptr->camera;
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(
        camera.pos.x,    camera.pos.y,    camera.pos.z,
        camera.lookat.x, camera.lookat.y, camera.lookat.z,
        camera.up.x,     camera.up.y,     camera.up.z
    );
    glPolygonMode(GL_FRONT, state::polygon_mode);
    render_axis();
    render_group(state::world_ptr->root);
    glutSwapBuffers();
}

// TODO: implement rotation and zoom.
auto update_camera(int) noexcept -> void {
    using state::kb;
    using enum config::kb_keys;

    auto& camera_pos = state::world_ptr->camera.pos;

    if (kb.pressed(KEY_MOVE_UP) and not kb.pressed(KEY_MOVE_DOWN)) {
        camera_pos.y += config::CAM_TRANSL_FACTOR;
    } else if (kb.pressed(KEY_MOVE_DOWN) and not kb.pressed(KEY_MOVE_UP)) {
        camera_pos.y -= config::CAM_TRANSL_FACTOR;
    }

    if (kb.pressed(KEY_MOVE_LEFT) and not kb.pressed(KEY_MOVE_RIGHT)) {
        camera_pos.x -= config::CAM_TRANSL_FACTOR;
    } else if (kb.pressed(KEY_MOVE_RIGHT) and not kb.pressed(KEY_MOVE_LEFT)) {
        camera_pos.x += config::CAM_TRANSL_FACTOR;
    }

    glutPostRedisplay();
    glutTimerFunc(config::RENDER_TICK_MILLIS, update_camera, 0);
}

auto render_axis() noexcept -> void {
    glBegin(GL_LINES);

    // x axis.
    glColor3fv(glm::value_ptr(config::AXIS_COLOR[0]));
    glVertex3f(-config::X_AXIS_HALF_LEN, 0.f, 0.f);
    glVertex3f(config::X_AXIS_HALF_LEN, 0.f, 0.f);

    // y axis.
    glColor3fv(glm::value_ptr(config::AXIS_COLOR[1]));
    glVertex3f(0.f, -config::Y_AXIS_HALF_LEN, 0.f);
    glVertex3f(0.f, config::Y_AXIS_HALF_LEN, 0.f);

    // z axis.
    glColor3fv(glm::value_ptr(config::AXIS_COLOR[2]));
    glVertex3f(0.f, 0.f, -config::Z_AXIS_HALF_LEN);
    glVertex3f(0.f, 0.f, config::Z_AXIS_HALF_LEN);

    glEnd();

    glColor3fv(glm::value_ptr(state::fg_color));
}

// TODO: Implement non-recursively.
auto render_group(group const& root) noexcept -> void {
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
        auto const* i = model.coords.data();
        auto const* const end = i + model.coords.size();
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

auto resize(int const width, int height) noexcept -> void {
    // Prevent a divide by zero when window is too short.
    if (height == 0) {
        height = 1;
    }

    auto const& camera_proj = state::world_ptr->camera.projection;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(
        camera_proj[0],
        static_cast<double>(width) / static_cast<double>(height),
        camera_proj[1],
        camera_proj[2]
    );
    glMatrixMode(GL_MODELVIEW);
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
