#pragma once

#include <vector>
#include <string>
#include <sstream>

#include <glm/glm.hpp>

#include "../engine_utils/mesh.h"
#include "../engine_utils/texture.h"
#include "../../core_utils/core_utils.h"

#include "tinyxml2/tinyxml2.h"

typedef struct SceneObject_t {
    SceneObject_t* parent;
    SceneObject_t** children;
    std::size_t num_children;

    std::size_t* mesh_indices;
    std::size_t* texture_indices;
    std::size_t num_meshes;

    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 position;
    glm::vec3 velocity;

    glm::mat4 model;
    glm::mat4 scale_matrix;
} SceneObject_t;

typedef struct {
    SceneObject_t* scene_root;

    std::vector<Mesh> meshes;
    std::vector<Texture<float>> textures;
} Scene_t;

/**
 * @brief Initialize the scene structure
 * @param scene --- pointer to the scene to be initialized
 */
int scene_init(Scene_t* scene);

/**
 * @brief Loads in a scene from a specific file and reads the corresponding models into the scene object tree
 * @param scene --- pointer to the scene that is being populated
 */
int scene_load_scene_from_file(Scene_t* scene, std::string assets_path, std::string scene_file_name);

/**
 * @brief Updates the transforms for each scene object in the scene given its rotation, scale and position
 * @param scene --- pointer to the scene to update
 */
int scene_update(Scene_t* scene);

/**
 * @brief Frees the scene structure and resources
 * @param scene --- pointer to the scene to be freed
 */
int scene_free(Scene_t* scene);

int _scene_propagete_and_calculate_transforms(SceneObject_t* root);

glm::vec3 _scene_string_to_vec3(const std::string& string);
glm::vec4 _scene_string_to_vec4(const std::string& string);
