#include "../../utilities/Types.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

typedef struct {
    glm::vec3 cam_pos;

    int vert_buf_size;
    int index_buf_size;

    glm::vec3* vert_buf;
    glm::vec3* norm_buf;
    glm::vec3* frag_buf;
    glm::vec2* uv_buf;
    unsigned long long* index_buf;
} primitives_assembler_input_t;

typedef struct {
    int tri_buf_size;
    triangle_t* tri_buf;
} primitives_assembler_output_t;

// Assumes vert has 3 verticies
static inline void pa_calculate_bounding_box(glm::vec3* vert, glm::vec2* bb_top_left, glm::vec2* bb_bottom_right);

int primitives_assembler(primitives_assembler_input_t* in, primitives_assembler_output_t* out);
