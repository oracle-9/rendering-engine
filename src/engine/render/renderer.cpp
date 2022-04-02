#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "engine/render/renderer.hpp"

#include "engine/config.hpp"
#include "engine/render/keyboard.hpp"
#include "engine/render/world/world.hpp"
#include "util/coord_conv.hpp"

#include <GL/freeglut.h>
#include <algorithm>
#include <cctype>
#include <fmt/core.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <nonnull_ptr.hpp>
#include <spdlog/spdlog.h>

namespace engine::render {

auto render() noexcept -> void;
auto update_camera(int) noexcept -> void;
auto render_axis() noexcept -> void;
auto render_group(group const& root) noexcept -> void;
auto resize(int width, int height) noexcept -> void;
auto key_down(unsigned char key, int x, int y) noexcept -> void;
auto key_up(unsigned char key, int x, int y) noexcept -> void;
auto display_info() -> void;

namespace state {

auto static bg_color = config::BG_COLOR;
auto static fg_color = config::FG_COLOR;

auto static enable_axis = config::ENABLE_AXIS;
auto static polygon_mode = static_cast<GLenum>(config::POLYGON_MODE);
auto static line_width = config::LINE_WIDTH;

auto static kb = keyboard{};

auto static default_world_mut = config::WORLD;
auto static world_ptr = ptr::nonnull_ptr_to(default_world_mut);

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
    glutInitWindowPosition(config::WIN_POS_X, config::WIN_POS_Y);
    glutInitWindowSize(config::WIN_WIDTH, config::WIN_HEIGHT);
    glutCreateWindow(config::WIN_TITLE);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutTimerFunc(config::RENDER_TICK_MILLIS, update_camera, 0);
    glutKeyboardFunc(key_down);
    glutKeyboardUpFunc(key_up);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(
        config::BG_COLOR.r,
        config::BG_COLOR.g,
        config::BG_COLOR.b,
        config::BG_COLOR.a
    );
    glPolygonMode(GL_FRONT, config::POLYGON_MODE);
    glLineWidth(config::LINE_WIDTH);
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

    // TODO: Use glm transforms instead of GLUT to avoid casting.
    // glMultMatrixf(
    //     glm::value_ptr(
    //         glm::lookAt(camera.pos, camera.lookat, camera.up)
    //     )
    // );
    // glTranslatef(-camera.pos.x, -camera.pos.y, -camera.pos.z);

    gluLookAt(
        static_cast<double>(camera.pos.x), static_cast<double>(camera.pos.y), static_cast<double>(camera.pos.z),
        static_cast<double>(camera.lookat.x), static_cast<double>(camera.lookat.y), static_cast<double>(camera.lookat.z),
        static_cast<double>(camera.up.x), static_cast<double>(camera.up.y), static_cast<double>(camera.up.z)
    );

    glPolygonMode(GL_FRONT, state::polygon_mode);
    glLineWidth(state::line_width);
    if (state::enable_axis) {
        render_axis();
    }
    render_group(state::world_ptr->root);
    glutSwapBuffers();
}

// TODO: implement rotation and zoom.
auto update_camera(int) noexcept -> void {
    using enum config::kb_keys;

    auto const& kb = state::kb;
    auto& camera_pos = state::world_ptr->camera.pos;

    // TODO: skip camera movement if no key is presssed.
    // if (not kb.pressed(KEY_MOVE_LEFT)
    //     and not kb.pressed(KEY_MOVE_RIGHT)
    //     and not kb.pressed(KEY_MOVE_UP)
    //     and not kb.pressed(KEY_MOVE_DOWN)
    // ) {
    //     goto SKIP_CAMERA_MOVE;
    // }

    ::util::cartesian_to_spherical_inplace(camera_pos);

    if (kb.pressed(KEY_MOVE_LEFT) and not kb.pressed(KEY_MOVE_RIGHT)) {
        camera_pos[2] -= config::CAM_ROTATE_ANGLE;
    } else if (kb.pressed(KEY_MOVE_RIGHT) and not kb.pressed(KEY_MOVE_LEFT)) {
        camera_pos[2] += config::CAM_ROTATE_ANGLE;
    }

    // TODO: prevent wrap arounds.
    if (kb.pressed(KEY_MOVE_UP) and not kb.pressed(KEY_MOVE_DOWN)) {
        // camera_pos[1] = std::max(0.f, camera_pos[1] - config::CAM_ROTATE_ANGLE);
        camera_pos[1] -= config::CAM_ROTATE_ANGLE;
    } else if (kb.pressed(KEY_MOVE_DOWN) and not kb.pressed(KEY_MOVE_UP)) {
        // camera_pos[1] = std::min(glm::pi<float>(), camera_pos[1] + config::CAM_ROTATE_ANGLE);
        camera_pos[1] += config::CAM_ROTATE_ANGLE;
    }

    // Zoom in and out
    if (kb.pressed(KEY_MOVE_CLOSER) and not kb.pressed(KEY_MOVE_FURTHER)) {
        camera_pos[0] -= config::CAM_ZOOM_STEP;
        if (camera_pos[0] < config::CAM_ZOOM_STEP)
            camera_pos[0] = config::CAM_ZOOM_STEP;
    } else if (kb.pressed(KEY_MOVE_FURTHER) and not kb.pressed(KEY_MOVE_CLOSER)) {
        camera_pos[0] += config::CAM_ZOOM_STEP;
    }

    ::util::spherical_to_cartesian_inplace(camera_pos);

    // TODO: skip camera movement if no key is presssed.
    // SKIP_CAMERA_MOVE:
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
    glPushMatrix();

    for (auto const& transform : root.transforms) {
        switch (transform.kind) {
            using enum transform::kind_t;
            case translate:
                glTranslatef(
                    transform.translate.x,
                    transform.translate.y,
                    transform.translate.z
                );
                break;
            case rotate:
                glRotatef(
                    transform.rotate[0],
                    transform.rotate[1],
                    transform.rotate[2],
                    transform.rotate[3]
                );
                break;
            case scale:
                glScalef(
                    transform.scale.x,
                    transform.scale.y,
                    transform.scale.z
                );
                break;
        }
    }

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

    glPopMatrix();
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
        static_cast<double>(camera_proj[0]),
        static_cast<double>(width) / static_cast<double>(height),
        static_cast<double>(camera_proj[1]),
        static_cast<double>(camera_proj[2])
    );
    glMatrixMode(GL_MODELVIEW);
}

auto key_down(unsigned char const key, int, int) noexcept -> void {
    state::kb.press(key);
    switch (key) {
        using enum config::kb_keys;
        case KEY_TOGGLE_AXIS:
            state::enable_axis = not state::enable_axis;
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
        case KEY_THINNER_LINES:
            state::line_width = std::max(
                state::line_width - config::LINE_WIDTH_STEP,
                config::LINE_WIDTH_MIN
            );
            break;
        case KEY_THICKER_LINES:
            state::line_width = std::min(
                state::line_width + config::LINE_WIDTH_STEP,
                config::LINE_WIDTH_MAX
            );
            break;
        default:
            break;
    }
}

auto key_up(unsigned char key, int, int) noexcept -> void {
    if (std::isalpha(key)) {
        key = static_cast<unsigned char>(tolower(key));
    }
    state::kb.release(key);
}

auto display_info() -> void {
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
