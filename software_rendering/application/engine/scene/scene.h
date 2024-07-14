#pragma once

#include "../../core_utils/core_utils.h"

typedef struct SceneObject_t {
    SceneObject_t* parent;
    SceneObject_t** children;
    std::size_t num_children;

    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 position;
    glm::vec3 velocity;

    glm::mat4 model;
    glm::mat4 scale_matrix;
} SceneObject_t;

typedef struct {
    SceneObject_t* scene_root;
} Scene_t;

int scene_init(Scene_t* scene);

int scene_update(Scene_t* scene);

int scene_free(Scene_t* scene);

int _scene_propagete_and_calculate_transforms(SceneObject_t* root);
