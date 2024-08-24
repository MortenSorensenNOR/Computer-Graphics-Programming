#include "types.h"
#include "vertex_shader.h"

void TR::vertex_shader(const std::vector<TR::Vertex>& in_vertices, const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj, std::vector<TR::Vertex>& out_vertices) {
    glm::mat3 transpose_inverse_model = glm::mat3(glm::transpose(glm::inverse(model)));

    for (std::size_t i = 0; i < in_vertices.size(); i++) {
        const Vertex* in_vert = &in_vertices.at(i);
        Vertex* out_vert = &out_vertices.at(i);

        out_vert->position = proj * view * model * in_vert->position;
        out_vert->normal = glm::normalize(transpose_inverse_model * in_vert->normal);
        out_vert->tex_coords = in_vert->tex_coords;
    }
}
