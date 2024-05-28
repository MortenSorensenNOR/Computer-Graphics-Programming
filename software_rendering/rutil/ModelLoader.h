#pragma once
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <stdio.h>
#include <stdlib.h>

#include "RenderUtil.h"

/**
 * @brief Converts an assimp vector 3 to a gml vec3 
 * @param p     Assimp vector3
 * @return Returns the converted vec3
 */
vec3 assimp_vec3_to_vec3(const struct aiVector3D* p);

/**
 * @brief Parses a .obj file into a model object using assimp
 * @param fpath     Path to the file to be parsed
 * @param scene     Pointer to the scene to be populated
 * @return Returns 0 for success and 1 for failure
 */
int parse_obj(const char* fpath, scene_t* scene);

/**
 * @brief Loads a texture from a fpath into a vec3 color buffer, and populates the buffer_size variable. Uses std_image.
 * @param fpath         The file path of the texture to be loaded
 * @param buffer        The destination buffer of the texture
 * @param buffer_size   The size variable of the buffer
 * @return Returns 0 for success and 1 for failure
 */
int load_texture(const char* fpath, vec3** buffer, int* texture_width, int* texture_height);

/**
 * @brief Frees the scene struct
 * @param scene     Pointer to the struct to be freed
 */
int free_scene(scene_t* scene);
