#include "engine/render/render.hpp"

#include "engine/config.hpp"
#include "engine/render/layout/world/group/group.hpp"
#include "engine/render/layout/world/group/model.hpp"
#include "engine/render/layout/world/group/transform/transform.hpp"
#include "engine/render/state.hpp"
#include "generator/primitives/box.hpp"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

int iii = 0;

namespace engine::render {

auto static render_axis() noexcept -> void;
auto static render_lookat_indicator() noexcept -> void;
auto static render_group(Group const& root) noexcept -> void;


auto render() noexcept -> void {
    auto const& camera = *state::camera_ptr;
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // TODO: Use glm transforms instead of GLUT to avoid casting.

    // glMultMatrixf(
    //     glm::value_ptr(
    //         glm::lookAt(camera.pos, camera.lookat, camera.up)
    //     )
    // );
    // glTranslatef(-camera.pos.x, -camera.pos.y, -camera.pos.z);

#   define DBL(x) static_cast<double>(x)
    gluLookAt(
        DBL(camera.pos.x),    DBL(camera.pos.y),    DBL(camera.pos.z),
        DBL(camera.lookat.x), DBL(camera.lookat.y), DBL(camera.lookat.z),
        DBL(camera.up.x),     DBL(camera.up.y),     DBL(camera.up.z)
    );
#   undef DBL

    glPolygonMode(GL_FRONT, state::polygon_mode);
    glLineWidth(state::line_width);
    if (state::enable_axis) {
        render_axis();
    }
    if (state::enable_lookat_indicator) {
        render_lookat_indicator();
    }
    render_group(state::world_ptr->root);
    iii = 0;
    glutSwapBuffers();
}

auto resize(int const width, int height) noexcept -> void {
    // Prevent a divide by zero when window is too short.
    if (height == 0) {
        height = 1;
    }

    auto const& camera_proj = state::camera_ptr->projection;

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

auto static render_axis() noexcept -> void {
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

    glColor3fv(glm::value_ptr(config::DEFAULT_FG_COLOR));
}

auto static render_lookat_indicator() noexcept -> void {
    auto static const lookat_model = ::generator::generate_box(0.5f, 1).value();
    glColor3fv(glm::value_ptr(config::LOOKAT_INDICATOR_COLOR));
    auto const& translate = state::camera_ptr->lookat;
    glPushMatrix();
    glTranslatef(translate.x, translate.y, translate.z);
    glBegin(GL_TRIANGLES);
    for (auto const& vertex : lookat_model) {
        glVertex3fv(glm::value_ptr(vertex));
    }
    glEnd();
    glPopMatrix();
    glColor3fv(glm::value_ptr(config::DEFAULT_FG_COLOR));
}

// TODO: Implement non-recursively.
auto static render_group(Group const& root) noexcept -> void {
    glPushMatrix();

    for (auto const& transform : root.transforms) {
        switch (transform.kind) {
            using enum Transform::Kind;

            case TRANSLATE:
                glTranslatef(
                    transform.translate.x,
                    transform.translate.y,
                    transform.translate.z
                );
                break;

            case ROTATE:
                // TODO: FIX THIS, NOT WORKING WITH TIME!
                glRotatef(
                    transform.rotate.rotate[0],
                    transform.rotate.rotate[1],
                    transform.rotate.rotate[2],
                    transform.rotate.rotate[3]
                );
                break;

            case SCALE:
                glScalef(
                    transform.scale.x,
                    transform.scale.y,
                    transform.scale.z
                );
                break;
        }
    }

    for (auto const& model : root.models) {
        glBindBuffer(GL_ARRAY_BUFFER, state::bind[iii]);
        glVertexPointer(3,GL_FLOAT,0,0);
        glDrawArrays(GL_TRIANGLES,0,state::buffers[iii].size()/3);
        iii++;
    }
    for (auto const& child_node : root.children) {
        render_group(child_node);
    }

    glPopMatrix();
}

} // namespace engine::render
