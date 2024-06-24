#pragma once
#include <stdlib.h>
#include "../gml/GML.h"

/**
 * @brief Basic triangle type
 */
typedef struct {
    vec3 vert[3];
    vec3 frag[3];
    vec2 uv[3];
    vec3 color[3];
    vec3 norm;
    vec2 bb_top_left;
    vec2 bb_bottom_right;
    char valid;
} triangle_t;

/**
 * @brief Mesh type
 * @param index*        Index buffer - edge between i and i+1 for i in {2*x | x in R}
 * @param vertex*       Vertex buffer - all verticies in mesh
 * @param normal*       Normal buffer - normal vectors for each vertex
 * @param uv*           UV buffer - uv-coordinates for each vertex
 * @param ibuff_size    Size of index buffer
 * @param vbuff_size    Size of vertex buffer
 * @param nbuff_size    Size of normal buffer
 * @param uvbuff_size   Size of uv buffer
 */
typedef struct {
    unsigned long long* index;
    vec4* vertex;
    vec3* normal;
    vec2* uv;

    int ibuff_size;
    int vbuff_size;
    int nbuff_size;
    int uvbuff_size;
} mesh_t;

/**
 * @brief Texture type - stores texture buffers
 * @param has_diffuse_buff      Bool with value 1 if it has a color buffer, else 0
 * @param diffuse_buff_size     Size of diffuse texture buffer
 * @param diffuse*              Pointer to the diffuse texture buffer
 * @param has_specular_buff     Bool with value 1 if it has a color buffer, else 0
 * @param specular_buff_size    Size of specular texture buffer
 * @param specular              Pointer to the specular texture buffer
 */
typedef struct {
    int width;
    int height;
    vec3* data;
} texture_t;

/**
 * @brief Basic instance of a render object
 * @param pos       Position of the mesh in global coordinates
 * @param mesh      Pointer to the objects mesh
 * @param texture   Pointer to the texture of the object
 */
typedef struct {
    vec4 pos;

    int num_meshes;
    mesh_t* meshes;

    texture_t* textures;
} render_object_t;

/**
 * @brief Basic light struct
 * @param pos       Position of the lihgt in global coordinates
 * @param ambient   Color/intensity of the ambient light - vec3
 * @param diffuse   Color/intensity of the diffuse light - vec3
 * @param specular  Color/intensity of the specular light - vec3
 */
typedef struct {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
} light_t;

/**
 * @brief Convert vec3 color to an integer representation for xlib framebuffer
 * @param c     Color to be converted
 * @returns Integer representaion in RGB 24-bit representation of the vec3 color
 */
int color_v(const vec3* c);

/**
 * @brief Convert a normalized RGB color to an integer representation for xlib framebuffer
 * @param r     Red color channel
 * @param g     Green color channel
 * @param b     Blye color channel
 * @returns Integer representaion in RGB 24-bit representation of the vec3 color
 */
int color_f(float r, float g, float b);

/**
 * @brief Computes the barycentric coordinate given the three corners of a triangle and a point inside - vec2
 * @param vert  The three verticies of the triangle
 * @param p     A point inside the triangle
 * @return The barycentric coordinate of the point - vec3
 */
vec3 get_barycentric_coordinate2d(const vec2 vert[3], const vec2* p);

/**
 * @brief Computes the barycentric coordinate given the three corners of a triangle and a point inside - vec3
 * @param vert  The three verticies of the triangle
 * @param p     A point inside the triangle
 * @return The barycentric coordinate of the point - vec3
 */
vec3 get_barycentric_coordinate3d(const vec3 vert[3], const vec3* p);

void bresenham(int* framebuffer, int s_width, int s_height, int x0, int y0, int x1, int y1, int color);
