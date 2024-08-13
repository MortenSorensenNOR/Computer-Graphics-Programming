#pragma once

#include <vector>
#include <memory>
#include <string>
#include <sstream>

#include <glm/glm.hpp>

#include "../../core_utils/core_utils.h"
#include "../engine_utils/mesh.h"
#include "../engine_utils/texture.h"
#include "../engine_utils/light.h"

#include "camera.h"
#include "asset_load.h"
#include "tinyxml2/tinyxml2.h"

struct SceneObject {
    std::size_t id;

    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 position;

    glm::mat4 model;
    glm::mat4 scale_matrix;

    std::vector<std::size_t> mesh_indices;
    std::vector<std::size_t> texture_indices;

    std::size_t parent;
    std::vector<std::size_t> children;

    SceneObject(std::size_t scene_parent = -1, std::size_t id = 0) {
        this->id = id;
        mesh_indices = {};
        texture_indices = {};
        children = {};
        parent = scene_parent;
    }
};

class Scene {
private:
    std::shared_ptr<Camera> camera;

    std::vector<std::shared_ptr<SceneObject>> scene_objects;
    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Texture<float>>> textures;

public:
    Scene(std::size_t screen_width, std::size_t screen_height);
    ~Scene();

    /**
     * @brief Loads in a scene from a specific file and reads the corresponding models into the scene object tree
     * @param scene --- pointer to the scene that is being populated
     */
    int load_from_file(std::string assets_path, std::string scene_file_name);

    /**
     * @brief Updates the transforms for each scene object in the scene given its rotation, scale and position
     * @param scene --- pointer to the scene to update
     */
    int update();

    /**
     * @brief Get the current scenes camera
     * @return Returns a shared pointer to the camera struct
     */
    std::shared_ptr<Camera> get_camera();

    /**
     * @brief Get the scene root object
     * @return Returns a shared pointer to the scene root
     */
    std::shared_ptr<SceneObject> get_scene_root();

    /**
     * @brief Get scene object by id
     * @return Returns a shared pointer to the scene object
     */
    std::shared_ptr<SceneObject> get_scene_object(std::size_t id);

    /**
     * @brief Get scene mesh by id
     * @return Returns a shared pointer to the mesh
     */
    std::shared_ptr<Mesh> get_mesh(std::size_t idx);

private:
    int _load_object_tree(std::string assets_path, tinyxml2::XMLElement* root_element, const std::size_t root_object);

    glm::vec3 _string_to_vec3(const std::string& string);
    glm::vec4 _string_to_vec4(const std::string& string);
    
    int _propagete_and_calculate_transforms(std::shared_ptr<SceneObject>& root);
};



