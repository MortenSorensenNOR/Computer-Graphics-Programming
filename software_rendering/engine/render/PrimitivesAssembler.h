#include "../rutil/RenderUtil.h"

typedef struct {
    vec3 cam_pos;

    int vert_buf_size;
    int index_buf_size;

    vec3* vert_buf;
    vec3* norm_buf;
    vec3* frag_buf;
    vec2* uv_buf;
    unsigned long long* index_buf;
} primitives_assembler_input_t;

typedef struct {
    int tri_buf_size;
    triangle_t* tri_buf;
} primitives_assembler_output_t;

// Assumes vert has 3 verticies
static inline void pa_calculate_bounding_box(vec3* vert, vec2* bb_top_left, vec2* bb_bottom_right);

int primitives_assembler(primitives_assembler_input_t* in, primitives_assembler_output_t* out);
