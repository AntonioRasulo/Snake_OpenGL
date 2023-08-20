#pragma once

#include <glm/glm.hpp>
#include "Shader.hpp"
#include "Texture.hpp"

namespace Utility {

    class SpriteRenderer
    {

        Shader m_shader;
        unsigned int m_quadVAO;
        void initRenderData();

    public:
        SpriteRenderer(Shader& shader);
        ~SpriteRenderer();

        void DrawSprite(Texture2D& texture, glm::vec2 position,
                        glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
                        glm::vec3 color = glm::vec3(1.0f));

    };

}
