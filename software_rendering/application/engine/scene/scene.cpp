#include "scene.h"

int scene_init(Scene_t* scene, std::size_t screen_width, std::size_t screen_height) {
    scene->scene_root = (SceneObject_t*)malloc(sizeof(SceneObject_t));
    scene->scene_root->parent = NULL;
    
    if (!scene->scene_root) {
        return 1;
    }

    camera_init(&scene->camera, screen_width, screen_height);

    scene->meshes = std::vector<Mesh>();
    scene->textures = std::vector<Texture<float>>();

    return 0;
}

int scene_load_scene_from_file(Scene_t* scene, std::string assets_path, std::string scene_file_name) {
    std::string scene_path = assets_path + scene_file_name;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(scene_path.c_str()) == tinyxml2::XML_SUCCESS) {
        tinyxml2::XMLElement* scene_element = doc.FirstChildElement("Scene");

        tinyxml2::XMLElement* camera = scene_element->FirstChildElement("Camera");
        glm::vec3 camera_position = _scene_string_to_vec3(camera->Attribute("position"));
        glm::vec3 camera_forward = _scene_string_to_vec3(camera->Attribute("forward"));
        glm::vec3 camera_up = _scene_string_to_vec3(camera->Attribute("up"));
        float fov = (float)std::atoi(camera->Attribute("fov"));

        camera_set_config(&scene->camera, camera_position, camera_forward, camera_up, fov);
        
        // TODO: Parse entire scene tree for all scene objects
        tinyxml2::XMLElement* root = scene_element->FirstChildElement("Objects");

        for (tinyxml2::XMLElement* object = root->FirstChildElement("Object"); object; object = object->NextSiblingElement("Object")) {
            glm::vec3 object_position = _scene_string_to_vec3(object->Attribute("position"));
            glm::vec3 object_rotation = _scene_string_to_vec3(object->Attribute("rotation"));
            glm::vec3 object_scale = _scene_string_to_vec3(object->Attribute("scale"));

            // TODO: Implement loading of multiple textures, s.a. diffuse, specular, AO, normal
            tinyxml2::XMLElement* object_model = object->FirstChildElement("Model");
            std::string model_mesh_path = object_model->Attribute("mesh");
            std::string model_texture_path = object_model->Attribute("texture");
        }
    } else {
        printf("Could not load scene from file\n");
        return 1;
    }

    printf("Scene path: %s\n", scene_path.c_str());
        
    return 0;
}

int scene_update(Scene_t* scene) {
    return _scene_propagete_and_calculate_transforms(scene->scene_root);
}

int scene_free(Scene_t* scene) {
    for (int i = 0; i < scene->meshes.size(); ++i) {
        Mesh* m = &scene->meshes[i];
        if (m->vertex_buffer_size)
            free(m->vertexes);
        if (m->normal_buffer_size)
            free(m->normals);
        if (m->uv_buffer_size)
            free(m->uvs);
        if (m->index_buffer_size)
            free(m->indices);
    }

    for (int i = 0; i < scene->textures.size(); ++i) {
        Texture<float>* t = &scene->textures[i];
        if (t->size)
            free(t->data);
    }

    free(scene->scene_root);

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

glm::vec3 _scene_string_to_vec3(const std::string& string) {
    std::stringstream ss(string);
    std::string value;
    std::vector<float> values;

    while (std::getline(ss, value, ',')) {
        values.push_back(std::stof(value));
    }

    assert(values.size() == 3);
    return glm::vec3(values[0], values[1], values[2]);
}

glm::vec4 _scene_string_to_vec4(const std::string& string) {
    std::stringstream ss(string);
    std::string value;
    std::vector<float> values;

    while (std::getline(ss, value, ',')) {
        values.push_back(std::stof(value));
    }

    assert(values.size() == 4);
    return glm::vec4(values[0], values[1], values[2], values[3]);
}
