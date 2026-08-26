#ifndef GAMELEVEL
#define GAMELEVEL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.hpp"
#include <vector>
#include "SpriteRenderer.hpp"

namespace game {

class GameLevel
{
    private:
        // Initialize level from tile data
        void Init(std::vector<std::pair<int, int>> tileData);
    public:
        glm::vec2 snakeStartPos;

        std::vector<GameObject> Bricks;

        // Load level from file
        void Load(const char *file);

        // Render level
        void Draw(Utility::SpriteRenderer &renderer);
};

}

#endif