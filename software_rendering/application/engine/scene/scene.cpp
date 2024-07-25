#include "scene.h"

int scene_init(Scene_t* scene, std::size_t screen_width, std::size_t screen_height) {
    scene->scene_root = new SceneObject_t;
    scene->scene_root->parent = NULL;
    scene->scene_root->children = new std::vector<SceneObject_t*>();
    
    if (!scene->scene_root) {
        return 1;
    }

    camera_init(&scene->camera, screen_width, screen_height);

    scene->meshes = new std::vector<Mesh>();
    scene->textures = new std::vector<Texture<float>>();

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
        _scene_load_object_tree(scene, root, scene->scene_root);

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
    for (int i = 0; i < scene->meshes->size(); ++i) {
        Mesh* m = &scene->meshes->at(i);
        buffer_free(m->vertexes);
        buffer_free(m->normals);
        buffer_free(m->uvs);
        buffer_free(m->indices);
    }

    for (int i = 0; i < scene->textures->size(); ++i) {
        Texture<float>* t = &scene->textures->at(i);
        buffer_free(scene->textures->at(i).data);
    }

    for (std::size_t i = 0; i < scene->scene_root->children->size(); i++) {
        _scene_free_scene_objects(scene->scene_root->children->at(i));
    }
    free(scene->scene_root->children);
    free(scene->scene_root);

    return 0;
}

int _scene_free_scene_objects(SceneObject_t* object) {
    printf("Hi %ld\n", object->children->size());

    for (std::size_t i = 0; i < object->children->size(); i++) {
        _scene_free_scene_objects(object->children->at(i));
    }

    free(object->children);
    free(object->mesh_indices);
    free(object->texture_indices);
    free(object);

    return 0;
}

int _scene_load_object_tree(Scene_t* scene, tinyxml2::XMLElement* root_element, SceneObject_t* root_object) {
    for (tinyxml2::XMLElement* object = root_element->FirstChildElement("Object"); object; object = object->NextSiblingElement("Object")) {
        glm::vec3 object_position = _scene_string_to_vec3(object->Attribute("position"));
        glm::vec3 object_rotation = _scene_string_to_vec3(object->Attribute("rotation"));
        glm::vec3 object_scale = _scene_string_to_vec3(object->Attribute("scale"));

        // TODO: Implement loading of multiple textures, s.a. diffuse, specular, AO, normal
        tinyxml2::XMLElement* object_model = object->FirstChildElement("Model");
        std::string model_mesh_path = object_model->Attribute("mesh");
        std::string model_texture_path = object_model->Attribute("texture");

        // Allocate new texture and mesh
        // Mesh object_mesh();
        // Texture<float> object_texture();
        //
        // scene->meshes->push_back(object_mesh);
        // scene->textures->push_back(object_texture);

        SceneObject_t* scene_object = new SceneObject_t;
        scene_object->parent = root_object;
        scene_object->children = new std::vector<SceneObject_t*>();

        scene_object->position = object_position;
        scene_object->scale = object_scale;
        scene_object->rotation = object_rotation;
        scene_object->mesh_indices = new std::vector<std::size_t>();
        scene_object->texture_indices = new std::vector<std::size_t>();
        
        root_object->children->push_back(scene_object);
        _scene_load_object_tree(scene, object, scene_object);
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

int _scene_propagete_and_calculate_transforms(SceneObject_t* root) {
    // Check if we are the scene root
    if (!root->parent) {
        for (std::size_t i = 0; i < root->children->size(); ++i) {
            _scene_propagete_and_calculate_transforms(root->children->at(i));
        }
        return 0;
    } 
    
    // Calculate transform of this SceneObject
    
    // Propagete transform to children
    for (std::size_t i = 0; i < root->children->size(); ++i) {
        _scene_propagete_and_calculate_transforms(root->children->at(i));
    }

    return 0;
}
