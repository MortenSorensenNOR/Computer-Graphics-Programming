#include "scene.h"

Scene::Scene(std::size_t screen_width, std::size_t screen_height) {
    std::shared_ptr<SceneObject> root = std::make_shared<SceneObject>();
    scene_objects.push_back(std::move(root));

    camera = std::make_shared<Camera>(screen_width, screen_height);
}

Scene::~Scene() {
    for (int i = 0; i < meshes.size(); ++i) {
        Mesh* m = meshes.at(i).get();
        buffer_free(m->vertexes);
        buffer_free(m->indices);
    }

    for (int i = 0; i < textures.size(); ++i) {
        Texture<float> *t = textures.at(i).get();
        buffer_free(textures.at(i)->data);
    }
}

int Scene::load_from_file(std::string assets_path, std::string scene_file_name) {
    std::string scene_path = assets_path + scene_file_name;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(scene_path.c_str()) == tinyxml2::XML_SUCCESS) {
        tinyxml2::XMLElement *scene_element = doc.FirstChildElement("Scene");

        tinyxml2::XMLElement *camera = scene_element->FirstChildElement("Camera");
        glm::vec3 camera_position = _string_to_vec3(camera->Attribute("position"));
        glm::vec3 camera_forward = _string_to_vec3(camera->Attribute("forward"));
        glm::vec3 camera_up = _string_to_vec3(camera->Attribute("up"));
        float fov = (float)std::atoi(camera->Attribute("fov"));

        this->camera->set_config(camera_position, glm::vec3(0.0, 1.0, 0.0), 0.0, -90.0, fov);

        tinyxml2::XMLElement *root = scene_element->FirstChildElement("Objects");
        _load_object_tree(assets_path, root, 0);

    } else {
        printf("Could not load scene from file\n");
        return 1;
    }

    printf("Scene loaded: %s\n", scene_file_name.c_str());

    return 0;
}

int Scene::update() {
    // _propagete_and_calculate_transforms(scene_objects.at(0));
    camera->update_vectors();
    camera->update_view();
    return 0;
}

std::shared_ptr<Camera> Scene::get_camera() {
    return camera;
}

std::shared_ptr<SceneObject> Scene::get_scene_root() {
    return scene_objects.at(0);
}

std::shared_ptr<SceneObject> Scene::get_scene_object(std::size_t id) {
    return scene_objects.at(id);
}

std::shared_ptr<Mesh> Scene::get_mesh(std::size_t idx) {
    return meshes.at(idx); 
}

// ############################
// ######### INTERNAL #########
// ############################

int Scene::_propagete_and_calculate_transforms(std::shared_ptr<SceneObject>& root) {
    // Check if we are the scene root
    if (!root->parent) {
        for (std::size_t i = 0; i < root->children.size(); ++i) {
            std::size_t child_id = root->children.at(i);
            _propagete_and_calculate_transforms(scene_objects.at(child_id));
        }
        return 0;
    }

    // TODO: Calculate transform of this SceneObject

    // Propagete transform to children
    for (std::size_t i = 0; i < root->children.size(); ++i) {
        std::size_t child_id = root->children.at(i);
        _propagete_and_calculate_transforms(scene_objects.at(child_id));
    }

    return 0;
}


int Scene::_load_object_tree(std::string assets_path, tinyxml2::XMLElement *root_element, const std::size_t root_object) {
    for (tinyxml2::XMLElement *object = root_element->FirstChildElement("Object"); object; object = object->NextSiblingElement("Object")) {
        glm::vec3 object_position = _string_to_vec3(object->Attribute("position"));
        glm::vec3 object_rotation = _string_to_vec3(object->Attribute("rotation"));
        glm::vec3 object_scale = _string_to_vec3(object->Attribute("scale"));

        // TODO: Implement loading of multiple textures, s.a. diffuse, specular, AO,
        // normal
        tinyxml2::XMLElement *object_model = object->FirstChildElement("Model");
        std::string model_mesh_path = object_model->Attribute("mesh");
        std::string model_texture_path = object_model->Attribute("texture");

        // Allocate new texture and mesh
        std::vector<std::size_t> mesh_indices = model_load_from_path(meshes, assets_path + model_mesh_path);

        std::shared_ptr<SceneObject> scene_object = std::make_shared<SceneObject>(root_object, scene_objects.size());
        scene_object->position = object_position;
        scene_object->scale = object_scale;
        scene_object->rotation = object_rotation;
        scene_object->mesh_indices = mesh_indices;


        scene_object->model = glm::scale(glm::mat4(1.0f), object_scale);
        scene_object->model = glm::translate(scene_object->model, scene_object->position);

        scene_objects.push_back(scene_object);
        scene_objects.at(root_object)->children.push_back(scene_object->id);
        _load_object_tree(assets_path, object, scene_object->id);
    }

    return 0;
}

glm::vec3 Scene::_string_to_vec3(const std::string &string) {
    std::stringstream ss(string);
    std::string value;
    std::vector<float> values;

    while (std::getline(ss, value, ',')) {
        values.push_back(std::stof(value));
    }

    assert(values.size() == 3);
    return glm::vec3(values[0], values[1], values[2]);
}

glm::vec4 Scene::_string_to_vec4(const std::string &string) {
    std::stringstream ss(string);
    std::string value;
    std::vector<float> values;

    while (std::getline(ss, value, ',')) {
        values.push_back(std::stof(value));
    }

    assert(values.size() == 4);
    return glm::vec4(values[0], values[1], values[2], values[3]);
}

