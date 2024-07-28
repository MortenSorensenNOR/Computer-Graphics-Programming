#pragma once

#include <vector>
#include <stdexcept>

#include <glm/glm.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>

#include "stb_image/stb_image.h"
#include "../../core_utils/buffer.h"
#include "../engine_utils/mesh.h"
#include "../engine_utils/texture.h"

/**
 * @brief Loads the meshes from an object file and pushes the meshes into the meshes vector of the scene.
 * @param meshes    The vector of all meshes in the scene
 * @param path      The path to the object which is being parsed
 * @return A vector of all the indicies of the loaded meshes
 */
std::vector<std::size_t> model_load_from_path(std::vector<Mesh>& meshes, std::string path);

/**
 * @brief Loads a texture from a path and pushes it into the vector of all textures in the scene.
 * @param textures  The vector of all textures in the scene
 * @param path      The path to the texture being loaded
 * @param type      The type of texture that is being loaded, i.e. DIFFUSE, NORMAL, SPECULAR, AO or DISPLACEMENT
 * @return The index of the texture into the textures vector.
 */
std::size_t texture_load_from_path(std::vector<Texture<float>>& textures, std::string path, TextureType type);
