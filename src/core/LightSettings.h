#ifndef LIGHT_SETTINGS_H
#define LIGHT_SETTINGS_H

#include "glm/vec3.hpp"

struct LightSettings {
    bool enabled;
    glm::vec3 look_at_vector;
    glm::vec3 color;
};

#endif // LIGHT_SETTINGS_H
