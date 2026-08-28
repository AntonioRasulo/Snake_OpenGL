#include "GameLevel.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include <fstream>
#include <sstream>

namespace game {

    void GameLevel::Load(const char *file)
    {
        // clear old data
        Bricks.clear();

        unsigned int tileCode;

        std::string line;
        std::ifstream fstream(file);

        std::vector<std::pair<int, int>> tileData;

        if(fstream)
        {
            // Read each line from level file
            while(std::getline(fstream, line))
            {
                std::istringstream sstream(line);
                std::vector<unsigned int> row;
                int xCoord = -1;
                // Read each word separated by spaces
                while(sstream >> tileCode)
                {
                    if(xCoord == -1)
                    {
                        xCoord = tileCode;
                    }
                    else
                    {
                        std::pair<int, int> coordPair(xCoord, tileCode);
                        tileData.push_back(coordPair);
                        xCoord = -1;
                    }
                }
                if(tileData.empty() == false)
                {
                    Init(tileData);
                }
            }
        }
    }

    void GameLevel::Init(std::vector<std::pair<int, int>> tileData)
    {
        for(auto coords : tileData)
        {
            glm::vec2 pos(SQUARE_SIZE * coords.first, SQUARE_SIZE * coords.second);
            glm::vec2 size(SQUARE_SIZE, SQUARE_SIZE);
            GameObject obj(pos, size, 
                           Utility::ResourceManager::GetTexture("bricks"),
                           glm::vec3(0.8f, 0.8f, 0.7f));
            Bricks.push_back(obj);
        }
    }

    void GameLevel::Draw(Utility::SpriteRenderer &renderer)
    {
        for(auto& brick: Bricks)
        {
            brick.Draw(renderer);
        }
    }

}