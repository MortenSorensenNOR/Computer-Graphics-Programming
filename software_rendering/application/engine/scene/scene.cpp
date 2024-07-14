#include "scene.h"

int scene_init(Scene_t* scene) {
    scene->scene_root = (SceneObject_t*)malloc(sizeof(SceneObject_t));
    scene->scene_root->parent = NULL;
    
    if (!scene->scene_root) {
        return 1;
    }

    return 0;
}

int scene_update(Scene_t* scene) {
    return _scene_propagete_and_calculate_transforms(scene->scene_root);
}

int scene_free(Scene_t* scene) {
    return 0;
}

int _scene_propagete_and_calculate_transforms(SceneObject_t* root) {
    // Check if we are the scene root
    if (!root->parent) {
        for (std::size_t i = 0; i < root->num_children; ++i) {
            _scene_propagete_and_calculate_transforms(root->children[i]);
        }
        return 0;
    } 
    
    // Calculate transform of this SceneObject

    
    // Propagete transform to children
    for (std::size_t i = 0; i < root->num_children; ++i) {
        _scene_propagete_and_calculate_transforms(root->children[i]);
    }

    return 0;
}
